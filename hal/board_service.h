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
#ifndef __BOARD_SERVICE_H
#define __BOARD_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

#include <xc.h>
//#include "../src/motor_control_noinline.h"
#include "../mc_lib/motor_control_types.h"
#include "../mcc_generated_files/pwm.h"
#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif

#define MCAPP_BOARD_BOOTSTRAP_SEQUENCE_DELAY 2
#define MCAPP_BOARD_BOOTSTRAP_PHASE_DELAY    100
#define MCAPP_BOARD_BOOTSTRAP_INITIAL_DELAY  1
        
#define HAL_PARAM_PWM_PERIOD_COUNTS          5000 
#define HAL_PARAM_MIN_LOWER_DUTY_COUNTS      225         
/* Minimum duty cycle register value, as ratio */
#define HAL_PARAM_MIN_DUTY_Q15                950     
/* Maximum low-side duty cycle register value in counts */
#define HAL_PARAM_MAX_LOWER_DUTY_COUNTS       4855           
        
#define HAL_PARAM_MAX_DUTY_Q15                31293      
        
#define HAL_ADC_InterruptFlagClear      ADC1_IndividualChannelInterruptFlagClear
#define HAL_ADC_InterruptEnable         ADC1_IndividualChannelInterruptEnable        
#define HAL_ADC_InterruptDisable        ADC1_IndividualChannelInterruptDisable
#define HAL_ADC_SetInterruptHandler     ADC1_SetADC_IAInterruptHandler
#define HAL_ADC_CHANNEL_USED_FOR_ISR    ADC_IA
#define PEDELEC_Interrupt_Enable()      {IEC4bits.CMP3IE = 1;}
#define PEDELEC_Interrupt_Disable()     {IEC4bits.CMP3IE = 0;}        
        
#define FOSC    200000000UL         //200MHz
#define PWMFREQUENCY 20000          //20kHz
#define DEADTIMESEC     0.0000005   // Dead time in seconds - 500ns
#define DFCY            FOSC/2      // Instruction cycle frequency (Hz)

#define DDEADTIME       (unsigned int)(DEADTIMESEC*DFCY) // Dead time in dTcys
#define LOOPINTCY       ((DFCY/PWMFREQUENCY)-1)          // Basic loop period in units of Tcy        

#define LOOPTIME_SEC     0.000050    //Specify PWM Period in seconds, (1/ PWMFREQUENCY) 
        
// Specify bootstrap charging time in Seconds (mention at least 10mSecs)
#define BOOTSTRAP_CHARGING_TIME_SECS 0.01
// Calculate Bootstrap charging time in number of PWM Half Cycles
#define BOOTSTRAP_CHARGING_COUNTS (uint16_t)((BOOTSTRAP_CHARGING_TIME_SECS/LOOPTIME_SEC )*2)
        
//******************************************************************************
// *****************************************************************************
// Section: Inverter Phase Definitions
//******************************************************************************
#define MOTOR_PHASE_A_DC        PG1DC
#define MOTOR_PHASE_B_DC        PG2DC
#define MOTOR_PHASE_C_DC        PG3DC
/* Button Scanning State

  Description:
    This structure will host parameters required by Button scan routine.
 */
typedef enum tagBUTTON_STATE
{
    BUTTON_NOT_PRESSED = 0, // wait for button to be pressed
    BUTTON_PRESSED = 1, // button was pressed, check if short press / long press
    BUTTON_DEBOUNCE = 2
} BUTTON_STATE;
    
// *****************************************************************************
/* Button data type

  Description:
    This structure will host parameters required by Button scan routine.
 */
typedef struct
{
   BUTTON_STATE state;
   uint16_t debounceCount;
   bool logicState;
   bool status;
} BUTTON_T;

typedef enum tagMCAPP_BOOTSTRAP_STATE
{
    /** Initialization state */
    MCBS_IDLE_START = 0,
    /** WAIT state before starting the bootstrap charging sequence */
    MCBS_WAIT_INITIAL = 1,
    /** Set up state for PHASE A bootstrap charging */
    MCBS_PHASE_A_SETUP_CHARGING = 2,
    /** PHASE A bootstrap charging */
    MCBS_PHASE_A_CHARGING = 3,
    /** Set up state for PHASE B bootstrap charging */
    MCBS_PHASE_B_SETUP_CHARGING = 4,
    /** PHASE B bootstrap charging */
    MCBS_PHASE_B_CHARGING = 5,
    /** Set up state for PHASE C bootstrap charging */
    MCBS_PHASE_C_SETUP_CHARGING = 6,
    /** PHASE C bootstrap charging */
    MCBS_PHASE_C_CHARGING = 7,
    /** Bootstrap charging is complete */
    MCBS_BOOTSTRAP_COMPLETE = 8
} MCAPP_BOOTSTRAP_STATE;

typedef struct tagMCAPP_BOARD_DATA
{ 
    /** This flag determines which function to be called among 
     *  Board_Service() or Board_Configure() functions
     *  if equals to '0' - Board_Configure()
     *  if equals to '1' - Board_Service()
     */
    bool configComplete;
    /** Maintains runtime state of Board_Service() or Board_Configure() functions */
//    HAL_BOARD_STATUS runtimeState;
    /** This variable is used to pre-scale the execution of board service 
     * routines,and is incremented in MCAPP_BoardServiceStepIsr */ 
    volatile uint16_t isrCount;
    /** This enum variables track progress of bootstrap charging process*/
    MCAPP_BOOTSTRAP_STATE bootstrapState;
    /** The array stores bootstrap charging duty cycles of Phase A,B,and C */
    uint16_t bootstrapDutycycle[3];
    /** This delay variable is used by bootstrap charging function to define 
     * delay in various states of the function */
    uint16_t delayCount;   
} MCAPP_BOARD_DATA;

/** Button De-bounce in milli Seconds */
#define	BUTTON_DEBOUNCE_COUNT      30
/** The board service Tick is set as 1 millisecond - specify the count in terms 
    PWM ISR cycles (i.e. BOARD_SERVICE_TICK_COUNT = 1 milli Second / PWM period)*/
#define BOARD_SERVICE_TICK_COUNT   20
extern void DisablePWMOutputsInverterA(void);
extern void EnablePWMOutputsInverterA(void);
extern void BoardServiceInit(void);
extern void BoardServiceStepIsr(void);
extern void BoardService(void);
extern bool IsPressed_Button1(void);
extern bool IsPressed_Button2(void);
extern void PWMDutyCycleSet(MC_DUTYCYCLEOUT_T *);
extern void PWMIdenticalDutyCycleSet(int16_t dutyCycle);

inline static void HAL_PWM_UpperTransistorsOverride_Low(void)
{
    /* Set PWM override data to 0b00 */
    PWM_OverrideDataSet(PWM_GENERATOR_1,0);
    PWM_OverrideDataSet(PWM_GENERATOR_2,0);
    PWM_OverrideDataSet(PWM_GENERATOR_3,0);

    /* Enable PWM override */
    PWM_OverrideHighEnable(PWM_GENERATOR_1);
    PWM_OverrideHighEnable(PWM_GENERATOR_2);
    PWM_OverrideHighEnable(PWM_GENERATOR_3);
}

inline static void HAL_PWM_LowerTransistorsOverride_Disable(void)
{
    PWM_OverrideLowDisable(PWM_GENERATOR_1);
    PWM_OverrideLowDisable(PWM_GENERATOR_2);
    PWM_OverrideLowDisable(PWM_GENERATOR_3);
}

inline static uint16_t minimumDutyCycleForBootstrapCharging(void)
{
    return HAL_PARAM_PWM_PERIOD_COUNTS - HAL_PARAM_MIN_LOWER_DUTY_COUNTS;
}

inline static void HAL_PWM_DutyCycle_Set(const uint16_t *pdc)
{
    PWM_DutyCycleSet(PWM_GENERATOR_1,pdc[0]);
    PWM_DutyCycleSet(PWM_GENERATOR_2,pdc[1]);
    PWM_DutyCycleSet(PWM_GENERATOR_3,pdc[2]);
}

inline static void MCAPP_BootstrapChargeInit(MCAPP_BOARD_DATA *pboard)
{
    pboard->bootstrapDutycycle[0] = 0;
    pboard->bootstrapDutycycle[1] = 0;
    pboard->bootstrapDutycycle[2] = 0;
    pboard->delayCount = 0;
    pboard->bootstrapState = 0;
}

bool MCAPP_BootstrapChargeStepIsr(MCAPP_BOARD_DATA *pboard);


#ifdef __cplusplus
}
#endif

#endif /* __BOARD_SERVICE_H */
