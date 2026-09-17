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
#include "userparms.h"
#include "../mcc_generated_files/sccp1_tmr.h"
#include "../hal/measure.h"

#define ONE_PEDELEC_CYCLE 12

MCAPP_MEASURE_AVG_T avgTargetRefValue;

uint16_t cmpStatus;
uint16_t fromOff, fromOn;
int16_t thetaError;
uint16_t startingCounter;
        
void PEDELEC_Signal_Detect(void)
{
    cmpStatus = DAC3CONLbits.CMPSTAT;

    if(cmpStatus){
        if(fromOff){
            SCCP1_TMR_Stop();
            fromOff = 0;

            if(!mcappData.runMotor){
                mcappData.runMotor = 1;
                mcappData.timerValue = STARTING_TIMER_COUNT;
                mcappData.fromStartPedelec = 1;
                startingCounter = 0;
            }
            else if(startingCounter < ONE_PEDELEC_CYCLE){
                startingCounter++;
                mcappData.timerValue = STARTING_TIMER_COUNT;
            }
            else{
                mcappData.fromStartPedelec = 0;
                mcappData.timerValue = SCCP1_TMR_Counter32BitGet();
            }
            SCCP1_TMR_Counter32BitSet(0);
            SCCP1_TMR_Start();
        }
    }
    else
    {
        fromOff = 1;
    }
}


void MCAPP_PAS1_Sensor_Handle(void){
    int32_t temp;
    
    temp = (int32_t)(__builtin_mulss((int16_t)mcappData.targetReferenceValue,(int16_t)5));
    mcappData.targetReferenceValue = (int16_t)(__builtin_divsd((int32_t)temp,(int16_t)10));
}

void MCAPP_PAS2_Sensor_Handle(void){
    int32_t temp;
    
    temp = (int32_t)(__builtin_mulss((int16_t)mcappData.targetReferenceValue,(int16_t)6));
    mcappData.targetReferenceValue = (int16_t)(__builtin_divsd((int32_t)temp,(int16_t)10));
}

void MCAPP_PAS3_Sensor_Handle(void){
    int32_t temp;
    
    temp = (int32_t)(__builtin_mulss((int16_t)mcappData.targetReferenceValue,(int16_t)7));
    mcappData.targetReferenceValue = (int16_t)(__builtin_divsd((int32_t)temp,(int16_t)10));
}

void MCAPP_PAS4_Sensor_Handle(void){
    int32_t temp;
    
    temp = (int32_t)(__builtin_mulss((int16_t)mcappData.targetReferenceValue,(int16_t)8));
    mcappData.targetReferenceValue = (int16_t)(__builtin_divsd((int32_t)temp,(int16_t)10));
}

void MCAPP_PAS5_Sensor_Handle(void){
    if(mcappData.measuredSpeed > (MCAF_VELOCITY_NOMINAL - MINIMUM_SPEED_ELECTR)){
        mcappData.runMotor = 0;
    }
}

void PEDELEC_Sensor_Handle(void)
{  
    if(mcappData.timerValue == 0){
       mcappData.targetReferenceValue = 0;
    }
    else{
       mcappData.pedelecSignalCalc = (uint16_t)(__builtin_divud((uint32_t)mcappData.timerValue,(uint16_t)DECIMATION_FACTOR));
       mcappData.pedelecSignalCalc = (uint16_t)(__builtin_divud((uint32_t)PEDELEC_SIGNAL_CONST, (uint16_t)mcappData.pedelecSignalCalc));
       mcappData.pedelecSignalStateVar = (uint32_t)(__builtin_muluu((uint16_t)NORM_SIGNAL_CONST,(uint16_t)mcappData.pedelecSignalCalc));
       avgTargetRefValue.input = (uint16_t)(__builtin_divud((uint32_t)mcappData.pedelecSignalStateVar,(uint16_t)DECIMATION_FACTOR));

       if(mcappData.fromStartPedelec){
           mcappData.targetReferenceValue = avgTargetRefValue.input;
           MCAPP_MeasureAvg(&avgTargetRefValue);
       }
       else{
            mcappData.targetReferenceValue = MCAPP_MeasureAvg(&avgTargetRefValue);
       }

        switch (mcappData.mode) {
            case MCAPP_PAS1:
            {
                MCAPP_PAS1_Sensor_Handle();
                break;
            }

            case MCAPP_PAS2:
            {
                MCAPP_PAS2_Sensor_Handle();
                break;
            }

            case MCAPP_PAS3:
            {
                MCAPP_PAS3_Sensor_Handle();
                break;
            }

            case MCAPP_PAS4:
            {
                MCAPP_PAS4_Sensor_Handle();
                break;
            }

            case MCAPP_PAS5:
            {
                MCAPP_PAS5_Sensor_Handle();
                break;
            }
        }
        
        if(mcappData.targetReferenceValue < MIN_REF_VALUE){
           mcappData.targetReferenceValue = MIN_REF_VALUE;
        }
        else if(mcappData.targetReferenceValue > MAX_REF_VALUE){
           mcappData.targetReferenceValue = MAX_REF_VALUE;
        }
   }
}

void PEDELEC_Timer_Monitor(void)
{
    mcappData.timerMonitor = SCCP1_TMR_Counter32BitGet();
    
    if(mcappData.timerMonitor > PULSE_ABSCENCE_LIMIT){      
        SCCP1_TMR_Stop();
        SCCP1_TMR_Counter32BitSet(0);
        mcappData.runMotor = 0;
        mcappData.timerMonitor = 0;
    }
}