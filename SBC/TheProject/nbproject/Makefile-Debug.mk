#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/globals.o \
	${OBJECTDIR}/SerialComm.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/TheMap.o \
	${OBJECTDIR}/MessageBuilder.o \
	${OBJECTDIR}/LogFile.o \
	${OBJECTDIR}/SoundRecorder.o \
	${OBJECTDIR}/SoundFFT.o \
	${OBJECTDIR}/ColorTracking.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pthread
CXXFLAGS=-pthread

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath /usr/include/opencv2 -lopencv_core -lopencv_core -lopencv_highgui -lopencv_highgui -lopencv_imgproc -lopencv_imgproc

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/theproject

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/theproject: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/theproject ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/globals.o: globals.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/opencv2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/globals.o globals.cpp

${OBJECTDIR}/SerialComm.o: SerialComm.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/opencv2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/SerialComm.o SerialComm.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/opencv2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/TheMap.o: TheMap.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/opencv2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/TheMap.o TheMap.cpp

${OBJECTDIR}/MessageBuilder.o: MessageBuilder.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/opencv2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/MessageBuilder.o MessageBuilder.cpp

${OBJECTDIR}/LogFile.o: LogFile.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/opencv2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/LogFile.o LogFile.cpp

${OBJECTDIR}/SoundRecorder.o: SoundRecorder.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/opencv2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/SoundRecorder.o SoundRecorder.cpp

${OBJECTDIR}/SoundFFT.o: SoundFFT.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/opencv2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/SoundFFT.o SoundFFT.cpp

${OBJECTDIR}/ColorTracking.o: ColorTracking.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/include/opencv2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/ColorTracking.o ColorTracking.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/theproject

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
