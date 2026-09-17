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
 * File:   fault.h
 * Author: a20687
 * Comments:
 * Revision history: v1.00
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FAULT_H
#define	FAULT_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "../mcc_generated_files/pin_manager.h"

//Vbus Monitoring - Configured for 54V Nominal (Shell Eco-marathon Battery Electric)
#define BUS_VOLTAGE          54      //54V Nominal battery supply
#define OVERVOLTAGE         (uint16_t)((float)1.16*(float)BUS_VOLTAGE) // ~62.6V Overvoltage Cutoff
#define UNDERVOLTAGE        (uint16_t)((float)0.80*(float)BUS_VOLTAGE) // ~43.2V Undervoltage Cutoff
#define R1                  (uint32_t) 82000 //Ohm, based on the Schematics
#define R2                  (uint16_t) 2400 //Ohm, based on the Schematics

//Fault Detection Parameters
#define OVERVOLTAGE_LIMITER    (uint16_t)(((((float)OVERVOLTAGE*(float)R2)/((float)R1+(float)R2))*(float)4095)/(float)3.3)
#define UNDERVOLTAGE_LIMITER   (uint16_t)(((((float)UNDERVOLTAGE*(float)R2)/((float)R1+(float)R2))*(float)4095)/(float)3.3)

#define OVERCURRENT_COUNTER    10000 
#define VOLTAGE_COUNTER        20000
#define OVERTEMP_MOSFET_115    2047
#define OVERTEMP_COUNTER       20
#define UNDERVOLTAGE_INDICATOR 200
#define FIFTY_ms               1000

typedef struct 
{
    uint16_t counter;
    uint16_t indicator;
    uint16_t monitor;    
    uint16_t measure;
}FAULT_DATA_T;

typedef struct 
{
    int8_t OverCurrentA;
    int8_t OverCurrentB;
    int8_t OverCurrentC;    
    int8_t OverTemperature;
    int8_t OverVoltage;
    int8_t UnderVoltage;
}FAULT_FLAGS_T;


void UndervoltageDetect(void);
void OvervoltageDetect(void);
void OvertemperatureDetectMOSFET(void);

extern volatile FAULT_DATA_T faultUndervoltage;
extern volatile FAULT_DATA_T faultOvervoltage;
extern volatile FAULT_DATA_T faultOverTempMOSFET;
extern volatile FAULT_DATA_T faultOverCurrentPhaseA;
extern volatile FAULT_DATA_T faultOverCurrentPhaseB;
extern volatile FAULT_DATA_T faultOverCurrentPhaseC;
extern volatile FAULT_FLAGS_T faultFlags;

inline static void FaultFlagsReset(void){
    faultUndervoltage.counter = 0;
    faultOverCurrentPhaseA.counter = 0;
    faultOverCurrentPhaseB.counter = 0;
    faultOverCurrentPhaseC.counter = 0;
    faultOverTempMOSFET.counter = 0;
    
    faultFlags.OverCurrentA = 0;
    faultFlags.OverCurrentB = 0;
    faultFlags.OverCurrentC = 0;
    faultFlags.OverTemperature = 0;
    faultFlags.OverVoltage = 0;
    faultFlags.UnderVoltage = 0;
    
    IO_FAULT_SetLow();
}

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* FAULT_H */

