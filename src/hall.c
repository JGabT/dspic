/**
 * hall.c
 *
 * Hall State signal processing
 * 
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
#include <xc.h>
#include <stdint.h>
#include "util.h"
#include "hall.h"
#include "hall_params.h"
#include "userparms.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/tmr1.h"


#define DEGREES_75      (32768/3+32768/12)  /* 60 degrees plus tolerance */
#define DEGREES_30      (32768/6)  /* 30 degrees*/
#define DEGREES_15      (32768/12)  /* 15 degrees*/

#ifdef HALL_SEQUENCE
//    const uint16_t HALL_RADIANS_TABLE_TL[8] = {0,21844,43691,32767,0,10922,54613,0};  //513264
const uint16_t HALL_RADIANS_TABLE_TL[8] = {0,21844-10376,43691-10376,32767-10376,0-10376,10922-10376,54613-10376,0};  //manually tuned by referring to vd and vq
#else
    const uint16_t HALL_RADIANS_TABLE_TL[8] = {0,0,21844,10922,43691,54613,32768,0};  //sequence 326451
#endif

MCAPP_U_ANGLE_MECH MCAPP_HALLTrackingLoop(MCAPP_HALL_TRACKING_LOOP *ptrkloop, MCAPP_U_ANGLE_ELEC input);

/* variables for plotting */
MCAPP_U_ANGLE_ELEC plotPositionError;
MCAPP_U_VELOCITY_ELEC plotVelocityError;
MCAPP_U_ANGLE_ELEC plotHallPhaseAdvanced;
MCAPP_U_VELOCITY_ELEC plotQeiVelocity;

static inline uint16_t HAL_HallsStateRead(void) 
{
#ifdef HALL_SEQUENCE
    return (((uint16_t) (IO_HALL1_GetValue() + (IO_HALL2_GetValue()<<1) + (IO_HALL3_GetValue()<<2))) & 0x7 );
#else
    return (((uint16_t) ~(IO_HALL1_GetValue() + (IO_HALL2_GetValue()<<1) + (IO_HALL3_GetValue()<<2))) & 0x7 );
#endif
}

uint16_t getRotorInitialPosition(void)
{
    return HALL_RADIANS_TABLE_TL[HAL_HallsStateRead()];
}

void MCAPP_EstimatorHallStartupInit(MCAPP_ESTIMATOR_HALL_T *phall)
{
    phall->trackingLoop.angleIntegrator.x32 = ((uint32_t)getRotorInitialPosition()<<15);
    phall->trackingLoop.velocityIntegrator.x32 = 0;
}


void MCAPP_EstimatorHallInit(MCAPP_ESTIMATOR_HALL_T *phall)
{
    phall->trackingLoop.kp = MCAPP_HALL_TRACKING_LOOP_KP; 
    phall->trackingLoop.kpShift = MCAPP_HALL_TRACKING_LOOP_KP_Q;
    phall->trackingLoop.ki = MCAPP_HALL_TRACKING_LOOP_KI; 
    phall->trackingLoop.dt = MCAPP_HALL_TRACKING_LOOP_DT;
    phall->trackingLoop.angleIntegrator.x32 = ((uint32_t)getRotorInitialPosition()<<15);
    phall->trackingLoop.velocityIntegrator.x32 = 0; 
}

int16_t calcPositionVelocityTrkLoop (MCAPP_ESTIMATOR_HALL_T *phall, uint16_t velocity,int16_t direction )
{
    /* compute current position based on previous velocity */    
    int16_t retValue;  
    uint16_t velocityAbs;
    velocityAbs = UTIL_Abs16(velocity);
    
    /* wait until velocity falls to a low enough speed to acknowledge  the UI imposed direction change */
    if (velocityAbs < 200)
    {
        if ( phall->direction != direction )
        {
           phall->direction = direction;
           phall->positionTrkLoop = phall->prevPosition - phall->direction * DEGREES_30;  
        }
    }
    /* compute current position based on previous velocity */     
    phall->omegaElectricalTrkLoop = phall->direction * velocityAbs;
    retValue = UTIL_MulQ15( phall->omegaElectricalTrkLoop , MCAPP_HALL_DT_ANGULAR_Q15 );
    phall->deltaPos = UTIL_Abs16(phall->positionTrkLoop - phall->prevPosition);
    if (phall->deltaPos > DEGREES_75)
        retValue = 0;
    return retValue;
}


inline static MCAPP_U_ANGLE_ELEC MCAF_TrackingLoopGetEstimatedAngleQ15(const MCAPP_HALL_TRACKING_LOOP *ptrkloop)
{
    return ptrkloop->angleIntegrator.x32 >> 15;
}

MCAPP_U_VELOCITY_ELEC MCAPP_HALLTrackingLoop(MCAPP_HALL_TRACKING_LOOP *ptrkloop, MCAPP_U_ANGLE_ELEC input)
{
    ptrkloop->angleInput = input;
    /* First integrate the PI loop output into the angle integrator. */
    ptrkloop->angleIntegrator.x32 += UTIL_mulss(ptrkloop->velocityEstimateUnfiltered, ptrkloop->dt);
    /*
     * In normal non-wrapping PI loops we have to be cautious about
     * wraparound error (difference of two int16_t is actually an int17_t)
     * but here we are safe since the error aliases to the principal angle
     * (between -pi and +pi radians error)
     */
    ptrkloop->angleError = ptrkloop->angleInput - MCAF_TrackingLoopGetEstimatedAngleQ15(ptrkloop);
    
    /*
     * Now iterate one step of the PI loop.
     * Ki is Q16, Kp is Qn
     * velocityIntegrator is Q31 = Q15 * Q16
     */
    ptrkloop->velocityIntegrator.x32 += UTIL_mulss(ptrkloop->angleError, ptrkloop->ki);
    /*
     * pi_sum = Q15 = (Q15 * Qn) >> n  (P term)
     *              + (Q15 * Q16) >> 16  (I term)
     */
    const int32_t pterm = UTIL_mulss(ptrkloop->angleError, ptrkloop->kp);
    
    int32_t pi_sum = UTIL_ShrS32N16(pterm, ptrkloop->kpShift);
    pi_sum += ptrkloop->velocityIntegrator.x16.hi;
    const int16_t satlimit = 0x7FFF;
    if (pi_sum >= satlimit)
    {
        ptrkloop->velocityEstimateUnfiltered = satlimit;
    }
    else if (pi_sum <= -satlimit)
    {
        ptrkloop->velocityEstimateUnfiltered = -satlimit;
    }
    else
    {
        ptrkloop->velocityEstimateUnfiltered = (int16_t)pi_sum;
    }
    return ptrkloop->velocityIntegrator.x16.hi;
}

/**
 * Executes one step of the Hall estimator
 * 
 * @param phall Hall state
 */
void MCAPP_EstimatorHallStep(MCAPP_ESTIMATOR_HALL_T *phall)
{
    int16_t velocity;
    
    /* reads halls */
    phall->readHall = HAL_HallsStateRead();    
    if ( phall->readHall != phall->prevHall )
    {
        phall->prevHall = phall->readHall;
        /* get absolute position */        
        phall->prevPosition = HALL_RADIANS_TABLE_TL[phall->readHall];
        /* update rotor position to actual value from Hall pattern */
        phall->positionTrkLoop = phall->prevPosition - phall->direction * DEGREES_30; 
        
    }
    velocity = MCAPP_HALLTrackingLoop(&phall->trackingLoop, phall->prevPosition);
    phall->positionTrkLoop += calcPositionVelocityTrkLoop( phall, velocity, mcappData.runDirection);
}

