//<editor-fold defaultstate="collapsed" desc="Disclaimer">
/* Microchip Technology Inc. and its subsidiaries.  You may use this software
 * and any derivatives exclusively with Microchip products.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */
 //</editor-fold>

#ifndef USERPARAMS_H
#define USERPARAMS_H

#include <xc.h> 
#include <stdbool.h>
#include "util.h"
#include "sat_PI_params.h"

/*
 Debug / telemetry interface selection
 -------------------------------------
 UART text telemetry and X2CScope share UART1, so select exactly one:

   TELEMETRY_UART      - human-readable text (default)
   TELEMETRY_X2CSCOPE  - X2CScope protocol

 Both use 115200 baud. Switching the mode also enables/disables the
 X2CScope calls in main.c and motor_drive.c.
 */
#define TELEMETRY_UART          0
#define TELEMETRY_X2CSCOPE      1

/* Fixed for the ESP32 link: UART1 carries text telemetry only. */
#define TELEMETRY_MODE          TELEMETRY_UART

#if (TELEMETRY_MODE == TELEMETRY_X2CSCOPE)
#define X2CScope_ENABLE
#endif

#define TELEMETRY_UART_ENABLE   (TELEMETRY_MODE == TELEMETRY_UART)
#define TELEMETRY_PERIOD_MS     100     // text telemetry period (10 Hz)

 /*
  Functioning Mode
  */
#define FAULT_MONITOR_ENABLE
#define TORQUE_MODE         //when commented out the default drive is constant speed


#define MOTOR_250W   // historical name; this block is the Maxon EC 48 V (634043)
  //#define MOTOR_2KW
  //#define MOTOR_1KW



  //#define HALL_OFFSET_DETERMINATION  //when experimenting to find the hall offset, it enables the PLL estim

  //******************************************************************************

  //******************************************************************************
  //******************************************************************************
  //Section: Motor Parameters, Derived Params for Calculations
  //******************************************************************************
#define SECTOR                     6

#define MCAF_VELOCITY_COMMAND_MAX           32000//27307 
#define MCAF_VELOCITY_COMMAND_MIN            1092 
#define MCAF_VELOCITY_NOMINAL               32000//21845

#define MINIMUM_SPEED_ELECTR                MCAF_VELOCITY_COMMAND_MIN
#define NOMINAL_SPEED_ELECTR                MCAF_VELOCITY_NOMINAL

#define VELOCITY_SLEWRATE_LIMIT1              32000
#define VELOCITY_SLEWRATE_LIMIT_ACCEL         1524
#define VELOCITY_SLEWRATE_LIMIT_DECEL          300
//******************************************************************************

//******************************************************************************
//******************************************************************************
// Throttle calibration - values measured from the installed potentiometer.
// (32767 counts = 3.3 V; 1 count = ~0.1007 mV)
//******************************************************************************
#define THROTTLE_LOW        7547   // 0.76 V - lower deadband / rest position
#define THROTTLE_HIGH       23500  // 2.37 V - full throttle
#define THROTTLE_HYSTERESIS 200    // 0.02 V - anti-chatter window
//******************************************************************************

// ***************************************************************************
// Section: Throttle Interpolator Constants
//******************************************************************************
#define THROTTLE_SCALE_A_COEFFICIENT (int)((33553408/(THROTTLE_HIGH - THROTTLE_LOW)))           
#define THROTTLE_SCALE_B_COEFFICIENT (long int)((33553408/(THROTTLE_HIGH - THROTTLE_LOW))*THROTTLE_LOW) 
#define THROTTLE_SAMPLE_COUNT         20      // THROTTLE_SAMPLE_COUNT = 1ms
//******************************************************************************
//******************************************************************************
//Section: Clock, PWM and Timer Parameters
//******************************************************************************
#define FCY     100000000UL          //100MHz
#define PWM_FREQUENCY 20000          //20kHz

/* Text telemetry period, expressed in ADC-ISR ticks (1 tick = 50 us @ 20 kHz) */
#define TELEMETRY_ISR_TICKS ((uint16_t)(TELEMETRY_PERIOD_MS * (PWM_FREQUENCY / 1000UL)))

#define TIMER_PRESCALER      64      //Timer peripheral prescaler

#define MIN_DUTY             0
//******************************************************************************
 //******************************************************************************
//Pedelec Signal Processing
//******************************************************************************
#define MAX_SIGNAL_FREQ     17      //in Hz as arbitrarily measured by pedaling
#define DECIMATION_FACTOR   100
#define MIN_REF_VALUE       1927    //(32767/DECIMATION_FACTOR)
#define MAX_REF_VALUE       32767
#define PEDELEC_SAMPLE_COUNT    4
#define PULSE_ABSCENCE_LIMIT   (uint32_t)(((float)FCY*(float)2.75)/(float)TIMER_PRESCALER) //2.75s limit, road standard is between 2-3s
#define STARTING_TIMER_COUNT   (uint32_t)((FCY*4)/(TIMER_PRESCALER*MAX_SIGNAL_FREQ)) 
#define PEDELEC_SIGNAL_CONST   (uint32_t)(FCY/TIMER_PRESCALER)
#define NORM_SIGNAL_CONST      (uint16_t)((float)32767/(float)MAX_SIGNAL_FREQ)
//******************************************************************************
 //******************************************************************************
//Hall Signal Processing
//******************************************************************************
#define STARTING_TIMER_COUNT   (uint32_t)((FCY*4)/(TIMER_PRESCALER*MAX_SIGNAL_FREQ))    
//Section: definitions for the Hall sensors angle interpolator/speed measurement
//******************************************************************************
#define PHASE_INC_CALC  (unsigned long)((float)FCY/((float)(TIMER_PRESCALER)*(float)(PWM_FREQUENCY))*(float)(65536/6))
#define SPEED_MULTI   (unsigned long)(((float)FCY/(float)(TIMER_PRESCALER*SECTOR))*(float)60) 




//MOTOR SPECIFICATIONS

#ifdef MOTOR_250W
    // Maxon EC 48 V, part 634043 (Shell Eco-marathon Battery Electric)
    // 48 V nominal, 3610 rpm nominal (3970 rpm no-load), 5.48 A nominal,
    // 627 mNm nominal, 8 pole pairs, 0.236 ohm / 0.424 mH phase-to-phase
    // (0.118 ohm / 0.212 mH per phase), Kt = 115 mNm/A, 5000 rpm max.
#define NOMINAL_SPEED_RPM          3610   // Nominal speed (RPM); no-load = 3970
#define MINIMUM_SPEED_RPM          150    // Minimum stable running speed (RPM)
#define POLEPAIRS                  8      // Maxon EC 48 V: 8 pole pairs

/* Normalized motor parameters for the sensorless estimator (estim.c).
 * These are only used when HALL_OFFSET_DETERMINATION is enabled.
 * NORM_RS = 0.118 ohm per phase (0.236 ohm phase-to-phase) in Q15
 * (1 ohm base); regenerate the Ls/Kfi terms in motorBench for the
 * Maxon if the sensorless estimator is used. */
#define NORM_LSDTBASE   15000
#define NORM_RS         3867
#define NORM_INVKFIBASE 14000
#define NORM_DELTAT     1790

#define D_ILIMIT_HS     800
#define D_ILIMIT_LS     5000

#elif defined(MOTOR_2KW)
    //2KW MOTOR Spescs
#define NOMINAL_SPEED_RPM          4000
#define MINIMUM_SPEED_RPM          300   
#define POLEPAIRS                  9
#define NORM_LSDTBASE   23515
#define NORM_RS         799
#define NORM_INVKFIBASE 15599
#define NORM_DELTAT     1790
#define D_ILIMIT_HS 1229
#define D_ILIMIT_LS 8651

#else
    //1KW MOTOR Specs
#define NOMINAL_SPEED_RPM          4000
#define MINIMUM_SPEED_RPM          300   
#define POLEPAIRS                  3

#define NORM_LSDTBASE   65014
#define NORM_RS         4024
#define NORM_INVKFIBASE 10476
#define NORM_DELTAT     1790
#define D_ILIMIT_HS 415
#define D_ILIMIT_LS 2884

#endif


/* Q15 Hall speed scale: measuredSpeed = 0.8425 * rpm * POLEPAIRS.
 * Calibrated from the original 32000 Q15 ~= 3800 rpm at 10 pole pairs.
 * Verify against a tachometer via the RPM telemetry field. */
#define MOTOR_SPEED_Q15_PER_RPM ((8425L * POLEPAIRS) / 100)
#define MCAPP_RPM_TO_SPEED_Q15(rpm) ((int16_t)(((int32_t)(rpm) * MOTOR_SPEED_Q15_PER_RPM) / 100))
#define MCAPP_SPEED_Q15_TO_RPM(s)   (((int32_t)(s) * 100) / MOTOR_SPEED_Q15_PER_RPM)


/* Filters constants definitions  */
/* BEMF filter for d-q components @ low speeds */
#define KFILTER_ESDQ 1200
/* BEMF filter for d-q components @ high speed - Flux Weakening case */
#define KFILTER_ESDQ_FW 164
/* Estimated speed filter constant */
#define KFILTER_VELESTIM 2*374


//******************************************************************************
//******************************************************************************
//Section: Current Calc, Acceleration and Deceleration
//******************************************************************************
/* The NORM_CURRENT_CONST value is calculated from Imax/32767 */
#define NORM_CURRENT_CONST     0.003967
/* current transformation macro, used below */
#define NORM_CURRENT(current_real) (Q15(current_real/NORM_CURRENT_CONST/32768))

//TORQUE SPECIFICATIONS

#ifdef MOTOR_250W
    /* Maxon EC 48 V (634043): nominal current 5.48 A (max continuous).
     * NORM_CURRENT(5.48) is a PEAK phase-current cap: the Park transform
     * produces the peak of the sinusoidal phase current. Maxon's 5.48 A
     * is a DC/terminal current; the peak equivalent for the same nominal
     * torque is about 6.0 A. A phase clamp meter therefore reads about
     * 3.9 A RMS at this cap.
     * See docs/THROTTLE_CALIBRATION_AND_TELEMETRY.md. */
#define MAX_TORQUE_CURRENT    NORM_CURRENT (5.48)
#define MIN_TORQUE_CURRENT    NORM_CURRENT (0.0)
#elif defined(MOTOR_2KW)
    /*Maximum Current for Torque Mode*/
#define MAX_TORQUE_CURRENT    NORM_CURRENT (10.0)
#define MIN_TORQUE_CURRENT    NORM_CURRENT (5.0)
#else
    /*Maximum Current for Torque Mode*/
#define MAX_TORQUE_CURRENT    NORM_CURRENT (30.0)
#define MIN_TORQUE_CURRENT    NORM_CURRENT (15.0)
#endif





/* The Speed Control Loop Executes every  SPEEDREFRAMP_COUNT */
#define SPEEDREFRAMP_COUNT   1//2
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************

//************** PI Coefficients ************
//// Current loop
// phase margin = 80 deg
// PI phase at crossover = 10.000 deg
// crossover frequency = 5.495 k rad/s (874.512 Hz)
/* Current loop proportional gain */



//PID COEFFICIENTS for Motor Current
//3100
//130
/* NOTE: these current-loop gains were tuned for the previous motor.
 * The Maxon EC 48 V (634043) has 0.118 ohm / 0.212 mH per phase
 * (0.236 ohm / 0.424 mH phase-to-phase); verify/retune KIP/KII
 * (or rerun motorBench) for this motor before relying on the
 * current-loop bandwidth. */
#ifdef MOTOR_2KW
#define KIP                                    2000//3100//16000//6000//16000//10000//7000//15000//14000//13000//12000//11000//10000//9000//8000//7500//4000//16000//4000//7500//7000//6500//6000//5500//5000//2000//3000//4000//28000//24000//20000//16000//12000//8000//4000//(James' tune)7500//15000//5267     
#define KIP_Q                                  15//17//16//20//15
/* Current loop integral gain */
#define KII                                   2//15//0//15//800//496     
#define KII_Q                                  15
#elif defined(MOTOR_250W)
/* Current-loop gains scaled for the Maxon EC 48 V (634043).
 *
 * The stock 16320 / 500 tune was made for the 1 kW motor, whose
 * NORM_LSDTBASE is 65014.  The Maxon's is 15000, i.e. its inductance is
 * 15000/65014 = 23.07 % of that motor's.  A PI current loop needs
 *     Kp proportional to L      -> 16320 * 0.2307 = 3765
 *     Ki proportional to R      ->  500 * (3867/4024) = 480
 * The old 12240 / 375 had 3.25x the proportional gain called for by the
 * inductance scaling, but a slightly lower integral gain.  Reducing Kp
 * did not remove the ~1.2 kHz tone, so that tone is a disturbance
 * (deadtime / back-EMF harmonic) rather than current-loop instability.
 * The PWM deadtime is now 500 ns in board_service.c, not the 2 us that
 * the MCC-generated pwm.c programs.
 * Verify with an X2C step response; Kp is the first knob to reduce if a
 * loop oscillation appears. */
#define KIP                                    3765
#define KIP_Q                                  15
#define KII                                    480
#define KII_Q                                  15
#else
#define KIP                                  16320//16500//17000//15000//16000//5267     
#define KIP_Q                                  15
/* Current loop integral gain */
#define KII                                   500//496     
#define KII_Q                                  15
#endif

/* ---------------------------------------------------------------------------
 * Single current-loop gain set for the Maxon EC 48 V.
 *
 * The soft-start gains (50 % Kp / 50 % Ki) are defined in motor_drive.c as
 * TUNE_SOFT_KP/TUNE_SOFT_KI. There is no rider-selected tune.
 * --------------------------------------------------------------------------- */





//// Velocity loop
// phase margin = 70 deg
// PI phase at crossover = 10.000 deg
// crossover frequency = 18.372 rad/s (2.924 Hz)
/* Velocity loop proportional gain */
#define KWP                                 6114//20000//30000//6114 
#define KWP_Q                                15
/* Velocity loop integral gain */
#define KWI                                  15//25//15
#define KWI_Q                                15


// D-axis current control loop coefficients
#define     DKP        KIP
#define     DKI        KII
#define     DKC        Q15(0.99999)
/* Limit for output line-to-line voltage of d-axis current controller, expressed as a fraction of DC link voltage */
#define MCAF_CURRENT_CTRL_D_OUT_LIMIT       18919      // Q15(  0.57736) =   +1.00002 line-to-line =   +1.00000 line-to-line + 0.0020%
#define     DKNP       (15-KIP_Q)
#define     DKNI       (15-KII_Q)

// Q-axis current control loop coefficients
#define     QKP        KIP
#define     QKI        KII
#define     QKC        Q15(0.99999)
/* Limit for output line-to-line voltage of q-axis current controller, expressed as a fraction of DC link voltage */
#define MCAF_CURRENT_CTRL_Q_OUT_LIMIT       32000//21756      // Q15(  0.66394) =   +1.14998 line-to-line =   +1.15000 line-to-line - 0.0019%
#define     QKNP       (15-KIP_Q)
#define     QKNI       (15-KII_Q)

// Velocity control loop coefficients
#define     WKP        KWP
#define     WKI        KWI
#define     WKC        Q15(0.0)
#define     MCAF_VELOCITY_CTRL_IQ_OUT_LIMIT  CURRENT_MAXIMUM_COMMAND   // see sat_PI_params.h for definition
#define     WKNP       (15-KWP_Q)
#define     WKNI       (15-KWI_Q)

/* Limits for magnitude and magnitude squared (Vd^2 + Vq^2)
 * as output of PI current controllers, in terms of line-neutral voltage */
 /* Limit for output magnitude of current controllers which triggers voltage saturation, expressed as a fraction of DC link voltage */
#define MCAF_CURRENT_CTRL_DQ_MAGNITUDE_LIMIT      18919      // Q15(  0.57735) = full linear SVPWM line-neutral voltage

/* Conservative high-speed field weakening for the Maxon EC-i 52.
 * Enable only above the nominal-speed region and while torque is requested. */
#define FIELD_WEAKENING_START_RPM                 4000
#define FIELD_WEAKENING_FULL_RPM                  4500
#define FIELD_WEAKENING_CURRENT_A                  1.0
#define FIELD_WEAKENING_MIN_IQ_A                   0.5
#define FIELD_WEAKENING_CURRENT_COUNTS             NORM_CURRENT(FIELD_WEAKENING_CURRENT_A)
#define FIELD_WEAKENING_MIN_IQ_COUNTS              NORM_CURRENT(FIELD_WEAKENING_MIN_IQ_A)

/* binary point for reciprocal voltages */
#define MCAF_RVDC_Q                            12
/* minimum DC link voltage for which we calculate reciprocals */
#define MCAF_RVDC_MIN_VDC                    4096      // Q15(  0.12500) =   +8.91250 V           =   +8.91277 V           - 0.0031%
/* reciprocal of minimum DC link voltage */
#define MCAF_RVDC_MIN                       32767      // Q12(  7.99976) = +112.19854 m1/V        = +112.19854 m1/V        + 0.0000%

#define MCAF_DEAD_TIME_COMPENSATION_VOLTAGE_DELAY          0

#define MCAF_RECIPROCAL_CURRENT_NUMERATOR (1<<11) // 1.0 Q11
// ***************************************************************************
// ***************************************************************************
// Section: Enums, Structures
// ***************************************************************************
typedef struct {
    uint16_t FromFullElectric;
    uint16_t FromPAS1;
    uint16_t FromPAS2;
    uint16_t FromPAS3;
    uint16_t FromPAS4;
    uint16_t FromPAS5;
} MODE_INDICATOR_T;

/**
 * Velocity control data
 */
typedef struct tagMCAPP_VELOCITY_CONTROL_DATA
{
    int16_t velocityCmdApi;         /** Reference velocity received from MCAPI */
    int16_t velocityCmd;            /** Reference velocity */
    int16_t velocityCmdRateLimited; /** Reference velocity after rate limiting */
    int16_t velocityCmdGain;      /** Reference velocity gain */
    int16_t slewRateLimit1;       /** rate limit relative to sensed velocity */
    int16_t slewRateLimitAccel;   /** acceleration slew rate limit */
    int16_t slewRateLimitDecel;   /** deceleration slew rate limit */
} MCAPP_VELOCITY_CONTROL_DATA;

typedef struct {
    uint16_t changeDirection;
    uint16_t state;
    uint16_t mode;
    int16_t  runDirection;
    uint16_t runMotor;


    int16_t  measuredSpeed;
    int16_t  measuredSpeedRPM;

    int16_t rVdc;

    uint16_t sector;
    uint16_t dutyCycle;
    uint16_t pwmPeriod;

    uint16_t pedelecSampleCounter;
    uint16_t pedelecSignalCalc;
    uint32_t pedelecSignalStateVar;
    uint16_t fromStartPedelec;
    uint16_t testCmpFlag;
    uint16_t fromOff, fromOn;

    uint32_t timerValue;
    uint32_t timerMonitor;
    MODE_INDICATOR_T modeFlags;

    uint32_t timerValueHall;

    int16_t targetReferenceValue;
    uint16_t throttleSampleCounter;
    int16_t  throttleScaledValue;

    /* FOC diagnostics, sampled by the UART telemetry.
     * Currents are in the same counts as NORM_CURRENT (0.003967 A/count).
     * Voltages are Q15 of the 116.05 V ADC full-scale bus voltage. */
    int16_t  idMeasured;
    int16_t  iqMeasured;
    int16_t  iqReference;
    int16_t  vqCommand;
    int16_t  vqLimit;
    uint16_t satState;
} MCAPP_DATA_T;


typedef enum {
    MCAPP_RESTART = 0,
    MCAPP_STOPPED = 1,
    MCAPP_STARTING = 2,
    MCAPP_RUNNING = 3,
    MCAPP_STOPPING = 4,
    MCAPP_FAULT = 5,
} MCAPP_STATES_T;

typedef enum {
    MCAPP_OFF = 0,
    MCAPP_FULL_ELECTRIC = 1,
    MCAPP_PAS1 = 2,
    MCAPP_PAS2 = 3,
    MCAPP_PAS3 = 4,
    MCAPP_PAS4 = 5,
    MCAPP_PAS5 = 6,
}MCAPP_MODE_T;

extern  MCAPP_DATA_T mcappData;

//extern MCAPP_MEASURE_AVG_T avgTargetRefValue; 

// ***************************************************************************
// ***************************************************************************
// Section: Interface Routines
// ***************************************************************************          
extern void ADC_ISR(void);
extern void PEDELEC_Signal_Detect(void);
extern void ResetParmeters(void);
extern void PWM_ISR();
extern void MCAPP_StateMachineInit(void);
extern void InitControlParameters(void);
extern void initControlLoopState(void);
extern void PEDELEC_Sensor_Handle(void);
extern void PEDELEC_Timer_Monitor(void);
// ***************************************************************************
#endif	/* USERPARAMS_H */

