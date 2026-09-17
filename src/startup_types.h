/**
 * startup_types.h
 * 
 * Type definitions for startup and open loop to closed loop transition 
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

#ifndef __STARTUP_TYPES_H
#define __STARTUP_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include "units.h"
#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

  
/** Different state for startup and open-loop to close transitioning  */
typedef enum tagMCAPP_STARTUP_FSM_STATE
{
   /** Start state (entered only on reset + exiting fault) */
   SSM_START            = 0,
   /** Rotor is locked during align state. 
    * Current ramp is applied during this state, angle held constant */
   SSM_CURRENT_RAMPUP   = 1, 
   /** Rotor is locked during align state. 
    * Current and angle held constant. */
   SSM_ALIGN            = 2,
   /** Motor is allowed to recover after initial acceleration during hold state. 
    * Speed and applied current torque command are constant during this state. */     
   SSM_REF_FRAME_ALIGN  = 3,
   /** Synchronizes with QEI offset function to allow for update of status */
   SSM_SYNC_QEI         = 4,
   /** indicates completion of open to close loop transition */
   SSM_COMPLETE         = 5,
   SSM_INACTIVE         = 6   /** inactive state for test modes */                   
} MCAPP_STARTUP_FSM_STATE;

typedef enum tagMCAPP_STARTUP_STATUS_T
{
    /** startup in progress; something is going on */
    MSST_UNSPECIFIED = 0,
    /** startup restarting, awaiting further action */
    MSST_RESTART = 1,
    /** align: commutation angle and current are constant */
    MSST_ALIGN = 2,
    /** accel: electrical frequency increases to accelerate the motor */
    MSST_ACCEL = 3,
    /** spin: electrical frequency held constant to support measurements (example: QEI synchronization) */
    MSST_SPIN = 4,
    /** angle lock: electrical frequency may be changed by primary estimator */
    MSST_ANGLE_LOCK = 5,
    /** startup is complete; return to closed-loop operation */
    MSST_COMPLETE = 6
} MCAPP_STARTUP_STATUS_T;

/**
 * Motor startup state variables for use during open-loop startup.
 * 
 * Some of these variables appear to duplicate the main state;
 * this is because during open-loop startup, the applied electrical angle
 * and the estimated electrical angle are different.
 * Variables stored here are related to the applied electrical angle.
 */
typedef struct tagMOTOR_STARTUP_DATA
{
     /** Slewrate limit for current. It is given by (max openloop Iq/rampup rate) */
     int16_t              iRampupLimit;   

     /** estimated electrical angle (from outside of open-loop operation) */
     MCAPP_U_ANGLE_ELEC    thetaElectricalEstimated;
     MCAPP_U_VELOCITY_ELEC omegaElectricalEstimated;
     
     /* ----- open-loop commutation ----- */
     sx1632_t             omegaElectrical;  /** open-loop electrical frequency */
     sx1632_t             thetaElectrical;  /** open-loop electrical angle */
     MCAPP_U_ANGLE_ELEC    rampupAngle;      /** angle applied during current rampup */
     MCAPP_U_ANGLE_ELEC    alignAngleDelta;  /** angle shift for align stage */
     int16_t           dt;               /** timestep scaling factor for angle */
     /**
      * Time after initial rampup of current at a fixed electrical angle,
      * and before open-loop acceleration during startup
      */
     uint32_t             alignTime;
     /** Amplitude of applied current during startup */
     MCAPP_U_CURRENT       iAmplitude;
     /** Nominal current applied during startup (always along the positive D axis) */
     MCAPP_U_CURRENT       iNominal;
     /** open-loop state machine state */
     MCAPP_STARTUP_FSM_STATE state;
     /** Whether open-loop startup is enabled */
     bool                 enable;
     /** Whether open-loop startup is complete */
     bool                 complete;
     /** Request for delay */
     bool                 delayRequest;
     /** Counter used in openloop for different states */
     uint32_t             counter;  
     
} MCAPP_MOTOR_STARTUP_DATA;

/**
 * Get implementation-independent status category
 * @param pstartup startup state
 * @return status category
 */
inline static MCAPP_STARTUP_STATUS_T MCAPP_STARTUPGetStatus(const MCAPP_MOTOR_STARTUP_DATA *pstartup)
{
    MCAPP_STARTUP_FSM_STATE state = pstartup->state;
    switch (state)
    {
        case SSM_START:      return MSST_RESTART;
        case SSM_ALIGN:      return MSST_ALIGN;
        case SSM_REF_FRAME_ALIGN:
                             return MSST_ANGLE_LOCK;
        case SSM_SYNC_QEI:   return MSST_SPIN;
        case SSM_COMPLETE:   return MSST_COMPLETE;
        default:             return MSST_UNSPECIFIED;
    }
}

/* Startup delay mechanism:
 * 
 * Estimators or other algorithms may request the startup routine 
 * to delay progress during certain stages, if the startup algorithm allows it.
 * 
 * MCAPP_STARTUPRequestDelay   -- other algorithm requests a delay in startup
 * MCAPP_STARTUPDelayPermitted -- startup specifies whether it can be delayed
 */

/**
 * Set a delay request flag to remain in certain states (ALIGN and SPIN)
 * NOTE: This is not a persistent flag; it needs to be re-asserted every control ISR
 * @param pstartup startup state
 */
inline static void MCAPP_STARTUPRequestDelay(MCAPP_MOTOR_STARTUP_DATA *pstartup)
{
    pstartup->delayRequest = true;
}

/**
 * Determines whether additional delays are allowed
 * 
 * @param pstartup startup state
 * @return whether additional delay requests are accepted
 */
inline static bool MCAPP_STARTUPDelayPermitted(MCAPP_MOTOR_STARTUP_DATA *pstartup)
{
    const MCAPP_STARTUP_STATUS_T msst = MCAPP_STARTUPGetStatus(pstartup);
    return (msst == MSST_ALIGN) || (msst == MSST_ANGLE_LOCK);
}

/**
 * Get angle difference from estimator
 * @param pstartup startup state
 */
inline static MCAPP_U_ANGLE_ELEC MCAPP_STARTUPGetThetaError(const MCAPP_MOTOR_STARTUP_DATA *pstartup)
{
    return 0;
}

/**
 * Set angle difference from estimator
 * (important for smooth state transitions)
 * 
 * @param pstartup startup state
 * @param thetaError desired angle difference
 */
inline static void MCAPP_STARTUPSetThetaError(MCAPP_MOTOR_STARTUP_DATA *pstartup, MCAPP_U_ANGLE_ELEC thetaError)
{
    // no action required
}

/**
 * Get angle of current vector command in dq-plane
 * used during forced commutation.
 *
 * Note that this is in the forced reference frame
 * (the reference frame defined by the forced electrical angle)
 * and not the actual electrical angle. If the current is sufficiently large,
 * so that maximum torque is much larger than the load torque, 
 * it will tend to pull the rotor's d-axis towards the current vector.
 *
 * A current vector angle of zero (along the d-axis) 
 * will tend to pull the rotor's d-axis toward the d-axis
 * of the forced reference frame.
 *
 * @param pstartup startup state
 * @return angle of current vector
 */
inline static MCAPP_U_ANGLE_ELEC MCAPP_STARTUPGetIdqCmdAngle(const MCAPP_MOTOR_STARTUP_DATA *pstartup)
{
    // always along the positive d-axis
    return 0;
}

extern MCAPP_MOTOR_STARTUP_DATA motorStartUpData;
#ifdef __cplusplus
}
#endif

#endif /* __STARTUP_TYPES_H */