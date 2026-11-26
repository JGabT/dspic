#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Vontroller_AI.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Vontroller_AI.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=hal/board_service.c hal/measure.c mcc_generated_files/X2Cscope/X2Cscope.c mcc_generated_files/X2Cscope/X2CscopeComm.c mcc_generated_files/pin_manager.c mcc_generated_files/mcc.c mcc_generated_files/reset.c mcc_generated_files/adc1.c mcc_generated_files/system.c mcc_generated_files/pwm.c mcc_generated_files/opa.c mcc_generated_files/traps.c mcc_generated_files/clock.c mcc_generated_files/uart1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/cmp1.c mcc_generated_files/tmr1.c mcc_generated_files/cmp3.c mcc_generated_files/cmp2.c mcc_generated_files/sccp1_tmr.c src/fault.c src/motor_drive.c src/hall.c src/startup.c src/sat_PI.c src/math_asm.s src/pas.c main.c src/estim.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/hal/board_service.o ${OBJECTDIR}/hal/measure.o ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/pwm.o ${OBJECTDIR}/mcc_generated_files/opa.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/cmp1.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/cmp3.o ${OBJECTDIR}/mcc_generated_files/cmp2.o ${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o ${OBJECTDIR}/src/fault.o ${OBJECTDIR}/src/motor_drive.o ${OBJECTDIR}/src/hall.o ${OBJECTDIR}/src/startup.o ${OBJECTDIR}/src/sat_PI.o ${OBJECTDIR}/src/math_asm.o ${OBJECTDIR}/src/pas.o ${OBJECTDIR}/main.o ${OBJECTDIR}/src/estim.o
POSSIBLE_DEPFILES=${OBJECTDIR}/hal/board_service.o.d ${OBJECTDIR}/hal/measure.o.d ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o.d ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/reset.o.d ${OBJECTDIR}/mcc_generated_files/adc1.o.d ${OBJECTDIR}/mcc_generated_files/system.o.d ${OBJECTDIR}/mcc_generated_files/pwm.o.d ${OBJECTDIR}/mcc_generated_files/opa.o.d ${OBJECTDIR}/mcc_generated_files/traps.o.d ${OBJECTDIR}/mcc_generated_files/clock.o.d ${OBJECTDIR}/mcc_generated_files/uart1.o.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/cmp1.o.d ${OBJECTDIR}/mcc_generated_files/tmr1.o.d ${OBJECTDIR}/mcc_generated_files/cmp3.o.d ${OBJECTDIR}/mcc_generated_files/cmp2.o.d ${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o.d ${OBJECTDIR}/src/fault.o.d ${OBJECTDIR}/src/motor_drive.o.d ${OBJECTDIR}/src/hall.o.d ${OBJECTDIR}/src/startup.o.d ${OBJECTDIR}/src/sat_PI.o.d ${OBJECTDIR}/src/math_asm.o.d ${OBJECTDIR}/src/pas.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/src/estim.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/hal/board_service.o ${OBJECTDIR}/hal/measure.o ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/pwm.o ${OBJECTDIR}/mcc_generated_files/opa.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/cmp1.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/cmp3.o ${OBJECTDIR}/mcc_generated_files/cmp2.o ${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o ${OBJECTDIR}/src/fault.o ${OBJECTDIR}/src/motor_drive.o ${OBJECTDIR}/src/hall.o ${OBJECTDIR}/src/startup.o ${OBJECTDIR}/src/sat_PI.o ${OBJECTDIR}/src/math_asm.o ${OBJECTDIR}/src/pas.o ${OBJECTDIR}/main.o ${OBJECTDIR}/src/estim.o

# Source Files
SOURCEFILES=hal/board_service.c hal/measure.c mcc_generated_files/X2Cscope/X2Cscope.c mcc_generated_files/X2Cscope/X2CscopeComm.c mcc_generated_files/pin_manager.c mcc_generated_files/mcc.c mcc_generated_files/reset.c mcc_generated_files/adc1.c mcc_generated_files/system.c mcc_generated_files/pwm.c mcc_generated_files/opa.c mcc_generated_files/traps.c mcc_generated_files/clock.c mcc_generated_files/uart1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/cmp1.c mcc_generated_files/tmr1.c mcc_generated_files/cmp3.c mcc_generated_files/cmp2.c mcc_generated_files/sccp1_tmr.c src/fault.c src/motor_drive.c src/hall.c src/startup.c src/sat_PI.c src/math_asm.s src/pas.c main.c src/estim.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Vontroller_AI.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33CK256MP505
MP_LINKER_FILE_OPTION=,--script=p33CK256MP505.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/hal/board_service.o: hal/board_service.c  .generated_files/flags/default/a53bf6370719d5c49247ffef7d53f198095289d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/board_service.o.d 
	@${RM} ${OBJECTDIR}/hal/board_service.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/board_service.c  -o ${OBJECTDIR}/hal/board_service.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/hal/board_service.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/hal/measure.o: hal/measure.c  .generated_files/flags/default/fb12982316e2cad5f562d3775214e5b993a5320c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/measure.o.d 
	@${RM} ${OBJECTDIR}/hal/measure.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/measure.c  -o ${OBJECTDIR}/hal/measure.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/hal/measure.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o: mcc_generated_files/X2Cscope/X2Cscope.c  .generated_files/flags/default/bb770bdca02b3079c671e7e519b1dc3660b6103b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/X2Cscope" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/X2Cscope/X2Cscope.c  -o ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o: mcc_generated_files/X2Cscope/X2CscopeComm.c  .generated_files/flags/default/80529d294dc7ba1f509aa3f6ca45d7856026914a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/X2Cscope" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/X2Cscope/X2CscopeComm.c  -o ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/fb6f546edbd890d1daa8132ae78b49cf6de85b1e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/ec4ec72e982993fb4418097fc48079572618433d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/default/17e23658add39672071144a43e9f64a96d46ded7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/default/3f186c9658b80f8eaf2977e5c3d79bf91208445f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/default/a9749e8d3f50c72e489954f4fa2d3184f38ed957 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pwm.o: mcc_generated_files/pwm.c  .generated_files/flags/default/8c7b81c1a7e625b7cf0b737ec7587877ca31b602 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pwm.c  -o ${OBJECTDIR}/mcc_generated_files/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pwm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/opa.o: mcc_generated_files/opa.c  .generated_files/flags/default/3e78603dc1ce83feb83de82092774abdaab49a97 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/opa.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/opa.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/opa.c  -o ${OBJECTDIR}/mcc_generated_files/opa.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/opa.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/default/c3af8e5728ed763e814598571f2a99ffafbedee6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/default/6c375ae66d78329b2115ec910229eaba0ebab5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/227ee70cf98aabda33decdf4f8c1303fa1956243 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/a1d14e91324c478d07d2073ba27304cbb7858352 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/cmp1.o: mcc_generated_files/cmp1.c  .generated_files/flags/default/3c4dd3bea4d9497f426523ea16cd3a11b5cceacb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/cmp1.c  -o ${OBJECTDIR}/mcc_generated_files/cmp1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/cmp1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/default/d384367e69bb165acde9771310696e20af8cefec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/cmp3.o: mcc_generated_files/cmp3.c  .generated_files/flags/default/5823baa5adbbec6e85b046d503c64f3921ffdc5a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/cmp3.c  -o ${OBJECTDIR}/mcc_generated_files/cmp3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/cmp3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/cmp2.o: mcc_generated_files/cmp2.c  .generated_files/flags/default/f5a9421a21cb5ab73ec0205ae1e96aef266ef430 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/cmp2.c  -o ${OBJECTDIR}/mcc_generated_files/cmp2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/cmp2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o: mcc_generated_files/sccp1_tmr.c  .generated_files/flags/default/b12f4af061ddc0007395bcaf3776ddfdf2757ee2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/sccp1_tmr.c  -o ${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/fault.o: src/fault.c  .generated_files/flags/default/f69d8c2b8228ddcce01e1e17b1539241bd85f8bb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/fault.o.d 
	@${RM} ${OBJECTDIR}/src/fault.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/fault.c  -o ${OBJECTDIR}/src/fault.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/fault.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/motor_drive.o: src/motor_drive.c  .generated_files/flags/default/a3de90dce1b0a7966964e149cddcc869ccc2c867 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/motor_drive.o.d 
	@${RM} ${OBJECTDIR}/src/motor_drive.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/motor_drive.c  -o ${OBJECTDIR}/src/motor_drive.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/motor_drive.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/hall.o: src/hall.c  .generated_files/flags/default/4fa349e9f5fac5f40953fc82a513108b12bb76a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/hall.o.d 
	@${RM} ${OBJECTDIR}/src/hall.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/hall.c  -o ${OBJECTDIR}/src/hall.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/hall.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/startup.o: src/startup.c  .generated_files/flags/default/d8ad56b933a27a9abd1472f683445e68e77cd22f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/startup.o.d 
	@${RM} ${OBJECTDIR}/src/startup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/startup.c  -o ${OBJECTDIR}/src/startup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/startup.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/sat_PI.o: src/sat_PI.c  .generated_files/flags/default/2596acb1d8969404c4c1f60f4d2264e7ff4b41d6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/sat_PI.o.d 
	@${RM} ${OBJECTDIR}/src/sat_PI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/sat_PI.c  -o ${OBJECTDIR}/src/sat_PI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/sat_PI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/pas.o: src/pas.c  .generated_files/flags/default/7d1d73976723267f1d7e7cfe05f6ddece1b0b38e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/pas.o.d 
	@${RM} ${OBJECTDIR}/src/pas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/pas.c  -o ${OBJECTDIR}/src/pas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/pas.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/6910e7e4c9f8a98e8bd73868d565388a10c87ce5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/estim.o: src/estim.c  .generated_files/flags/default/457491ee6fea2fc59448b81a3f070b69fcb7a1d0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/estim.o.d 
	@${RM} ${OBJECTDIR}/src/estim.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/estim.c  -o ${OBJECTDIR}/src/estim.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/estim.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/hal/board_service.o: hal/board_service.c  .generated_files/flags/default/4290035cc7613f99f71e5c2af7e4cfc0a33a49ab .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/board_service.o.d 
	@${RM} ${OBJECTDIR}/hal/board_service.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/board_service.c  -o ${OBJECTDIR}/hal/board_service.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/hal/board_service.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/hal/measure.o: hal/measure.c  .generated_files/flags/default/dd27f5ad3f159cf4446b7494951763bb2f8de9ab .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/hal" 
	@${RM} ${OBJECTDIR}/hal/measure.o.d 
	@${RM} ${OBJECTDIR}/hal/measure.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  hal/measure.c  -o ${OBJECTDIR}/hal/measure.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/hal/measure.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o: mcc_generated_files/X2Cscope/X2Cscope.c  .generated_files/flags/default/a650de4f54aa8c1d14d714576eea4a856b9ed839 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/X2Cscope" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/X2Cscope/X2Cscope.c  -o ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/X2Cscope/X2Cscope.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o: mcc_generated_files/X2Cscope/X2CscopeComm.c  .generated_files/flags/default/c553d3b4de930de60555dac9f4cdf3fd922d2885 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/X2Cscope" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/X2Cscope/X2CscopeComm.c  -o ${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/X2Cscope/X2CscopeComm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/91da78ca335be6dcc7542d007af96297adb8c45d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/dd02f1ab0618df4323937a35781318ae135a4293 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/default/3e970445f91c3d8d295cfb2c83abc91542826101 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/default/b36dfad22c58326c4e02a25c5ce91411c5247468 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/default/4584a56845ddc51fabd61bf9ea5dc23e0189ec17 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pwm.o: mcc_generated_files/pwm.c  .generated_files/flags/default/ce17ca65191ef6355a80170c5405e73a1de33dfb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pwm.c  -o ${OBJECTDIR}/mcc_generated_files/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pwm.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/opa.o: mcc_generated_files/opa.c  .generated_files/flags/default/48d192bb1e0c625c14e8bc5c8a6c27fb7644de6c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/opa.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/opa.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/opa.c  -o ${OBJECTDIR}/mcc_generated_files/opa.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/opa.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/default/862786429f9ea2b41833c5a2f35c820b2aaf6efe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/default/a7ee0e5ed98543acbe8ec500c0536d97157a4f91 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/c93145442461c0a431d8f49bb0742399bd238463 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/62f60acce86203c197cb018c4cf48c7f759fa380 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/cmp1.o: mcc_generated_files/cmp1.c  .generated_files/flags/default/4ce7106a9906a637c8adc1353788ef3d1f671f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/cmp1.c  -o ${OBJECTDIR}/mcc_generated_files/cmp1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/cmp1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/default/61a24db082a26d75d1930f2a8a5d3d1dd17b1cf1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/cmp3.o: mcc_generated_files/cmp3.c  .generated_files/flags/default/fa743d86a758061e6298979eb7cc1d64cd89e3df .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/cmp3.c  -o ${OBJECTDIR}/mcc_generated_files/cmp3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/cmp3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/cmp2.o: mcc_generated_files/cmp2.c  .generated_files/flags/default/db6ddd045212082386904fe3534e54191fcf1da5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/cmp2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/cmp2.c  -o ${OBJECTDIR}/mcc_generated_files/cmp2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/cmp2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o: mcc_generated_files/sccp1_tmr.c  .generated_files/flags/default/b5fd6f3a6b9e59dbecd684bcb5e256348e9e45fc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/sccp1_tmr.c  -o ${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/sccp1_tmr.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/fault.o: src/fault.c  .generated_files/flags/default/c80a7f056b2a5ff54ff8e72f880ea965cae16af8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/fault.o.d 
	@${RM} ${OBJECTDIR}/src/fault.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/fault.c  -o ${OBJECTDIR}/src/fault.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/fault.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/motor_drive.o: src/motor_drive.c  .generated_files/flags/default/31f434c97c24093d259f5fb15e550c027bdca6b5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/motor_drive.o.d 
	@${RM} ${OBJECTDIR}/src/motor_drive.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/motor_drive.c  -o ${OBJECTDIR}/src/motor_drive.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/motor_drive.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/hall.o: src/hall.c  .generated_files/flags/default/eabd15a267a3aef20c492c1356aa9b27b2a347d5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/hall.o.d 
	@${RM} ${OBJECTDIR}/src/hall.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/hall.c  -o ${OBJECTDIR}/src/hall.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/hall.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/startup.o: src/startup.c  .generated_files/flags/default/e20962678424b87009440bd35f82b631451f5c71 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/startup.o.d 
	@${RM} ${OBJECTDIR}/src/startup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/startup.c  -o ${OBJECTDIR}/src/startup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/startup.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/sat_PI.o: src/sat_PI.c  .generated_files/flags/default/f09e8b1fcb8bfd1d4b0b59ec3e67382224cff2e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/sat_PI.o.d 
	@${RM} ${OBJECTDIR}/src/sat_PI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/sat_PI.c  -o ${OBJECTDIR}/src/sat_PI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/sat_PI.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/pas.o: src/pas.c  .generated_files/flags/default/26929967e6f9b6d3eecd50b8dfe0c864f9b09299 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/pas.o.d 
	@${RM} ${OBJECTDIR}/src/pas.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/pas.c  -o ${OBJECTDIR}/src/pas.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/pas.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/ead279dbfff2b2f7bbc3cfd28f68daf5cd12f7a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/src/estim.o: src/estim.c  .generated_files/flags/default/32157dc4f08166806728b66044de1be7651f9ced .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/estim.o.d 
	@${RM} ${OBJECTDIR}/src/estim.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/estim.c  -o ${OBJECTDIR}/src/estim.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/src/estim.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/math_asm.o: src/math_asm.s  .generated_files/flags/default/3af25249c4c1e058bfb11c841753eac236e63c5f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/math_asm.o.d 
	@${RM} ${OBJECTDIR}/src/math_asm.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/math_asm.s  -o ${OBJECTDIR}/src/math_asm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -omf=elf -DXPRJ_default=$(CND_CONF)    -I"motor_control" -Wa,-MD,"${OBJECTDIR}/src/math_asm.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK4=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/src/math_asm.o: src/math_asm.s  .generated_files/flags/default/890b2093207b11ad4e554ba871f5a1ea14b8cc15 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/math_asm.o.d 
	@${RM} ${OBJECTDIR}/src/math_asm.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/math_asm.s  -o ${OBJECTDIR}/src/math_asm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)    -I"motor_control" -Wa,-MD,"${OBJECTDIR}/src/math_asm.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Vontroller_AI.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  mcc_generated_files/X2Cscope/libx2cscope-33ck-elf.a mc_lib/libmotor_control_dspic-elf.a  
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Vontroller_AI.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    mcc_generated_files\X2Cscope\libx2cscope-33ck-elf.a mc_lib\libmotor_control_dspic-elf.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK4=1  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK4=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library=q,--library-path="mcc_generated_files/X2Cscope",--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Vontroller_AI.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  mcc_generated_files/X2Cscope/libx2cscope-33ck-elf.a mc_lib/libmotor_control_dspic-elf.a 
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Vontroller_AI.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    mcc_generated_files\X2Cscope\libx2cscope-33ck-elf.a mc_lib\libmotor_control_dspic-elf.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library=q,--library-path="mcc_generated_files/X2Cscope",--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/Vontroller_AI.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
