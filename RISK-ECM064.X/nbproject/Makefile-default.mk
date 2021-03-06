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
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RISK-ECM064.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RISK-ECM064.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../game/cards.c ../game/game.c ../game/gamedata.c ../game/log.c ../game/territory.c ../game/save.c ../game/stats.c init.c main.c lcd.c buttons.c pi.c displays.c flash.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/760992031/cards.o ${OBJECTDIR}/_ext/760992031/game.o ${OBJECTDIR}/_ext/760992031/gamedata.o ${OBJECTDIR}/_ext/760992031/log.o ${OBJECTDIR}/_ext/760992031/territory.o ${OBJECTDIR}/_ext/760992031/save.o ${OBJECTDIR}/_ext/760992031/stats.o ${OBJECTDIR}/init.o ${OBJECTDIR}/main.o ${OBJECTDIR}/lcd.o ${OBJECTDIR}/buttons.o ${OBJECTDIR}/pi.o ${OBJECTDIR}/displays.o ${OBJECTDIR}/flash.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/760992031/cards.o.d ${OBJECTDIR}/_ext/760992031/game.o.d ${OBJECTDIR}/_ext/760992031/gamedata.o.d ${OBJECTDIR}/_ext/760992031/log.o.d ${OBJECTDIR}/_ext/760992031/territory.o.d ${OBJECTDIR}/_ext/760992031/save.o.d ${OBJECTDIR}/_ext/760992031/stats.o.d ${OBJECTDIR}/init.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/lcd.o.d ${OBJECTDIR}/buttons.o.d ${OBJECTDIR}/pi.o.d ${OBJECTDIR}/displays.o.d ${OBJECTDIR}/flash.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/760992031/cards.o ${OBJECTDIR}/_ext/760992031/game.o ${OBJECTDIR}/_ext/760992031/gamedata.o ${OBJECTDIR}/_ext/760992031/log.o ${OBJECTDIR}/_ext/760992031/territory.o ${OBJECTDIR}/_ext/760992031/save.o ${OBJECTDIR}/_ext/760992031/stats.o ${OBJECTDIR}/init.o ${OBJECTDIR}/main.o ${OBJECTDIR}/lcd.o ${OBJECTDIR}/buttons.o ${OBJECTDIR}/pi.o ${OBJECTDIR}/displays.o ${OBJECTDIR}/flash.o

# Source Files
SOURCEFILES=../game/cards.c ../game/game.c ../game/gamedata.c ../game/log.c ../game/territory.c ../game/save.c ../game/stats.c init.c main.c lcd.c buttons.c pi.c displays.c flash.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/RISK-ECM064.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048ECM064
MP_LINKER_FILE_OPTION=,--script="linker.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/760992031/cards.o: ../game/cards.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/cards.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/cards.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/cards.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/cards.o.d" -o ${OBJECTDIR}/_ext/760992031/cards.o ../game/cards.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/game.o: ../game/game.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/game.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/game.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/game.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/game.o.d" -o ${OBJECTDIR}/_ext/760992031/game.o ../game/game.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/gamedata.o: ../game/gamedata.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/gamedata.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/gamedata.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/gamedata.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/gamedata.o.d" -o ${OBJECTDIR}/_ext/760992031/gamedata.o ../game/gamedata.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/log.o: ../game/log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/log.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/log.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/log.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/log.o.d" -o ${OBJECTDIR}/_ext/760992031/log.o ../game/log.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/territory.o: ../game/territory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/territory.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/territory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/territory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/territory.o.d" -o ${OBJECTDIR}/_ext/760992031/territory.o ../game/territory.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/save.o: ../game/save.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/save.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/save.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/save.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/save.o.d" -o ${OBJECTDIR}/_ext/760992031/save.o ../game/save.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/stats.o: ../game/stats.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/stats.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/stats.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/stats.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/stats.o.d" -o ${OBJECTDIR}/_ext/760992031/stats.o ../game/stats.c    -std=gnu99
	
${OBJECTDIR}/init.o: init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/init.o.d 
	@${RM} ${OBJECTDIR}/init.o 
	@${FIXDEPS} "${OBJECTDIR}/init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/init.o.d" -o ${OBJECTDIR}/init.o init.c    -std=gnu99
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -std=gnu99
	
${OBJECTDIR}/lcd.o: lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd.o.d 
	@${RM} ${OBJECTDIR}/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/lcd.o.d" -o ${OBJECTDIR}/lcd.o lcd.c    -std=gnu99
	
${OBJECTDIR}/buttons.o: buttons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/buttons.o.d 
	@${RM} ${OBJECTDIR}/buttons.o 
	@${FIXDEPS} "${OBJECTDIR}/buttons.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/buttons.o.d" -o ${OBJECTDIR}/buttons.o buttons.c    -std=gnu99
	
${OBJECTDIR}/pi.o: pi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pi.o.d 
	@${RM} ${OBJECTDIR}/pi.o 
	@${FIXDEPS} "${OBJECTDIR}/pi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/pi.o.d" -o ${OBJECTDIR}/pi.o pi.c    -std=gnu99
	
${OBJECTDIR}/displays.o: displays.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/displays.o.d 
	@${RM} ${OBJECTDIR}/displays.o 
	@${FIXDEPS} "${OBJECTDIR}/displays.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/displays.o.d" -o ${OBJECTDIR}/displays.o displays.c    -std=gnu99
	
${OBJECTDIR}/flash.o: flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/flash.o.d 
	@${RM} ${OBJECTDIR}/flash.o 
	@${FIXDEPS} "${OBJECTDIR}/flash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/flash.o.d" -o ${OBJECTDIR}/flash.o flash.c    -std=gnu99
	
else
${OBJECTDIR}/_ext/760992031/cards.o: ../game/cards.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/cards.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/cards.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/cards.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/cards.o.d" -o ${OBJECTDIR}/_ext/760992031/cards.o ../game/cards.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/game.o: ../game/game.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/game.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/game.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/game.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/game.o.d" -o ${OBJECTDIR}/_ext/760992031/game.o ../game/game.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/gamedata.o: ../game/gamedata.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/gamedata.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/gamedata.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/gamedata.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/gamedata.o.d" -o ${OBJECTDIR}/_ext/760992031/gamedata.o ../game/gamedata.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/log.o: ../game/log.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/log.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/log.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/log.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/log.o.d" -o ${OBJECTDIR}/_ext/760992031/log.o ../game/log.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/territory.o: ../game/territory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/territory.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/territory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/territory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/territory.o.d" -o ${OBJECTDIR}/_ext/760992031/territory.o ../game/territory.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/save.o: ../game/save.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/save.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/save.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/save.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/save.o.d" -o ${OBJECTDIR}/_ext/760992031/save.o ../game/save.c    -std=gnu99
	
${OBJECTDIR}/_ext/760992031/stats.o: ../game/stats.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/760992031" 
	@${RM} ${OBJECTDIR}/_ext/760992031/stats.o.d 
	@${RM} ${OBJECTDIR}/_ext/760992031/stats.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/760992031/stats.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/_ext/760992031/stats.o.d" -o ${OBJECTDIR}/_ext/760992031/stats.o ../game/stats.c    -std=gnu99
	
${OBJECTDIR}/init.o: init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/init.o.d 
	@${RM} ${OBJECTDIR}/init.o 
	@${FIXDEPS} "${OBJECTDIR}/init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/init.o.d" -o ${OBJECTDIR}/init.o init.c    -std=gnu99
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -std=gnu99
	
${OBJECTDIR}/lcd.o: lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lcd.o.d 
	@${RM} ${OBJECTDIR}/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/lcd.o.d" -o ${OBJECTDIR}/lcd.o lcd.c    -std=gnu99
	
${OBJECTDIR}/buttons.o: buttons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/buttons.o.d 
	@${RM} ${OBJECTDIR}/buttons.o 
	@${FIXDEPS} "${OBJECTDIR}/buttons.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/buttons.o.d" -o ${OBJECTDIR}/buttons.o buttons.c    -std=gnu99
	
${OBJECTDIR}/pi.o: pi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pi.o.d 
	@${RM} ${OBJECTDIR}/pi.o 
	@${FIXDEPS} "${OBJECTDIR}/pi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/pi.o.d" -o ${OBJECTDIR}/pi.o pi.c    -std=gnu99
	
${OBJECTDIR}/displays.o: displays.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/displays.o.d 
	@${RM} ${OBJECTDIR}/displays.o 
	@${FIXDEPS} "${OBJECTDIR}/displays.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/displays.o.d" -o ${OBJECTDIR}/displays.o displays.c    -std=gnu99
	
${OBJECTDIR}/flash.o: flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/flash.o.d 
	@${RM} ${OBJECTDIR}/flash.o 
	@${FIXDEPS} "${OBJECTDIR}/flash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -DUSE_FULL_MAP -DUSE_RANDOM -MMD -MF "${OBJECTDIR}/flash.o.d" -o ${OBJECTDIR}/flash.o flash.c    -std=gnu99
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/RISK-ECM064.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    linker.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RISK-ECM064.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x27F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-G64
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/RISK-ECM064.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   linker.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/RISK-ECM064.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",-G64
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/RISK-ECM064.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
