//<editor-fold defaultstate="collapsed" desc="Description">
/*
   * @Company
    Microchip Technology Inc.

  @File Name
    fault.c

  @Summary
    This file contains the fault detection routines for this application.
*/
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Disclaimer">
/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/
//</editor-fold>

#include "fault.h"
#include "userparms.h"
#include "../mcc_generated_files/adc1.h"
#include "../mcc_generated_files/cmp1.h"
#include "../mcc_generated_files/cmp2.h"
#include "../mcc_generated_files/cmp3.h"

volatile FAULT_DATA_T faultUndervoltage;
volatile FAULT_DATA_T faultOvervoltage;
volatile FAULT_DATA_T faultOverTempMOSFET;
volatile FAULT_DATA_T faultOverCurrentPhaseA;
volatile FAULT_DATA_T faultOverCurrentPhaseB;
volatile FAULT_DATA_T faultOverCurrentPhaseC;
volatile FAULT_FLAGS_T faultFlags;

void CMP1_CallBack(void)
{
    if (faultOverCurrentPhaseA.counter > OVERCURRENT_COUNTER)
    {
        mcappData.state = MCAPP_FAULT;
        faultFlags.OverCurrentA = 1;
    }
    faultOverCurrentPhaseA.counter++;
}

void CMP2_CallBack(void)
{
    if (faultOverCurrentPhaseC.counter > OVERCURRENT_COUNTER)
    {
        mcappData.state = MCAPP_FAULT;
        faultFlags.OverCurrentC = 1;
    }
    faultOverCurrentPhaseC.counter++;
}

void UndervoltageDetect(void)
{
    faultUndervoltage.monitor = (uint16_t)(faultUndervoltage.measure >> 4);
    
    if(faultUndervoltage.monitor < UNDERVOLTAGE_LIMITER)
    {
        faultUndervoltage.counter++;
        
        if(faultUndervoltage.counter > VOLTAGE_COUNTER)
        {
            mcappData.state = MCAPP_FAULT;
            faultFlags.UnderVoltage = 1;
        }
    }
}

void OvervoltageDetect(void)
{
    faultOvervoltage.monitor = (uint16_t)(faultOvervoltage.measure >> 4);
    
    if(faultOvervoltage.monitor > OVERVOLTAGE_LIMITER)
    {
        faultOvervoltage.counter++;
        
        if(faultOvervoltage.counter > VOLTAGE_COUNTER)
        {
            mcappData.state = MCAPP_FAULT;
            faultFlags.OverVoltage = 1;
        }
    }
}

void OvertemperatureDetectMOSFET(void)
{
    faultOverTempMOSFET.monitor = (faultOverTempMOSFET.measure >> 4);
    
    if(faultOverTempMOSFET.monitor > OVERTEMP_MOSFET_115)
    {
        faultOverTempMOSFET.counter++;
        
        if(faultOverTempMOSFET.counter > OVERTEMP_COUNTER)
        {
            mcappData.state = MCAPP_FAULT;
            faultFlags.OverTemperature = 1;
        }
    }
}
/*
 End of file
 */
