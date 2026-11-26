// <editor-fold defaultstate="collapsed" desc="Disclaimer ">
/*******************************************************************************
* Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.
*
* SOFTWARE LICENSE AGREEMENT:
* 
* Microchip Technology Incorporated ("Microchip") retains all ownership and
* intellectual property rights in the code accompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,
* WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
* STATUTORY DUTY),STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE,
* FOR ANY INDIRECT, SPECIAL,PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL
* LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE,
* HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR
* THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS CODE,
* SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
*******************************************************************************/
//</editor-fold>
#include <xc.h>
#include <libq.h>
  
#include "../mc_lib/motor_control.h"
#include "motor_control_function_mapping.h"

#include "../mcc_generated_files/adc1.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/pwm.h"
#include "../mcc_generated_files/tmr1.h"
#include "../mcc_generated_files/sccp1_tmr.h"
#include "../mcc_generated_files/X2Cscope/X2Cscope.h"
#include "../hal/board_service.h"
#include "../hal/measure.h"

#include "control.h"  
#include "estim.h"
#include "fault.h"
#include "filter.h"
#include "hall.h"
#include "mcapp_watchdog.h"
#include "math_asm.h"
#include "startup.h"
#include "sat_PI.h"
#include "util.h"
#include "userparms.h"

#define ADCBUF_INV_A_IPHASE1   (int16_t)(-ADCBUF0)
#define ADCBUF_INV_A_IPHASE2   (int16_t)(-ADCBUF4)

#define ADCBUF_SPEED_REF_A      ADCBUF17        
#define ADCBUF_TEMP_SENSOR      ADCBUF12
#define ADCBUF_VBUS_16          (uint16_t)ADCBUF15

//<editor-fold defaultstate="collapsed" desc="Variable Declaration">


volatile UGF_T uGF;

CTRL_PARM_T ctrlParm;
MCAPP_DATA_T mcappData;
MCAPP_ESTIMATOR_HALL_T hallEstimator;
MCAPP_ESTIMATOR_HALL_HE_T hallEstimatorHE;

volatile int16_t thetaElectrical = 0;

MC_ALPHABETA_T valphabeta,ialphabeta;
MC_SINCOS_T sincosTheta;
MC_DQ_T vdq,idq,idqCmdRaw,vdqCmd;
MC_DUTYCYCLEOUT_T pwmDutycycle;
MC_ABC_T   vabc,iabc;

MC_PIPARMIN_T piInputIq;
MC_PIPARMOUT_T piOutputIq;
MC_PIPARMIN_T piInputId;
MC_PIPARMOUT_T piOutputId;
MC_PIPARMIN_T piInputOmega;
MC_PIPARMOUT_T piOutputOmega;

MCAF_PISTATE_T omegaCtrl;
MCAF_PISTATE_T iqCtrl;
MCAF_PISTATE_T idCtrl;
MCAF_SAT_DETECT_T motorSatDetect;
MC_DQ_T idqCtrlOutLimit; 
MCAPP_U_DUTYCYCLE_ABC dabcRaw;
MCAPP_U_DUTYCYCLE_ABC dabc;

MCAPP_MEASURE_T measureInputs;
MCAPP_BOARD_DATA pboard;
MCAPP_MOTOR_STARTUP_DATA motorStartUpData;
MCAPP_VELOCITY_CONTROL_DATA velocityControl;

int16_t adcBuffer;
uint16_t speedRefTimer;
uint16_t runningInit;

/* Zero-throttle start interlock, evaluated in the ADC ISR at the
 * throttle sample rate. Re-armed by the state-machine init routines. */
static bool throttleSafeStarted = false;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Single tune with automatic soft start">
/*
 * One throttle map, one current limit, one torque ramp.
 *
 * The current-loop gains are softer for the first TUNE_SOFT_START_MS after the
 * throttle is applied, then fade to the full gains over TUNE_BLEND_MS. The
 * rider does not select anything: the soft phase repeats on every start and is
 * reset by a throttle release, a stop or a fault.
 *
 * Works with TORQUE_MODE defined in userparms.h (throttle -> torque command).
 */

/* ---------------- TUNING KNOBS - edit these ---------------- */
/* NORM_CURRENT(amps) converts amps into current counts */
#define TUNE_MAX_CURRENT          (MAX_TORQUE_CURRENT)  /* hard cap: 5.48 A */
#define TUNE_MIN_CURRENT          MIN_TORQUE_CURRENT    /* 0 A; no low-end step */
#define TUNE_RAMP_A_PER_S         15.0                  /* torque rise rate; lower = softer */
#define TUNE_FALL_A_PER_S         15.0                  /* torque fall rate; smooth roll-off */
#define TUNE_THROTTLE_FILTER_MS   20.0                  /* throttle low-pass filter */
#define TUNE_SOFT_START_MS        500                   /* soft gains for this long */
#define TUNE_BLEND_MS             300                   /* fade to full gains */

/* Soft-phase gains: 50 % of the moderate run gains. */
#define TUNE_SOFT_KP              (KIP - (KIP / 2))
#define TUNE_SOFT_KI              (KII / 2)
#define TUNE_FULL_KP              (KIP)
#define TUNE_FULL_KI              (KII)
/* ------------------------------------------------------------ */

/* milliseconds -> throttle samples (one sample = THROTTLE_SAMPLE_COUNT + 1 = 21 ADC interrupts, about 1.05 ms) */
#define TUNE_MS_TO_SAMPLES(ms) \
    ((uint16_t)(((uint32_t)(ms) * (PWM_FREQUENCY / 1000UL)) / (THROTTLE_SAMPLE_COUNT + 1)))

#define SOFT_START_SAMPLES     TUNE_MS_TO_SAMPLES(TUNE_SOFT_START_MS)
#define BLEND_SAMPLES          TUNE_MS_TO_SAMPLES(TUNE_BLEND_MS)
#define BLEND_STEP             ((int16_t)(32767L / BLEND_SAMPLES))

/* torque rise rate: amps per second -> current counts per throttle sample */
#define TUNE_RAMP_STEP(a_per_s) \
    ((int16_t)((((float)(a_per_s) / (float)NORM_CURRENT_CONST) * \
      ((float)(THROTTLE_SAMPLE_COUNT + 1) / (float)PWM_FREQUENCY)) + 0.5f))

/* Q15 IIR coefficient for the throttle low-pass filter. */
#define TUNE_THROTTLE_FILTER_ALPHA_Q15 \
    ((uint16_t)(32767.0f * \
      ((1000.0f * (THROTTLE_SAMPLE_COUNT + 1)) / PWM_FREQUENCY) / \
      (TUNE_THROTTLE_FILTER_MS + \
      ((1000.0f * (THROTTLE_SAMPLE_COUNT + 1)) / PWM_FREQUENCY)) + 0.5f))

typedef struct
{
    int16_t  blend;        /* 0 = soft gains ... 32767 = full gains      */
    int16_t  kp;           /* active current-loop Kp                     */
    int16_t  ki;           /* active current-loop Ki                     */
    int16_t  rampStep;     /* torque rise limit per sample               */
    int16_t  fallStep;     /* torque fall limit per sample               */
    int16_t  torqueCmd;    /* final torque command                       */
} TUNE_MODE_T;

TUNE_MODE_T tuneMode;

static uint16_t softStartTimer;
static int32_t throttleFilteredPot;
static bool throttleFilterInitialized;

/* a + (b - a) * t, with t from 0 to 32767 */
static int16_t TUNE_Lerp(int16_t a, int16_t b, int16_t t)
{
    if (t <= 0)
    {
        return a;
    }
    if (t >= INT16_MAX)
    {
        return b;
    }
    return (int16_t)(a + ((((int32_t)b - a) * t) >> 15));
}

/* Push the active current-loop gains into the controllers.
 * The Q format is unchanged, so nkp/nki do not need updating here. */
static void TUNE_ApplyGains(void)
{
    idCtrl.kp = tuneMode.kp;
    idCtrl.ki = tuneMode.ki;
    iqCtrl.kp = tuneMode.kp;
    iqCtrl.ki = tuneMode.ki;
}

static void TUNE_ApplyBlend(void)
{
    const int16_t t = tuneMode.blend;

    tuneMode.kp       = TUNE_Lerp(TUNE_SOFT_KP, TUNE_FULL_KP, t);
    tuneMode.ki       = TUNE_Lerp(TUNE_SOFT_KI, TUNE_FULL_KI, t);
    tuneMode.rampStep = TUNE_RAMP_STEP(TUNE_RAMP_A_PER_S);
    tuneMode.fallStep = TUNE_RAMP_STEP(TUNE_FALL_A_PER_S);

    TUNE_ApplyGains();
}

/* Abort the torque command and re-arm the soft phase. */
void TUNE_ClearCommand(void)
{
    tuneMode.torqueCmd = 0;
    tuneMode.blend     = 0;
    softStartTimer     = 0;
    TUNE_ApplyBlend();
}

void TUNE_Init(void)
{
    tuneMode.blend     = 0;
    tuneMode.torqueCmd = 0;
    softStartTimer     = 0;
    TUNE_ApplyBlend();
}

/* Call once per throttle sample */
void TUNE_Update(int16_t potValue, int16_t throttleRef)
{
    (void)potValue;

    if (mcappData.state == MCAPP_FAULT)
    {
        TUNE_ClearCommand();
    }

    /* ---------------- automatic soft-start timer ---------------- */
    if (!mcappData.runMotor)
    {
        /* Throttle released or stopped: arm a fresh soft start. */
        softStartTimer = 0;
        if (tuneMode.blend != 0)
        {
            tuneMode.blend = 0;
            TUNE_ApplyBlend();
        }
    }
    else if (softStartTimer < SOFT_START_SAMPLES)
    {
        softStartTimer++;
    }

    /* ---------------- fade from soft to full gains ---------------- */
    {
        const int32_t target = (softStartTimer >= SOFT_START_SAMPLES) ? INT16_MAX : 0;
        int32_t b = tuneMode.blend;
        const int16_t previousBlend = tuneMode.blend;

        if (b < target)
        {
            b += BLEND_STEP;
            if (b > target)
            {
                b = target;
            }
        }
        else if (b > target)
        {
            b -= BLEND_STEP;
            if (b < target)
            {
                b = target;
            }
        }
        tuneMode.blend = (int16_t)b;

        /* Gains only change while the blend is moving. */
        if (tuneMode.blend != previousBlend)
        {
            TUNE_ApplyBlend();
        }
    }

    /* ---------------- torque command with rise/fall limits ---------------- */
    {
        int16_t raw;
        int16_t cmd = tuneMode.torqueCmd;

        if (throttleRef == 0)
        {
            raw = 0;
        }
        else
        {
            /* Quadratic throttle curve: 50 % throttle commands about
             * 25 % current, giving useful low-speed resolution without
             * changing the 5.48 A full-throttle cap. */
            const int16_t curve = (int16_t)(UTIL_mulss(throttleRef, throttleRef) >> 15);
            raw = (int16_t)(UTIL_mulss(curve, TUNE_MAX_CURRENT) >> 15);
            if (throttleRef >= 32760)
            {
                raw = TUNE_MAX_CURRENT;
            }
            else if (raw == 0)
            {
                raw = 1;  /* about 4 mA; avoids a quantized zero step */
            }
        }

        if (raw < cmd)
        {
            int32_t next = (int32_t)cmd - tuneMode.fallStep;
            cmd = (next < raw) ? raw : (int16_t)next;
        }
        else
        {
            int32_t next = (int32_t)cmd + tuneMode.rampStep;
            cmd = (next > raw) ? raw : (int16_t)next;
        }
        tuneMode.torqueCmd = cmd;
    }
}
//</editor-fold>


inline static void MCAPP_ScaleQ15(const MC_ABC_T *pabc_in, MC_DUTYCYCLEOUT_T *pabc_out, int16_t k)
{
    pabc_out->dutycycle1 = UTIL_MulQ15(pabc_in->a, k);
    pabc_out->dutycycle2 = UTIL_MulQ15(pabc_in->b, k);
    pabc_out->dutycycle3 = UTIL_MulQ15(pabc_in->c, k);
}

inline static void MCAF_ApplyDCLinkVoltageCompensation(const MC_ABC_T *pvabc,
                                                        MC_ABC_T *pdabc,
                                                        int16_t rVdc,
                                                        int16_t rVdc_q)
{
    pdabc->a = (int16_t) (UTIL_mulss(pvabc->a, rVdc) >> rVdc_q);
    pdabc->b = (int16_t) (UTIL_mulss(pvabc->b, rVdc) >> rVdc_q);
    pdabc->c = (int16_t) (UTIL_mulss(pvabc->c, rVdc) >> rVdc_q);
}

inline static int16_t MCAF_ComputeReciprocalDCLinkVoltage(int16_t vdc)
{
    if (vdc <= MCAF_RVDC_MIN_VDC)
    {
        return MCAF_RVDC_MIN;
    }
    else
    {
        return __builtin_divf(MCAF_RVDC_MIN_VDC, vdc);
    }
}

static void CommutationStep(void)
{
    MCAPP_EstimatorHallStep(&hallEstimator);
    thetaElectrical = MCAPP_EstimatorHallCommutationAngle(&hallEstimator);
    mcappData.measuredSpeed = MCAPP_EstimatorHallElectricalFrequency(&hallEstimator);
     

    MCAPP_StartupTransitioningStep(&motorStartUpData,
            &idqCmdRaw, 
            mcappData.runDirection
        );
    
//    ctrlParm.qVdRef = idqCmdRaw.d;
//    ctrlParm.qVqRef = idqCmdRaw.q;
    
    const MCAPP_U_ANGLE_ELEC thetaAdjusted = thetaElectrical + MCAPP_STARTUPGetThetaError(&motorStartUpData);
    MCAPP_U_ANGLE_ELEC deltatheta;
    deltatheta = thetaAdjusted - thetaElectrical;
    
    thetaElectrical += deltatheta;   
}

//<editor-fold defaultstate="collapsed" desc="DoControl() Description">
// ***************************************************************************
/* Function:
    DoControl()

  Summary:
    Executes one PI iteration for each of the three loops Id,Iq,Speed

  Description:
    This routine executes one PI iteration for each of the three loops
    Id,Iq,Speed
 */
//</editor-fold>
static void DoControl(void)
{  

    if(ctrlParm.speedRampCount < SPEEDREFRAMP_COUNT)
    {
        ctrlParm.speedRampCount++; 
    }
    else
    {
        ctrlParm.speedRampCount = 0;
        
        velocityControl.velocityCmdRateLimited = UTIL_LimitSlewRateSymmetrical(ctrlParm.targetSpeed,
                mcappData.measuredSpeed, velocityControl.slewRateLimit1);
        
        int16_t limitPos, limitNeg;
        if(ctrlParm.qVelRef > 0){
            limitPos = velocityControl.slewRateLimitAccel;
            limitNeg = velocityControl.slewRateLimitDecel;
        }
        else
        {
            limitPos = velocityControl.slewRateLimitDecel;
            limitNeg = velocityControl.slewRateLimitAccel;
        }
        
        ctrlParm.qVelRef = UTIL_LimitSlewRate(velocityControl.velocityCmdRateLimited,
                ctrlParm.qVelRef, limitPos, limitNeg);
        
#ifndef TORQUE_MODE
        
        const MCAF_SAT_STATE_T satState = motorSatDetect.state;
        MCAF_ControllerPIUpdate(
                ctrlParm.qVelRef,
                mcappData.measuredSpeed,
                &omegaCtrl,
                satState,
                &idqCmdRaw.q,
                mcappData.runDirection);
            
            ctrlParm.qVqRef = idqCmdRaw.q;
#else
        ctrlParm.qVqRef = ctrlParm.targetTorque;
#endif

        /* Field weakening is enabled only when the motor is already in the
         * high-speed region and a useful torque command is present.  The
         * d-axis current is limited to a conservative 1 A so the total
         * current vector still stays inside the 5.48 A phase-current cap. */
        int16_t fieldWeakeningCurrent = 0;
        if ((mcappData.state == MCAPP_RUNNING) &&
            (UTIL_Abs16(ctrlParm.qVqRef) >= FIELD_WEAKENING_MIN_IQ_COUNTS))
        {
            int32_t rpmAbs = MCAPP_SPEED_Q15_TO_RPM(mcappData.measuredSpeed);
            if (rpmAbs < 0)
            {
                rpmAbs = -rpmAbs;
            }

            if (rpmAbs > FIELD_WEAKENING_START_RPM)
            {
                int32_t scaled =
                    ((rpmAbs - FIELD_WEAKENING_START_RPM) *
                     (int32_t)FIELD_WEAKENING_CURRENT_COUNTS) /
                    (FIELD_WEAKENING_FULL_RPM - FIELD_WEAKENING_START_RPM);

                if (scaled > (int32_t)FIELD_WEAKENING_CURRENT_COUNTS)
                {
                    scaled = (int32_t)FIELD_WEAKENING_CURRENT_COUNTS;
                }
                fieldWeakeningCurrent = (int16_t)scaled;
            }
        }

        if (fieldWeakeningCurrent != 0)
        {
            const int16_t squaredLimit =
                UTIL_SignedSqrNoOverflow(TUNE_MAX_CURRENT) -
                UTIL_SignedSqrNoOverflow(fieldWeakeningCurrent);
            const int16_t qCurrentLimit = Q15SQRT(squaredLimit);

            ctrlParm.qVqRef = UTIL_LimitS16(
                    ctrlParm.qVqRef,
                    -qCurrentLimit,
                    qCurrentLimit);
        }

        ctrlParm.qVdRef = idqCmdRaw.d - fieldWeakeningCurrent;
    }   
        
    ctrlParm.qVdRef = idqCmdRaw.d;

    /* PI control for D */
    const int16_t vlim_d = UTIL_MulQ15(idqCtrlOutLimit.d, measureInputs.dcBusVoltage);

    idCtrl.outMax = vlim_d;
    idCtrl.outMin = -vlim_d;
    MCAF_ControllerPIUpdate(
            ctrlParm.qVdRef,
            idq.d,
            &idCtrl,
            MCAF_SAT_NONE,
            &vdqCmd.d,
            0);

    vdq.d = vdqCmd.d;

    /* Dynamic d-q adjustment
     with d component priority 
     vq=sqrt (vsLimit^2 - vd^2) 
     limit vq maximum to the one resulting from the calculation above */
        
    const int16_t vdSquared = UTIL_SignedSqr(vdq.d);
    const int16_t vdqSquaredLimit = UTIL_SignedSqr(UTIL_MulQ15(measureInputs.dcBusVoltage, MCAF_CURRENT_CTRL_DQ_MAGNITUDE_LIMIT));
        
    iqCtrl.outMax = Q15SQRT(vdqSquaredLimit - vdSquared);
    iqCtrl.outMin = -iqCtrl.outMax;
    MCAF_ControllerPIUpdate(
                ctrlParm.qVqRef,
                idq.q,
                &iqCtrl,
                MCAF_SAT_NONE,
                &vdqCmd.q,
                0);
        vdq.q = vdqCmd.q;
    MCAF_SatDetect(&motorSatDetect, &idq, &vdq, measureInputs.dcBusVoltage);

    /* Latest commands and limits for the UART telemetry: if IQ tracks IQR
     * but both stop below TUNE_MAX_CURRENT, the throttle map is the limit;
     * if IQ lags IQR, the current loop is out of voltage/headroom. */
    mcappData.iqReference = ctrlParm.qVqRef;
    mcappData.vqCommand   = vdq.q;
    mcappData.vqLimit     = iqCtrl.outMax;
    mcappData.satState    = (uint16_t)motorSatDetect.state;
}
extern uint16_t thetaError;
static void FOC_FeedbackPath(void)
{
    MCAPP_MeasureCurrentCalibrate(&measureInputs);
    iabc.a = measureInputs.current.Ia;
    iabc.b = measureInputs.current.Ib;

    /* Calculate qId,qIq from qSin,qCos,qIa,qIb */
    MC_TransformClarke(&iabc,&ialphabeta);
    MC_TransformPark(&ialphabeta,&sincosTheta,&idq);

    /* Latest measured d/q currents for the UART telemetry. */
    mcappData.idMeasured = idq.d;
    mcappData.iqMeasured = idq.q;

#ifdef HALL_OFFSET_DETERMINATION
    Estim();
    thetaError = thetaElectrical - estimator.qRho;
#endif
    
    CommutationStep();
 
    MC_CalculateSineCosine(thetaElectrical,&sincosTheta);
}

static void FOC_ForwardPath(void)
{
    /* Calculate control values */
    DoControl();
    
    MC_TransformParkInverse(&vdq,&sincosTheta,&valphabeta);
    MC_TransformClarkeInverse(&valphabeta,&vabc);
    
    MCAF_ApplyDCLinkVoltageCompensation(&vabc, &dabcRaw, mcappData.rVdc, MCAF_RVDC_Q);
    
    MC_CalculateZeroSequenceModulation(&dabcRaw,&dabc,HAL_PARAM_MIN_DUTY_Q15, HAL_PARAM_MAX_DUTY_Q15);   
    MCAPP_ScaleQ15(&dabc,&pwmDutycycle,HAL_PARAM_PWM_PERIOD_COUNTS); 
    PWMDutyCycleSet(&pwmDutycycle);
}


//<editor-fold defaultstate="collapsed"  desc="InitControlParameters() Description">
// ***************************************************************************
/* Function:
    InitControlParameters()

  Summary:
    Function initializes control parameters

  Description:
    Initialize control parameters: PI coefficients, scaling constants etc.
 */
//</editor-fold> 
void InitControlParameters(void)
{   
    ctrlParm.speedRampCount = SPEEDREFRAMP_COUNT;
    
    idCtrl.kp = DKP;
    idCtrl.ki = DKI;
    idCtrl.nkp = DKNP;
    idCtrl.nki = DKNI;
    idCtrl.kc = DKC;
    idCtrl.outMax = 0;
    idCtrl.outMin = 0;
    idqCtrlOutLimit.d = MCAF_CURRENT_CTRL_D_OUT_LIMIT;
            
    iqCtrl.kp = QKP;
    iqCtrl.ki = QKI;
    iqCtrl.nkp = QKNP;
    iqCtrl.nki = QKNI;
    iqCtrl.kc = QKC;
    iqCtrl.outMax = 0;
    iqCtrl.outMin = 0;
    idqCtrlOutLimit.q = MCAF_CURRENT_CTRL_Q_OUT_LIMIT;
    
    omegaCtrl.kp = WKP;
    omegaCtrl.ki = WKI;
    omegaCtrl.nkp = WKNP;
    omegaCtrl.nki = WKNI;
    velocityControl.velocityCmdGain = INT16_MAX;
    
    omegaCtrl.kc = WKC;
    omegaCtrl.outMax = MCAF_VELOCITY_CTRL_IQ_OUT_LIMIT;
    omegaCtrl.outMin = -MCAF_VELOCITY_CTRL_IQ_OUT_LIMIT;

    TUNE_Init();
}

void initControlLoopState(){
    
    mcappData.rVdc = MCAF_ComputeReciprocalDCLinkVoltage(INT16_MAX);
    
    idCtrl.integrator = 0;
    iqCtrl.integrator = 0;
    omegaCtrl.integrator = 0;
	
    vdq.d = 0;
    vdq.q = 0;
    ctrlParm.qVdRef = 0;
    ctrlParm.qVqRef = 0;
    
    MCAF_SatInit(&motorSatDetect);
}

 inline static void MCAPP_CommutationTransitionToClosedLoop()
 {
    /*
     * The first time the velocity controller is executed, we want the output
     * to be continuous with the existing current command.
     */
    omegaCtrl.integrator = (int32_t) idqCmdRaw.q << 16; 
    motorStartUpData.counter = 0;
}

void MCAPP_RestartInit(void)
{
    MCAPP_BootstrapChargeInit(&pboard);
    measureInputs.current.status = 0;   //FOC Restart
    mcappData.runDirection = 1;
    MCAPP_MeasureCurrentInit(&measureInputs);
    
    velocityControl.slewRateLimit1      = VELOCITY_SLEWRATE_LIMIT1;
    velocityControl.slewRateLimitAccel  = VELOCITY_SLEWRATE_LIMIT_ACCEL;
    velocityControl.slewRateLimitDecel  = VELOCITY_SLEWRATE_LIMIT_DECEL;
    
    FaultFlagsReset();
    throttleSafeStarted = false;
}

void MCAPP_Restart(void)
{   
    bool bootstrapComplete = MCAPP_BootstrapChargeStepIsr(&pboard);
    
    if(bootstrapComplete){
        if (MCAPP_MeasureCurrentOffsetStatus(&measureInputs) == 0) {
                    MCAPP_MeasureCurrentOffset(&measureInputs);
        }
    }    
}

void MCAPP_StartingInit(void){
    ctrlParm.qVdRef = 0;
    ctrlParm.qVqRef = 0;
    idqCmdRaw.d = 0;
    idqCmdRaw.q = 0;
    ctrlParm.qVelRef = 0;
    
    mcappData.runDirection = 1;
    runningInit = 1;
    
    MCAPP_STARTUPReinit(&motorStartUpData,mcappData.runDirection);
    MCAPP_STARTUPEnable(&motorStartUpData);
    
    
    MCAPP_EstimatorHallStartupInit(&hallEstimator);
    InitEstimParm();
    
    MCAPP_MeasureAvgInit(&avgTargetRefValue,4);
    
    //FOC Initialize Integrators    
    idCtrl.integrator = 0;
    iqCtrl.integrator = 0;
    omegaCtrl.integrator = 0;
    
    mcappData.throttleSampleCounter = 0;

	mcappData.pedelecSampleCounter = 0;
    mcappData.pedelecSignalCalc = 0;
    mcappData.pedelecSignalStateVar = 0;
    mcappData.timerValue = 0;
    speedRefTimer = 0;
    
    
    //Enable PWM Min Duty
    PWMIdenticalDutyCycleSet(MIN_DUTY);
    EnablePWMOutputsInverterA();
}


void MCAPP_Starting(void){
    FOC_ForwardPath();
}

void MCAPP_RunningInit(void){
    MCAPP_CommutationTransitionToClosedLoop();
    runningInit = 1;
}

void MCAPP_Running(void){
    FOC_ForwardPath();
}

void MCAPP_StoppingInit(void){
    mcappData.targetReferenceValue = 0;
    ctrlParm.qVdRef = 0;
    ctrlParm.qVqRef = 0;
    ctrlParm.qVelRef = 0;
    ctrlParm.targetTorque = 0;

    PWMIdenticalDutyCycleSet(MIN_DUTY);
    DisablePWMOutputsInverterA();
    throttleSafeStarted = false;
    
    mcappData.timerValue = 0;
    MCAPP_MeasureAvgInit(&avgTargetRefValue,4);
    mcappData.fromStartPedelec = 0;
    
    mcappData.measuredSpeed = 0;   
	
	hallEstimator.omegaElectricalTrkLoop = 0;
    hallEstimator.trackingLoop.velocityEstimateUnfiltered = 0;
    hallEstimator.trackingLoop.angleError = 0;
    hallEstimator.trackingLoop.velocityIntegrator.x32 = 0;   
}

void MCAPP_FaultInit(void){
    IO_FAULT_SetHigh();
    PWMIdenticalDutyCycleSet(MIN_DUTY);
    DisablePWMOutputsInverterA();   /* both gates off: no active 3-phase short */
    mcappData.runMotor = 0;
    ctrlParm.targetTorque = 0;
    TUNE_ClearCommand();            /* keep the selected mode across the fault */
    throttleSafeStarted = false;
}


void MCAPP_StoppedInit(void){
    PWMIdenticalDutyCycleSet(MIN_DUTY);
    DisablePWMOutputsInverterA();
    FaultFlagsReset();
	
	//FOC Initialize Integrators
    idCtrl.integrator = 0;
    iqCtrl.integrator = 0;
    omegaCtrl.integrator = 0;
    vdq.d = 0;
    vdq.q = 0;
}

void MCAPP_Stopped(void){
    hallEstimator.deltaPos = 0;
    hallEstimator.positionTrkLoop = hallEstimator.prevPosition;
    hallEstimator.trackingLoop.velocityEstimateUnfiltered = 0;
    hallEstimator.trackingLoop.angleIntegrator.x32 = ((uint32_t)getRotorInitialPosition()<<15);
    hallEstimator.trackingLoop.angleError = 0;    
    hallEstimator.trackingLoop.velocityIntegrator.x32 = 0;    
}

bool stopping_complete(void){
    
    if(mcappData.measuredSpeed < 2500){
        return true;
    }
    else{
        return false;
    }
}

void MCAPP_StateMachineInit(void){
    mcappData.state = MCAPP_RESTART;
    mcappData.mode = 0;
    MCAPP_RestartInit();
    MCAPP_EstimatorHallInit(&hallEstimator);
}

inline static MCAPP_STATES_T MCAPP_DetermineNextState()
{
    const MCAPP_STATES_T this_state = mcappData.state;
    MCAPP_STATES_T next_state = this_state;
    
    switch(this_state){
        
        case MCAPP_RESTART:
        {
            if(MCAPP_MeasureCurrentOffsetStatus(&measureInputs) == 1)
            {
                next_state = MCAPP_STOPPED;
            }
        }
        break; 
        case MCAPP_STOPPED:
        {
            if(mcappData.runMotor)
            {
                next_state = MCAPP_STARTING;
            }
        }
        break;
        case MCAPP_STARTING:
        {
            if(mcappData.runMotor == 0){
                next_state = MCAPP_STOPPING;
            }
            else
            {
                next_state = MCAPP_RUNNING;
            }
        }
        break;
        case MCAPP_RUNNING:
        {
            if(mcappData.runMotor == 0){
                next_state = MCAPP_STOPPING;
            }
        }
        break;
        case MCAPP_STOPPING:
        {
            if(stopping_complete()){
                next_state = MCAPP_STOPPED;
            }
        }
        break;
        case MCAPP_FAULT:
        {
            if(mcappData.runMotor == 0){
                next_state = MCAPP_RESTART;
            }
        }
        break;
        default:
            next_state = MCAPP_RESTART;
    }
    return next_state; 
}

inline static void MCAPP_StateMachine_Dispatch(MCAPP_STATES_T next_state)
{
    const MCAPP_STATES_T this_state = mcappData.state;
    const bool state_changed = (next_state != this_state);
    mcappData.state = next_state;
    
    switch(next_state)
    {
        case MCAPP_RESTART:
        {
            if(state_changed)
            {
                MCAPP_RestartInit();
            }
            MCAPP_Restart();
        }
        break; 
        case MCAPP_STOPPED:
        {
            if(state_changed)
            {
                MCAPP_StoppedInit();
            }
            MCAPP_Stopped();
        }
        break;
        case MCAPP_STARTING:
        {
            if(state_changed)
            {
                MCAPP_StartingInit();
            } 
            MCAPP_Starting();
        }
        break;
        case MCAPP_RUNNING:
        {
            if(state_changed)
            {
                MCAPP_RunningInit();
            }  
            MCAPP_Running(); 
        }
        break;
        case MCAPP_STOPPING:
        {
            if(state_changed)
            {
                MCAPP_StoppingInit();
            }
            //Do nothing
        }
        break;
        case MCAPP_FAULT:
        {
            if(state_changed)
            {
                MCAPP_FaultInit();
            }            
        }
        break;
    }
}

inline static void MeasureInputs(void)
{
    measureInputs.current.Ia = ADCBUF_INV_A_IPHASE1;
    measureInputs.current.Ib = ADCBUF_INV_A_IPHASE2; 

    measureInputs.potValue = (int16_t)(ADCBUF_SPEED_REF_A >> 1);
    measureInputs.dcBusVoltage = (int16_t)(ADCBUF_VBUS_16 >> 1);
    
    faultUndervoltage.measure = (uint16_t)ADCBUF_VBUS_16;
    faultOvervoltage.measure = faultUndervoltage.measure;
    faultOverTempMOSFET.measure = ADCBUF_TEMP_SENSOR;  
}

static void TargetReference_Setting(void)
{
    speedRefTimer++;
    if(speedRefTimer == 4){ 
        speedRefTimer = 0;
        //Need to have a factor to determine the correct velocity in torque mode.
        //The desired speed should be around around 3700RPM which DS shows the most efficient.
        //Need vary target speed vs consumption using Dynamometer.
        
//        ctrlParm.targetSpeed = (int16_t)(__builtin_mulss(mcappData.targetReferenceValue, NOMINAL_SPEED_ELECTR - MINIMUM_SPEED_ELECTR)>>15)+ MINIMUM_SPEED_ELECTR;
        ctrlParm.targetSpeed = (int16_t)(__builtin_mulss(mcappData.targetReferenceValue, MCAF_VELOCITY_NOMINAL  - MCAF_VELOCITY_COMMAND_MIN)>>15)+ MCAF_VELOCITY_COMMAND_MIN;
    }
    
#ifdef TORQUE_MODE
    ctrlParm.targetTorque = tuneMode.torqueCmd;
#endif
}

/*
 * Throttle mapping (values come from userparms.h):
 *   potValue <= THROTTLE_LOW  (7547  = 0.76V)  -> throttle 0
 *   potValue >= THROTTLE_HIGH (23500 = 2.37V)  -> throttle 32767 (100%)
 *   in between                                  -> linear 0..32767
 */
void Throttle_Handle(void)
{
   int32_t Throttle_scale_temp;
   
   if(mcappData.throttleSampleCounter < THROTTLE_SAMPLE_COUNT)
   {
       mcappData.throttleSampleCounter++;
   }
   //Throttle is sampled every 20ADC ISRs. = 1mS.
   else
   {
       mcappData.throttleSampleCounter = 0;
       
       const int16_t rawPot = measureInputs.potValue;
        if (!throttleFilterInitialized)
        {
            throttleFilteredPot = rawPot;
            throttleFilterInitialized = true;
        }
        else
        {
            const int32_t error = (int32_t)rawPot - throttleFilteredPot;
            throttleFilteredPot += ((error * TUNE_THROTTLE_FILTER_ALPHA_Q15) >> 15);
        }
        int16_t pot = (int16_t)throttleFilteredPot;

        if (pot <= THROTTLE_LOW)
       {
           mcappData.throttleScaledValue = 0;
       }
       else
       {
           
           if(pot > THROTTLE_HIGH)
           {
               pot = THROTTLE_HIGH; // clamp to maximum defined range
           }

           Throttle_scale_temp = ((int32_t)pot * THROTTLE_SCALE_A_COEFFICIENT) - THROTTLE_SCALE_B_COEFFICIENT;
           if(Throttle_scale_temp < 0)
           {
               Throttle_scale_temp = 0;
           }
           mcappData.throttleScaledValue = (int16_t)(Throttle_scale_temp >> 10);
           if(mcappData.throttleScaledValue > 32767)
           {
               mcappData.throttleScaledValue = 32767;
           }
       }
       mcappData.targetReferenceValue = mcappData.throttleScaledValue;

       /* Zero-throttle start interlock.
        * Evaluated in the ISR at the 1 ms throttle sample rate, so it
        * does not depend on the main loop. Re-armed by
        * MCAPP_RestartInit(), MCAPP_StoppingInit() and MCAPP_FaultInit(). */
        if (!throttleSafeStarted)
       {
           if (measureInputs.potValue <= THROTTLE_LOW)
           {
               throttleSafeStarted = true;
           }
           mcappData.runMotor = 0;
       }
       else if (measureInputs.potValue > (THROTTLE_LOW + THROTTLE_HYSTERESIS))
       {
           mcappData.runMotor = 1;
       }
       else if (measureInputs.potValue <= THROTTLE_LOW)
       {
           mcappData.runMotor = 0;
       }

       TUNE_Update(measureInputs.potValue, mcappData.targetReferenceValue);
   }
}


//<editor-fold defaultstate="collapsed"     desc="ADC_ISR() Description">
// ***************************************************************************
/* Function:
   ADC_ISR()

  Summary:
   ADC_ISR routine

  Description:
    Does speed calculation and executes the vector update loop
    The ADC sample and conversion is triggered by the PWM period.
    The speed calculation assumes a fixed time interval between calculations.
 */
//</editor-fold>
void ADC_ISR (void)
{    
    //This measures Ia,Ib, Vbus, FETs temperature.
    MeasureInputs();
    
    if(mcappData.mode < 2){
        Throttle_Handle();
    }
    else{
//        PEDELEC_Sensor_Handle();
    }
    
    FOC_FeedbackPath();
    
    TargetReference_Setting();
    
    const MCAPP_STATES_T next_state = MCAPP_DetermineNextState();
    
    MCAPP_StateMachine_Dispatch(next_state);
    
    mcappData.rVdc = MCAF_ComputeReciprocalDCLinkVoltage(measureInputs.dcBusVoltage);
    
#ifdef FAULT_MONITOR_ENABLE  
    UndervoltageDetect();
    OvervoltageDetect();
    OvertemperatureDetectMOSFET();
#endif    
    
#ifdef X2CScope_ENABLE
    X2CScope_Update();
#endif
    BoardServiceStepIsr();
	WATCHDOG_TimerClear();
    
    adcBuffer = ADCBUF_INV_A_IPHASE1;
    HAL_ADC_InterruptFlagClear(HAL_ADC_CHANNEL_USED_FOR_ISR);  
}

void PWM_ISR(void){
	mcappData.state = MCAPP_FAULT;
}
/*
 End of file
 */

