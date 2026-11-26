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
//</editor-fold>

void WatchdogResetHandle(void);
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
    mcappData.mode = MCAPP_FULL_ELECTRIC;
    
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
    }
    return 1; 
}

void WatchdogResetHandle(void){
    if(_WDTO == 1){
        _WDTO = 0;
    }
}
/**
 End of File
*/  