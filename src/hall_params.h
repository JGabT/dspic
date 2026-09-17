/* 
 * hall_params.h
 * 
 * parameters for the HALL module
 * 
 */ 
/*
 * (c) 2017 - 2023 Microchip Technology Inc. and its subsidiaries. You may use
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
 * motorBench(R) DEVELOPMENT SUITE TOOL, PARAMETERS AND GENERATED CODE,
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
 *
 ******************************************************************************/

#ifndef __HALL_PARAMS_H
#define __HALL_PARAMS_H


#ifdef  __cplusplus
extern "C" {
#endif

///*
//// * Hall Estimator tracking loop for velocity estimation:
//// *
//// * tau:                           Time constant of tracking loop
//// * zeta:                          Damping factor of tracking loop
//// * Ki:                            Integral gain of tracking loop = 1/tau^2
//// * Kp:                            Proportional gain of tracking loop = 2*zeta/tau
//// * Ts:                            Current loop sampling time
//// */
//// 
///* Tracking loop proportional gain */       
//#define MCAPP_HALL_TRACKING_LOOP_KP           15737  //1966  
//#define MCAPP_HALL_TRACKING_LOOP_KP_Q         15    //14   
///* Tracking loop integral gain */
//#define MCAPP_HALL_TRACKING_LOOP_KI           82    //8
///* Tracking loop timestep */
//#define MCAPP_HALL_TRACKING_LOOP_DT           1023  //1638    
   
    
/*
 * Encoder tracking loop for velocity estimation:
 *
 * tau:                  8.000 ms           Time constant of tracking loop
 * zeta:                 1.200              Damping factor of tracking loop
 * Ki:                  15.625 krad/s^2     Integral gain of tracking loop = 1/tau^2
 * Kp:                 300.000  rad/s       Proportional gain of tracking loop = 2*zeta/tau
 * Ts:                  50.000 us           Current loop sampling time
 */
 
/* Tracking loop proportional gain */
//#define MCAPP_HALL_TRACKING_LOOP_KP          13107      // Q15(  0.39999) = +299.99543 rad/s       = +300.00000 rad/s       - 0.0015%
//#define MCAPP_HALL_TRACKING_LOOP_KP_Q           15
///* Tracking loop integral gain */
//#define MCAPP_HALL_TRACKING_LOOP_KI             68      // Q16(  0.00104) =  +15.56397 krad/s^2    =  +15.62500 krad/s^2    - 0.3906%
///* Tracking loop timestep */
//#define MCAPP_HALL_TRACKING_LOOP_DT           1229      // Q15(  0.03751) =  +50.00814 us          =  +50.00000 us 

    
    
 /*
 * tau:                  4.500 ms           Time constant of tracking loop
 * zeta:                 1.200              Damping factor of tracking loop
 * Ki:                  49.383 krad/s^2     Integral gain of tracking loop = 1/tau^2
 * Kp:                 533.333  rad/s       Proportional gain of tracking loop = 2*zeta/tau
 * Ts:                  50.000 us           Current loop sampling time
 */
 
/* Tracking loop proportional gain */
//#define MCAF_HALL_TRACKING_LOOP_KP          11651      // Q14(  0.71112) = +533.34046 rad/s       = +533.33333 rad/s       + 0.0013%
//#define MCAF_HALL_TRACKING_LOOP_KP_Q           14
///* Tracking loop integral gain */
//#define MCAF_HALL_TRACKING_LOOP_KI            216      // Q16(  0.00330) =  +49.43848 krad/s^2    =  +49.38272 krad/s^2    + 0.1129%
///* Tracking loop timestep */
//#define MCAF_HALL_TRACKING_LOOP_DT           1229      // Q15(  0.03751) =  +50.00814 us          =  +50.00000 us   
    
 /*
 * Encoder tracking loop for velocity estimation:
 *
 * tau:                  4.500 ms           Time constant of tracking loop
 * zeta:                 1.200              Damping factor of tracking loop
 * Ki:                  49.383 krad/s^2     Integral gain of tracking loop = 1/tau^2
 * Kp:                 533.333  rad/s       Proportional gain of tracking loop = 2*zeta/tau
 * Ts:                  50.000 us           Current loop sampling time
 */
 
/* Tracking loop proportional gain */
//#define MCAPP_HALL_TRACKING_LOOP_KP          13988      // Q14(  0.85376) = +533.33002 rad/s       = +533.33333 rad/s       - 0.0006%
//#define MCAPP_HALL_TRACKING_LOOP_KP_Q           14
///* Tracking loop integral gain */
//#define MCAPP_HALL_TRACKING_LOOP_KI            259      // Q16(  0.00395) =  +49.37535 krad/s^2    =  +49.38272 krad/s^2    - 0.0149%
///* Tracking loop timestep */
//#define MCAPP_HALL_TRACKING_LOOP_DT           1023      // Q15(  0.03122) =  +49.97644 us          =  +50.00000 us          - 0.0471%

    
#define MCAPP_HALL_TRACKING_LOOP_KP          13981      // Q14(  0.85333) = +533.33283 rad/s       = +533.33333 rad/s       - 0.0001%
#define MCAPP_HALL_TRACKING_LOOP_KP_Q           14
/* Tracking loop integral gain */
#define MCAPP_HALL_TRACKING_LOOP_KI            259      // Q16(  0.00395) =  +49.40033 krad/s^2    =  +49.38272 krad/s^2    + 0.0357%
/* Tracking loop timestep */
#define MCAPP_HALL_TRACKING_LOOP_DT           1024      // Q15(  0.03125) =  +50.00000 us          =  +50.00000 us 
    

#ifdef  __cplusplus
}
#endif

#endif // __HALL_PARAMS_H
