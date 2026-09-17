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

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MCAPP_WATCHDOG_H
#define	MCAPP_WATCHDOG_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "../mcc_generated_files/watchdog.h"

#define MCAPP_WATCHDOG_MAINLOOP_TIMEOUT  10000

typedef struct tagWatchdog
{
    uint16_t isrCount; /** count of ISRs */
} MCAPP_WATCHDOG_T;

inline static void MCAPP_WatchdogManageMainLoop(volatile MCAPP_WATCHDOG_T *pwatchdog)
{
    pwatchdog->isrCount = 0;
}

inline static void MCAPP_WatchdogManageIsr(volatile MCAPP_WATCHDOG_T *pwatchdog)
{
    if (pwatchdog->isrCount < MCAPP_WATCHDOG_MAINLOOP_TIMEOUT)
    {
        ++pwatchdog->isrCount;
        WATCHDOG_TimerClear();
    }
}

extern volatile MCAPP_WATCHDOG_T pwatchdog;

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* MCAPP_WATCHDOG_H */

