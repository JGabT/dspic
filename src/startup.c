/**
 * startup.c
 *
 * Module for startup and open loop to closed loop transition
 * 
 * Component: state machine
 */

/* *********************************************************************
 *
 * (c) 2017 Microchip Technology Inc. and its subsidiaries. You may use
 * this software and any derivatives exclusively with Microchip products.
 *
 * This software and any accompanying information is for suggestion only.
 * It does not modify Microchip's standard warranty for its products.
 * You agree that you are solely responsible for testing the software and
 * determining its suitability.  Microchip has no obligation to modify,
 * test, certify, or support the software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH
 * MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
 * APPLICATION.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL,
 * PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF
 * ANY KIND WHATSOEVER RELATED TO THE USE OF THIS SOFTWARE, THE
 * motorBench(TM) DEVELOPMENT SUITE TOOL, PARAMETERS AND GENERATED CODE,
 * HOWEVER CAUSED, BY END USERS, WHETHER MICROCHIP'S CUSTOMERS OR
 * CUSTOMER'S CUSTOMERS, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
 * CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
 * OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
 * SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
 * THESE TERMS.
 *
 * *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "startup.h"
#include "startup_params.h"
#include "util.h"

extern uint16_t getRotorInitialPosition(void);

void MCAPP_StartupTransitioningInit(MCAPP_MOTOR_STARTUP_DATA *pstartup)
{
    pstartup->alignTime = STARTUP_ALIGN_TIME;
    pstartup->rampupAngle = getRotorInitialPosition();
    pstartup->iAmplitude = MCAPP_STARTUP_CURRENT;
    pstartup->dt = STARTUP_DELTA_T_FACTOR;
}

#define MCAPP_STARTUP_RESET_CYCLES 1

void MCAPP_StartupTransitioningStep(MCAPP_MOTOR_STARTUP_DATA *pstartup,
        MC_DQ_T *idqCommand, int16_t direction)
{
    const MC_DQ_T idqcmd = *idqCommand;
    MC_DQ_T idqcmd_next = idqcmd;
    
    switch (pstartup->state)
    {
        case SSM_START:
            /* startup in reset: stay in this state until re-enabled */
            if (pstartup->enable)
            {
                if (pstartup->counter < MCAPP_STARTUP_RESET_CYCLES)
                {
                    ++pstartup->counter;
                }
                else
                {
                    pstartup->state = SSM_CURRENT_RAMPUP;
                }
            }
            pstartup->iNominal = pstartup->iAmplitude;
            break;
        case SSM_CURRENT_RAMPUP:
        {
            idqcmd_next.d = UTIL_LimitSlewRateSymmetrical(
                    pstartup->iNominal,                /* input */
                    idqcmd.d,                          /* previousOutput */
                    pstartup->iRampupLimit);           /* limit */

            if (idqcmd.d == idqcmd_next.d)
            {
                pstartup->counter = 0;
                pstartup->state = SSM_ALIGN;
            }
            break;
        }
        case SSM_ALIGN:
        {
            if (pstartup->counter < pstartup->alignTime)
            {
                ++pstartup->counter;
            }
            else if (!pstartup->delayRequest)
            {
                pstartup->counter = 0;
                pstartup->state = SSM_REF_FRAME_ALIGN;
            }
            break;
        }
        case SSM_REF_FRAME_ALIGN:
        {
            idqcmd_next.d = 0;
            if (!pstartup->delayRequest)
            {
                pstartup->complete = true;
                pstartup->state = SSM_SYNC_QEI;
            }
            break;
        }
        case SSM_SYNC_QEI:
        {
            pstartup->state = SSM_COMPLETE;
            break;
        }
        case SSM_COMPLETE:
            /* startup complete: do nothing */
            break;
        case SSM_INACTIVE:
            /* do nothing in test modes */
            break;
        default:
        {
           // MCAF_UiFlashErrorCodeForever(ERR_INVALID_STARTUP_FSM_STATE);
        }
    }
    
    *idqCommand = idqcmd_next;
    pstartup->delayRequest = false;
}
