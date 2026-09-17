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

#define BUTTON_ASSIST_LEVEL_DEC   IO_SW1_GetValue()
#define BUTTON_ASSIST_LEVEL_INC   IO_SW0_GetValue()

BUTTON_T buttonAssistLevelDec;
BUTTON_T buttonAssistLevelInc;

uint16_t boardServiceISRCounter = 0;
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
}
#include "../mcc_generated_files/uart1.h"
#include "measure.h"

static void Telemetry_SendChar(char c)
{
    while(!UART1_IsTxReady());
    UART1_Write((uint8_t)c);
}

static void Telemetry_SendString(const char *s)
{
    while(*s)
    {
        Telemetry_SendChar(*s++);
    }
}

static void Telemetry_SendInt(int32_t val)
{
    char buf[12];
    int idx = 0;
    if(val < 0)
    {
        Telemetry_SendChar('-');
        val = -val;
    }
    if(val == 0)
    {
        Telemetry_SendChar('0');
        return;
    }
    while(val > 0 && idx < 11)
    {
        buf[idx++] = (char)('0' + (val % 10));
        val /= 10;
    }
    while(idx > 0)
    {
        Telemetry_SendChar(buf[--idx]);
    }
}

static void BoardService_SendTelemetry(void)
{
    // Fast, lightweight real-time telemetry stream @ 10Hz (115200 baud)
    // Example: POT:15230 V_mV:1533 THR%:46 RUN:1 RPM:1200
    int32_t pot = measureInputs.potValue;
    int32_t vmV = ((int32_t)pot * 3300) >> 15;
    int32_t thrPct = ((int32_t)mcappData.throttleScaledValue * 100) >> 15;

    Telemetry_SendString("POT:");
    Telemetry_SendInt(pot);
    Telemetry_SendString(" V_mV:");
    Telemetry_SendInt(vmV);
    Telemetry_SendString(" THR%:");
    Telemetry_SendInt(thrPct);
    Telemetry_SendString(" RUN:");
    Telemetry_SendInt(mcappData.runMotor);
    Telemetry_SendString(" STATE:");
    Telemetry_SendInt((int32_t)mcappData.state);
    Telemetry_SendString("\r\n");
}

void BoardService(void)
{
    if (boardServiceISRCounter ==  BOARD_SERVICE_TICK_COUNT)
    {

        ButtonScan(&buttonAssistLevelDec, BUTTON_ASSIST_LEVEL_DEC);
        ButtonScan(&buttonAssistLevelInc, BUTTON_ASSIST_LEVEL_INC);

        // Stream real-time telemetry to UART1 (RC8 TX / RC9 RX)
        BoardService_SendTelemetry();

        boardServiceISRCounter = 0;
    }
}

void BoardServiceInit(void)
{
    ButtonGroupInitialize();
    boardServiceISRCounter = BOARD_SERVICE_TICK_COUNT;
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
    pwmDutyCycleLimitCheck(pPwmDutycycle,(DDEADTIME>>1),(LOOPINTCY - (DDEADTIME>>1)));  
    
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