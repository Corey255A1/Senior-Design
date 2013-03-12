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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC24_Ultrasonic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC24_Ultrasonic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1126441362/9axis.o ${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o ${OBJECTDIR}/_ext/1126441362/globals.o ${OBJECTDIR}/_ext/1126441362/i2c.o ${OBJECTDIR}/_ext/1126441362/pic24EP.o ${OBJECTDIR}/_ext/1126441362/servo.o ${OBJECTDIR}/_ext/1126441362/spi.o ${OBJECTDIR}/_ext/1126441362/temperature.o ${OBJECTDIR}/_ext/1126441362/ultrasonic.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1126441362/9axis.o.d ${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o.d ${OBJECTDIR}/_ext/1126441362/globals.o.d ${OBJECTDIR}/_ext/1126441362/i2c.o.d ${OBJECTDIR}/_ext/1126441362/pic24EP.o.d ${OBJECTDIR}/_ext/1126441362/servo.o.d ${OBJECTDIR}/_ext/1126441362/spi.o.d ${OBJECTDIR}/_ext/1126441362/temperature.o.d ${OBJECTDIR}/_ext/1126441362/ultrasonic.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1126441362/9axis.o ${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o ${OBJECTDIR}/_ext/1126441362/globals.o ${OBJECTDIR}/_ext/1126441362/i2c.o ${OBJECTDIR}/_ext/1126441362/pic24EP.o ${OBJECTDIR}/_ext/1126441362/servo.o ${OBJECTDIR}/_ext/1126441362/spi.o ${OBJECTDIR}/_ext/1126441362/temperature.o ${OBJECTDIR}/_ext/1126441362/ultrasonic.o


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PIC24_Ultrasonic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24EP32MC202
MP_LINKER_FILE_OPTION=,--script=p24EP32MC202.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1126441362/9axis.o: ../pic24EP.X/9axis.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/9axis.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/9axis.c  -o ${OBJECTDIR}/_ext/1126441362/9axis.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/9axis.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/9axis.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o: ../pic24EP.X/frontUltrasonic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/frontUltrasonic.c  -o ${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/globals.o: ../pic24EP.X/globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/globals.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/globals.c  -o ${OBJECTDIR}/_ext/1126441362/globals.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/globals.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/globals.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/i2c.o: ../pic24EP.X/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/i2c.c  -o ${OBJECTDIR}/_ext/1126441362/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/i2c.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/pic24EP.o: ../pic24EP.X/pic24EP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/pic24EP.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/pic24EP.c  -o ${OBJECTDIR}/_ext/1126441362/pic24EP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/pic24EP.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/pic24EP.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/servo.o: ../pic24EP.X/servo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/servo.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/servo.c  -o ${OBJECTDIR}/_ext/1126441362/servo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/servo.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/servo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/spi.o: ../pic24EP.X/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/spi.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/spi.c  -o ${OBJECTDIR}/_ext/1126441362/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/spi.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/temperature.o: ../pic24EP.X/temperature.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/temperature.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/temperature.c  -o ${OBJECTDIR}/_ext/1126441362/temperature.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/temperature.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/temperature.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/ultrasonic.o: ../pic24EP.X/ultrasonic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/ultrasonic.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/ultrasonic.c  -o ${OBJECTDIR}/_ext/1126441362/ultrasonic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/ultrasonic.o.d"        -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/ultrasonic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1126441362/9axis.o: ../pic24EP.X/9axis.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/9axis.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/9axis.c  -o ${OBJECTDIR}/_ext/1126441362/9axis.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/9axis.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/9axis.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o: ../pic24EP.X/frontUltrasonic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/frontUltrasonic.c  -o ${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/frontUltrasonic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/globals.o: ../pic24EP.X/globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/globals.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/globals.c  -o ${OBJECTDIR}/_ext/1126441362/globals.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/globals.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/globals.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/i2c.o: ../pic24EP.X/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/i2c.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/i2c.c  -o ${OBJECTDIR}/_ext/1126441362/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/i2c.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/pic24EP.o: ../pic24EP.X/pic24EP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/pic24EP.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/pic24EP.c  -o ${OBJECTDIR}/_ext/1126441362/pic24EP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/pic24EP.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/pic24EP.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/servo.o: ../pic24EP.X/servo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/servo.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/servo.c  -o ${OBJECTDIR}/_ext/1126441362/servo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/servo.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/servo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/spi.o: ../pic24EP.X/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/spi.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/spi.c  -o ${OBJECTDIR}/_ext/1126441362/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/spi.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/spi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/temperature.o: ../pic24EP.X/temperature.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/temperature.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/temperature.c  -o ${OBJECTDIR}/_ext/1126441362/temperature.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/temperature.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/temperature.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1126441362/ultrasonic.o: ../pic24EP.X/ultrasonic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1126441362 
	@${RM} ${OBJECTDIR}/_ext/1126441362/ultrasonic.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../pic24EP.X/ultrasonic.c  -o ${OBJECTDIR}/_ext/1126441362/ultrasonic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1126441362/ultrasonic.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1126441362/ultrasonic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PIC24_Ultrasonic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PIC24_Ultrasonic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PIC24_Ultrasonic.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PIC24_Ultrasonic.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/PIC24_Ultrasonic.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
