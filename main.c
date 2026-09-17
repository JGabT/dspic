
/**
  Section: Included Files
*/
#include <xc.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/X2Cscope/X2Cscope.h"
#include "mcc_generated_files/pwm.h"
#include "hal/board_service.h"
#include "hal/measure.h"
#include "src/control.h"
#include "src/fault.h"
#include "src/userparms.h"
#include "src/hall.h"
#include "src/util.h"
#include "src/mcapp_watchdog.h"
#include "src/startup.h"
#include "mcc_generated_files/cmp3.h"
//</editor-fold>

#define HALF_SECOND  (2000000UL)

static bool throttleSafeStarted = false;

void FullElectricState(void);
void WatchdogResetHandle(void);

void FullElectric(void){
    if(uGF.bits.NewThrottleValue)
    { 
        uGF.bits.NewThrottleValue = 0;

        // Shell Eco-marathon Scrutineering Safety Requirement:
        // Zero-Throttle Startup Interlock (Dead-Man Protection)
        // Motor is strictly inhibited on power-on until throttle is confirmed at zero (< THROTTLE_LOW)
        if(!throttleSafeStarted)
        {
            if(measureInputs.potValue < THROTTLE_LOW)
            {
                throttleSafeStarted = true; // Armed only after verified in resting deadband
            }
            mcappData.runMotor = 0;
            return;
        }

        // True Hysteresis Window to prevent boundary chatter
        if(measureInputs.potValue > (THROTTLE_LOW + THROTTLE_HYSTERESIS))
        {
            mcappData.runMotor = 1;
        }
        else if(measureInputs.potValue < THROTTLE_LOW)
        {
            mcappData.runMotor = 0;
        }
    }
}
/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    BoardServiceInit();
    
    FaultFlagsReset();
	
	HAL_ADC_SetInterruptHandler(ADC_ISR);
    PWM_SetGenerator1InterruptHandler(PWM_ISR);
	
    PWM_Enable();
    MCAPP_StateMachineInit();
    
    WATCHDOG_TimerSoftwareEnable();
    WatchdogResetHandle();
    
    InitControlParameters();
    initControlLoopState();
    MCAPP_StartupTransitioningInit(&motorStartUpData);
    
    HAL_ADC_InterruptFlagClear(HAL_ADC_CHANNEL_USED_FOR_ISR);
    HAL_ADC_InterruptEnable(HAL_ADC_CHANNEL_USED_FOR_ISR);
        
    
    while (1)
    {
        
#ifdef X2CScope_ENABLE
        X2CScope_Communicate();
#endif
		//UI Service every 100mS. (20 ADC interrupts. 1ADC Interrupt/ PWM Cycle. PWM =20khz.)
        BoardService();
        
        mcappData.mode = MCAPP_FULL_ELECTRIC; 

        FullElectric();
    }
    return 1; 
}

void FullElectricState(void)
{
    switch(mcappData.mode){
        case MCAPP_OFF:
        {
            mcappData.runMotor = 0;
            
            if(mcappData.modeFlags.FromFullElectric){
                mcappData.modeFlags.FromFullElectric = 0;
                MCAPP_StateMachineInit();
                InitControlParameters();
                initControlLoopState();
                MCAPP_StartupTransitioningInit(&motorStartUpData);
            }
            break;
        }
        case MCAPP_FULL_ELECTRIC:
        {
            mcappData.modeFlags.FromFullElectric = 1;
            break;
        }

        default:
        {
            mcappData.mode = MCAPP_OFF;
            break;
        }
    }
}

void WatchdogResetHandle(void){
    if(_WDTO == 1){
        _WDTO = 0;
    }
}
/**
 End of File
*/