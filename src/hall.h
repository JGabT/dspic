/**
 * hall.h
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

#ifndef __HALL_H
#define __HALL_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "util.h"
#include "units.h"
#include "hall_params.h"
#include "userparms.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    

#define HALL_SEQUENCE 513264   //Uncomment this definition if the sequence is 326451
    
#define MCAPP_HALL_DT_ANGULAR         MCAPP_HALL_TRACKING_LOOP_DT 
#define MCAPP_HALL_DT_ANGULAR_Q15     MCAPP_HALL_DT_ANGULAR  

/**
 * Tracking loop state variables
 * Not used for now
 */
typedef struct tagHallTrackingLoop
{
    int16_t           kp;                 /** proportional gain */
    uint16_t          kpShift;            /** shift count for kp */
    int16_t           ki;                 /** integral gain */
    int16_t           dt;                 /** timestep */
    MCAPP_U_ANGLE_ELEC angleInput;         /** input angle */
    sx1632_t          velocityIntegrator; /** velocity integrator */
    MCAPP_U_VELOCITY_ELEC velocityEstimateUnfiltered;   /** velocity estimate */
    sx1632_t          angleIntegrator;    /** angle integrator */
    /**
     * angle error, retained as a state variable to assist in debugging and testing.
     */
    MCAPP_U_ANGLE_ELEC  angleError; 
} MCAPP_HALL_TRACKING_LOOP;

typedef struct tagHallPosOmegaInterpolator
{
    int16_t           kp;                 /** proportional gain */
    uint16_t          kpShift;            /** shift count for kp */
    int16_t           ki;                 /** integral gain */
    int16_t           dt;                 /** timestep */
    MCAPP_U_ANGLE_ELEC angleInput;         /** input angle */
    sx1632_t          velocityIntegrator; /** velocity integrator */
    MCAPP_U_VELOCITY_ELEC velocityEstimateUnfiltered;   /** velocity estimate */
    sx1632_t          angleIntegrator;    /** angle integrator */
    
    int32_t              periodStateVar;
    int16_t              PeriodKFilter;
    uint16_t             phaseInc;   
    uint16_t             period;
    int16_t              periodFilter;
    int16_t              velocity;
    /**
     * angle error, retained as a state variable to assist in debugging and testing.
     */
    MCAPP_U_ANGLE_ELEC  angleError; 
} MCAPP_HALL_POS_OMEGA_INTERPOLATOR_T;
/**
 * State variables for the Hall estimator
 */
typedef struct tagMCAPP_ESTIMATOR_HALL_T
{
    uint16_t             startup;
    uint16_t             prevHall;
    uint16_t             readHall;
    uint16_t             deltaPos;
    MCAPP_U_ANGLE_ELEC    prevPosition;
    MCAPP_U_ANGLE_ELEC    positionTrkLoop;
    MCAPP_U_ANGLE_ELEC    positionTrkLoopInternal;
    int16_t              direction;
    MCAPP_U_VELOCITY_ELEC omegaElectricalTrkLoop;
    MCAPP_HALL_TRACKING_LOOP   trackingLoop;    
} MCAPP_ESTIMATOR_HALL_T;
 
typedef struct tagMCAPP_ESTIMATOR_HALL_HE_T
{
    uint16_t             startup;
    uint16_t             prevHall;
    uint16_t             readHall;
    uint16_t             deltaPos;
    
    uint16_t             timerValue;      
    
    MCAPP_U_ANGLE_ELEC    prevPosition;
    MCAPP_U_ANGLE_ELEC    position;
    int16_t              direction;
    MCAPP_U_VELOCITY_ELEC omegaElectrical;
//    MCAPP_HALL_TRACKING_LOOP   trackingLoop;   
    MCAPP_HALL_POS_OMEGA_INTERPOLATOR_T posOmegaInterpolator;
} MCAPP_ESTIMATOR_HALL_HE_T;

    
/**
 * Returns commutation angle
 * 
 * @param phall Hall state
 * @return commutation angle
 */
inline static MCAPP_U_ANGLE_ELEC MCAPP_EstimatorHallCommutationAngle(const MCAPP_ESTIMATOR_HALL_T *phall)
{
     return phall->positionTrkLoop;
}

/**
 * Returns electrical frequency
 * 
 * @param phall Hall state
 * @return electrical frequency
 */
inline static MCAPP_U_VELOCITY_ELEC MCAPP_EstimatorHallElectricalFrequency(const MCAPP_ESTIMATOR_HALL_T *phall)
{
    return phall->omegaElectricalTrkLoop;
}

//Modified Old Estimator
inline static MCAPP_U_ANGLE_ELEC MCAPP_EstimatorHallCommutationAngleHE(const MCAPP_ESTIMATOR_HALL_HE_T *phall)
{
     return phall->position;
}

inline static MCAPP_U_VELOCITY_ELEC MCAPP_EstimatorHallElectricalFrequencyHE(const MCAPP_ESTIMATOR_HALL_HE_T *phall)
{
    return phall->omegaElectrical;
}


/**
 * Initializes Hall state on reset.
 *
 * @param phall Hall state
 */
void MCAPP_EstimatorHallInit(MCAPP_ESTIMATOR_HALL_T *phall); 
void MCAPP_EstimatorHallStartupInit(MCAPP_ESTIMATOR_HALL_T *phall);
void MCAPP_EstimatorHallStep(MCAPP_ESTIMATOR_HALL_T *phall);

void MCAPP_EstimatorHallInitHE(MCAPP_ESTIMATOR_HALL_HE_T *phall_HE); 
void MCAPP_EstimatorHallStartupInitHE(MCAPP_ESTIMATOR_HALL_HE_T *phall_HE);
void MCAPP_EstimatorHallStepHE(MCAPP_ESTIMATOR_HALL_HE_T *phall_HE);

extern MCAPP_ESTIMATOR_HALL_T hallEstimator;
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* HALL_H */

