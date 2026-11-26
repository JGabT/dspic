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

#include "board_service.h"
#include "../src/userparms.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/pwm.h"
#include "../src/control.h"
#include "../src/hall.h"
#include "../src/fault.h"

#define BUTTON_ASSIST_LEVEL_DEC   IO_SW1_GetValue()
#define BUTTON_ASSIST_LEVEL_INC   IO_SW0_GetValue()

BUTTON_T buttonAssistLevelDec;
BUTTON_T buttonAssistLevelInc;

volatile uint16_t boardServiceISRCounter = 0;
#if TELEMETRY_UART_ENABLE
volatile uint16_t telemetryISRCounter = 0;
#endif
void DisablePWMOutputsInverterA(void);
void EnablePWMOutputsInverterA(void);
void BoardServiceInit(void);
void BoardServiceStepIsr(void);
void BoardService(void);
bool IsPressed_Button1(void);
void PWMDutyCycleSet(MC_DUTYCYCLEOUT_T *);
void PWMIdenticalDutyCycleSet(int16_t dutyCycle);
void pwmDutyCycleLimitCheck(MC_DUTYCYCLEOUT_T *,uint16_t,uint16_t);
static void ButtonGroupInitialize(void);
static void ButtonScan(BUTTON_T * ,bool);

bool IsPressed_Button1(void)
{
    if (buttonAssistLevelDec.status)
    {
        buttonAssistLevelDec.status = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool IsPressed_Button2(void)
{
    if (buttonAssistLevelInc.status)
    {
        buttonAssistLevelInc.status = false;
        return true;
    }
    else
    {
        return false;
    }
}
void BoardServiceStepIsr(void)
{
    if (boardServiceISRCounter <  BOARD_SERVICE_TICK_COUNT)
    {
        boardServiceISRCounter += 1;
    }

#if TELEMETRY_UART_ENABLE
    if (telemetryISRCounter < TELEMETRY_ISR_TICKS)
    {
        telemetryISRCounter += 1;
    }
#endif
}
#include "../mcc_generated_files/uart1.h"
#include "measure.h"

#if TELEMETRY_UART_ENABLE

#define TELEMETRY_TX_BUFFER_SIZE 224

static char    telemetryTxBuffer[TELEMETRY_TX_BUFFER_SIZE];
static uint8_t telemetryTxLength = 0;
static uint8_t telemetryTxIndex = 0;

static void Telemetry_AppendChar(char c)
{
    if (telemetryTxLength < TELEMETRY_TX_BUFFER_SIZE)
    {
        telemetryTxBuffer[telemetryTxLength++] = c;
    }
}

static void Telemetry_AppendString(const char *s)
{
    while (*s)
    {
        Telemetry_AppendChar(*s++);
    }
}

static void Telemetry_AppendInt(int32_t val)
{
    char buf[12];
    int idx = 0;

    if (val < 0)
    {
        Telemetry_AppendChar('-');
        val = -val;
    }
    if (val == 0)
    {
        Telemetry_AppendChar('0');
        return;
    }
    while (val > 0 && idx < 11)
    {
        buf[idx++] = (char)('0' + (val % 10));
        val /= 10;
    }
    while (idx > 0)
    {
        Telemetry_AppendChar(buf[--idx]);
    }
}

static void BoardService_SendTelemetry(void)
{
    // 10 Hz text telemetry, built into a buffer and sent without blocking.
    // Example:
    // POT:15230 POT_mV:1533 THR%:100 RUN:1 STATE:3 RPM:1500 DIR:1 FLT:0
    // VBUS_mV:54008 IQ_mA:5478 ID_mA:12 IQR_mA:5478 VQ_mV:3270 VMAX_mV:29606 SAT:0
    const int32_t pot    = measureInputs.potValue;
    const int32_t potMv  = (pot * 3300) >> 15;
    const int32_t thrPct = ((int32_t)mcappData.throttleScaledValue * 100) >> 15;
    const int32_t speedAbs = (mcappData.measuredSpeed < 0)
                           ? -(int32_t)mcappData.measuredSpeed
                           : (int32_t)mcappData.measuredSpeed;
    const int32_t rpm = MCAPP_SPEED_Q15_TO_RPM(speedAbs);
    const int32_t faults = ((int32_t)faultFlags.UnderVoltage    << 0)
                         | ((int32_t)faultFlags.OverVoltage     << 1)
                         | ((int32_t)faultFlags.OverTemperature << 2)
                         | ((int32_t)faultFlags.OverCurrentA    << 3)
                         | ((int32_t)faultFlags.OverCurrentB    << 4)
                         | ((int32_t)faultFlags.OverCurrentC    << 5);

    /* Bus voltage is Q15 of the 116.05 V ADC full scale
     * (3.3 V * (82k + 2.4k) / 2.4k).  116050/32768 = 3.5416 mV/count. */
    const int32_t vbusMv = ((int32_t)measureInputs.dcBusVoltage * 3627) >> 10;
    /* 1 current count = 0.003967 A = 3.967 mA. */
    const int32_t iqMa   = ((int32_t)mcappData.iqMeasured  * 3967) / 1000;
    const int32_t idMa   = ((int32_t)mcappData.idMeasured  * 3967) / 1000;
    const int32_t iqrMa  = ((int32_t)mcappData.iqReference * 3967) / 1000;
    /* Voltage commands are Q15 of the same 116.05 V base. */
    const int32_t vqMv   = ((int32_t)mcappData.vqCommand * 3627) / 1024;
    const int32_t vmaxMv = ((int32_t)mcappData.vqLimit   * 3627) / 1024;

    telemetryTxLength = 0;
    telemetryTxIndex  = 0;

    Telemetry_AppendString("POT:");
    Telemetry_AppendInt(pot);
    Telemetry_AppendString(" POT_mV:");
    Telemetry_AppendInt(potMv);
    Telemetry_AppendString(" THR%:");
    Telemetry_AppendInt(thrPct);
    Telemetry_AppendString(" RUN:");
    Telemetry_AppendInt(mcappData.runMotor);
    Telemetry_AppendString(" STATE:");
    Telemetry_AppendInt((int32_t)mcappData.state);
    Telemetry_AppendString(" RPM:");
    Telemetry_AppendInt(rpm);
    Telemetry_AppendString(" DIR:");
    Telemetry_AppendInt(mcappData.runDirection);
    Telemetry_AppendString(" FLT:");
    Telemetry_AppendInt(faults);
    Telemetry_AppendString(" VBUS_mV:");
    Telemetry_AppendInt(vbusMv);
    Telemetry_AppendString(" IQ_mA:");
    Telemetry_AppendInt(iqMa);
    Telemetry_AppendString(" ID_mA:");
    Telemetry_AppendInt(idMa);
    Telemetry_AppendString(" IQR_mA:");
    Telemetry_AppendInt(iqrMa);
    Telemetry_AppendString(" VQ_mV:");
    Telemetry_AppendInt(vqMv);
    Telemetry_AppendString(" VMAX_mV:");
    Telemetry_AppendInt(vmaxMv);
    Telemetry_AppendString(" SAT:");
    Telemetry_AppendInt((int32_t)mcappData.satState);
    Telemetry_AppendString("\r\n");
}

/* Called every main-loop iteration. Sends only while the UART1 TX
 * buffer has room, so it never stalls the main loop. */
static void Telemetry_Task(void)
{
    while ((telemetryTxIndex < telemetryTxLength) && UART1_IsTxReady())
    {
        UART1_Write((uint8_t)telemetryTxBuffer[telemetryTxIndex++]);
    }
}

#else
#define Telemetry_Task()    ((void)0)
#endif

void BoardService(void)
{
    if (boardServiceISRCounter ==  BOARD_SERVICE_TICK_COUNT)
    {
        ButtonScan(&buttonAssistLevelDec, BUTTON_ASSIST_LEVEL_DEC);
        ButtonScan(&buttonAssistLevelInc, BUTTON_ASSIST_LEVEL_INC);

        boardServiceISRCounter = 0;
    }

#if TELEMETRY_UART_ENABLE
    /* Start a new line every TELEMETRY_PERIOD_MS, but only when the
     * previous line has finished transmitting. */
    if ((telemetryISRCounter >= TELEMETRY_ISR_TICKS) &&
        (telemetryTxIndex >= telemetryTxLength))
    {
        telemetryISRCounter = 0;
        BoardService_SendTelemetry();
    }
    Telemetry_Task();
#endif
}

void BoardServiceInit(void)
{
    ButtonGroupInitialize();
    boardServiceISRCounter = BOARD_SERVICE_TICK_COUNT;

    /* The MCC-generated PWM_Initialize() programs PGxDTL/PGxDTH to 200
     * counts (2 us at 100 MHz).  This board uses MIC4604 gate drivers
     * (39 ns propagation delay, no shoot-through protection) and only
     * needs the 500 ns defined by DEADTIMESEC.  The 2 us deadtime adds a
     * large volt-second error at low current, producing 5th/7th harmonic
     * phase current and a 6th-harmonic torque buzz.  Re-apply the board
     * value here so opening MCC cannot silently restore the long
     * deadtime. */
    PWM_DeadTimeSet(PWM_GENERATOR_1, DDEADTIME);
    PWM_DeadTimeSet(PWM_GENERATOR_2, DDEADTIME);
    PWM_DeadTimeSet(PWM_GENERATOR_3, DDEADTIME);

#if TELEMETRY_UART_ENABLE
    telemetryISRCounter = TELEMETRY_ISR_TICKS;
#endif
}
void ButtonScan(BUTTON_T *pButton,bool button) 
{
    if (button == true) 
    {
        if (pButton->debounceCount < BUTTON_DEBOUNCE_COUNT) 
        {
            pButton->debounceCount++;
            pButton->state = BUTTON_DEBOUNCE;
        }
    } 
    else 
    {
        if (pButton->debounceCount < BUTTON_DEBOUNCE_COUNT) 
        {
            pButton->state = BUTTON_NOT_PRESSED;
        } 
        else 
        {
            pButton->state = BUTTON_PRESSED;
            pButton->status = true;
        }
        pButton->debounceCount = 0;
    }
}
void ButtonGroupInitialize(void)
{
    buttonAssistLevelDec.state = BUTTON_NOT_PRESSED;
    buttonAssistLevelDec.debounceCount = 0;
    buttonAssistLevelDec.state = false;
    
    buttonAssistLevelInc.state = BUTTON_NOT_PRESSED;
    buttonAssistLevelInc.debounceCount = 0;
    buttonAssistLevelInc.state = false;
}

/**
 * Disable the PWM channels assigned for Inverter #A by overriding them to low state.
 * @example
 * <code>
 * DisablePWMOutputsInverterA();
 * </code>
 */
void DisablePWMOutputsInverterA(void)
{
    /** Set Override Data on all PWM outputs */
    PWM_OverrideDataSet(PWM_GENERATOR_1,0);
    PWM_OverrideDataSet(PWM_GENERATOR_2,0);
    PWM_OverrideDataSet(PWM_GENERATOR_3,0);
    
    PWM_OverrideHighEnable(PWM_GENERATOR_1);
    PWM_OverrideHighEnable(PWM_GENERATOR_2);
    PWM_OverrideHighEnable(PWM_GENERATOR_3);
    
    PWM_OverrideLowEnable(PWM_GENERATOR_1);
    PWM_OverrideLowEnable(PWM_GENERATOR_2);
    PWM_OverrideLowEnable(PWM_GENERATOR_3); 
}

/**
 * Enable the PWM channels assigned for Inverter #A by removing Override.
 * @example
 * <code>
 * EnablePWMOutputsInverterA();
 * </code>
 */
void EnablePWMOutputsInverterA(void)
{    
    PWM_OverrideHighDisable(PWM_GENERATOR_1);
    PWM_OverrideHighDisable(PWM_GENERATOR_2);
    PWM_OverrideHighDisable(PWM_GENERATOR_3);  
    
    PWM_OverrideLowDisable(PWM_GENERATOR_1);
    PWM_OverrideLowDisable(PWM_GENERATOR_2);
    PWM_OverrideLowDisable(PWM_GENERATOR_3);  
}


void PWMDutyCycleSet(MC_DUTYCYCLEOUT_T *pPwmDutycycle)
{
    /* Keep every pulse at least one full deadtime wide.  The previous
     * half-deadtime limit allowed pulses shorter than the gate-driver
     * deadtime, which the bridge cannot reproduce faithfully. */
    pwmDutyCycleLimitCheck(pPwmDutycycle,DDEADTIME,(LOOPINTCY - DDEADTIME));
    
    MOTOR_PHASE_C_DC = pPwmDutycycle->dutycycle3;
    MOTOR_PHASE_B_DC = pPwmDutycycle->dutycycle2;
    MOTOR_PHASE_A_DC = pPwmDutycycle->dutycycle1;
}

void PWMIdenticalDutyCycleSet(int16_t dutyCycle)
{
    pwmDutycycle.dutycycle1 = dutyCycle;
    pwmDutycycle.dutycycle2 = dutyCycle;
    pwmDutycycle.dutycycle3 = dutyCycle;
    
    PWMDutyCycleSet(&pwmDutycycle);
}

void pwmDutyCycleLimitCheck (MC_DUTYCYCLEOUT_T *pPwmDutycycle,uint16_t min,uint16_t max)
{
    if (pPwmDutycycle->dutycycle1 < min)
    {
        pPwmDutycycle->dutycycle1 = min;
    }
    else if (pPwmDutycycle->dutycycle1 > max)
    {
        pPwmDutycycle->dutycycle1 = max;
    }
    
    if (pPwmDutycycle->dutycycle2 < min)
    {
        pPwmDutycycle->dutycycle2 = min;
    }
    else if (pPwmDutycycle->dutycycle2 > max)
    {
        pPwmDutycycle->dutycycle2 = max;
    }
    
    if (pPwmDutycycle->dutycycle3 < min)
    {
        pPwmDutycycle->dutycycle3 = min;
    }
    else if (pPwmDutycycle->dutycycle3 > max)
    {
        pPwmDutycycle->dutycycle3 = max;
    }
}

bool MCAPP_BootstrapChargeStepIsr(MCAPP_BOARD_DATA *pboard)
{
    bool returnState = false;
    
    switch(pboard->bootstrapState)
    {
        case MCBS_IDLE_START:
            /* Override high-side PWMx to LOW and set 
             * low-side PWMx to 0% duty cycle */
            DisablePWMOutputsInverterA();
            pboard->bootstrapDutycycle[0] = HAL_PARAM_PWM_PERIOD_COUNTS;
            pboard->bootstrapDutycycle[1] = HAL_PARAM_PWM_PERIOD_COUNTS;
            pboard->bootstrapDutycycle[2] = HAL_PARAM_PWM_PERIOD_COUNTS;
            HAL_PWM_UpperTransistorsOverride_Low();
            HAL_PWM_LowerTransistorsOverride_Disable();
            
            pboard->delayCount = MCAPP_BOARD_BOOTSTRAP_INITIAL_DELAY;
            pboard->bootstrapState = MCBS_WAIT_INITIAL;
            break;
            
        case MCBS_WAIT_INITIAL:
            /* Wait for a preset duration of time before starting to
             * charge the Phase-A bootstrap */
            if (pboard->delayCount == 0)
            {   
                pboard->bootstrapState = MCBS_PHASE_A_SETUP_CHARGING;
            }
            break;

        case MCBS_PHASE_A_SETUP_CHARGING:
            pboard->bootstrapDutycycle[0] = minimumDutyCycleForBootstrapCharging();
            pboard->delayCount = MCAPP_BOARD_BOOTSTRAP_PHASE_DELAY;
            pboard->bootstrapState = MCBS_PHASE_A_CHARGING;
            break;
            
        case MCBS_PHASE_A_CHARGING:
            /* Wait for a preset duration of time to let the bootstrap drive
             * charge the Phase-A bootstrap capacitor */
            if (pboard->delayCount == 0)
            {
                pboard->bootstrapState = MCBS_PHASE_B_SETUP_CHARGING;
            }
            break;
            
        case MCBS_PHASE_B_SETUP_CHARGING:
            pboard->bootstrapDutycycle[1] = minimumDutyCycleForBootstrapCharging();
            pboard->delayCount = MCAPP_BOARD_BOOTSTRAP_PHASE_DELAY;
            pboard->bootstrapState = MCBS_PHASE_B_CHARGING;
            break;
            
        case MCBS_PHASE_B_CHARGING:
            /* Wait for a preset duration of time to let the bootstrap drive
             * charge the Phase-B bootstrap capacitor */
            if (pboard->delayCount == 0)
            {
                pboard->bootstrapState = MCBS_PHASE_C_SETUP_CHARGING;
            }
            break;

        case MCBS_PHASE_C_SETUP_CHARGING:
            pboard->bootstrapDutycycle[2] = minimumDutyCycleForBootstrapCharging();
            pboard->delayCount = MCAPP_BOARD_BOOTSTRAP_PHASE_DELAY;
            pboard->bootstrapState = MCBS_PHASE_C_CHARGING;
            break;
            
        case MCBS_PHASE_C_CHARGING:
            /* Wait for a preset duration of time to let the bootstrap drive
             * charge the Phase-C bootstrap capacitor */
            if (pboard->delayCount == 0)
            {
                pboard->bootstrapState = MCBS_BOOTSTRAP_COMPLETE;
            }
            break;
            
        case MCBS_BOOTSTRAP_COMPLETE:
            /* Bootstrap sequence is complete, wait in this state */
            returnState = true;
            break;
    }
    HAL_PWM_DutyCycle_Set(pboard->bootstrapDutycycle);
    
    if (pboard->delayCount > 0)
    {
        pboard->delayCount--;
    }
    
    return returnState;
}

/*
 End of file
 */