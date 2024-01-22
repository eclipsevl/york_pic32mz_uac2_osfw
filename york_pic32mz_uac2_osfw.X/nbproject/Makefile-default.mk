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
FINAL_IMAGE=${DISTDIR}/york_pic32mz_uac2_osfw.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/york_pic32mz_uac2_osfw.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=src/app/main.c src/app/system.c src/drivers/usb/usb_LL.c src/drivers/usb/usb_PL.c src/drivers/usb/usb_descriptors.c src/drivers/i2s/i2s.c src/drivers/port/port.c src/drivers/tmr/tmr.c src/drivers/dma/dma.c src/drivers/clk/clk.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/app/main.o ${OBJECTDIR}/src/app/system.o ${OBJECTDIR}/src/drivers/usb/usb_LL.o ${OBJECTDIR}/src/drivers/usb/usb_PL.o ${OBJECTDIR}/src/drivers/usb/usb_descriptors.o ${OBJECTDIR}/src/drivers/i2s/i2s.o ${OBJECTDIR}/src/drivers/port/port.o ${OBJECTDIR}/src/drivers/tmr/tmr.o ${OBJECTDIR}/src/drivers/dma/dma.o ${OBJECTDIR}/src/drivers/clk/clk.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/app/main.o.d ${OBJECTDIR}/src/app/system.o.d ${OBJECTDIR}/src/drivers/usb/usb_LL.o.d ${OBJECTDIR}/src/drivers/usb/usb_PL.o.d ${OBJECTDIR}/src/drivers/usb/usb_descriptors.o.d ${OBJECTDIR}/src/drivers/i2s/i2s.o.d ${OBJECTDIR}/src/drivers/port/port.o.d ${OBJECTDIR}/src/drivers/tmr/tmr.o.d ${OBJECTDIR}/src/drivers/dma/dma.o.d ${OBJECTDIR}/src/drivers/clk/clk.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/app/main.o ${OBJECTDIR}/src/app/system.o ${OBJECTDIR}/src/drivers/usb/usb_LL.o ${OBJECTDIR}/src/drivers/usb/usb_PL.o ${OBJECTDIR}/src/drivers/usb/usb_descriptors.o ${OBJECTDIR}/src/drivers/i2s/i2s.o ${OBJECTDIR}/src/drivers/port/port.o ${OBJECTDIR}/src/drivers/tmr/tmr.o ${OBJECTDIR}/src/drivers/dma/dma.o ${OBJECTDIR}/src/drivers/clk/clk.o

# Source Files
SOURCEFILES=src/app/main.c src/app/system.c src/drivers/usb/usb_LL.c src/drivers/usb/usb_PL.c src/drivers/usb/usb_descriptors.c src/drivers/i2s/i2s.c src/drivers/port/port.c src/drivers/tmr/tmr.c src/drivers/dma/dma.c src/drivers/clk/clk.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/york_pic32mz_uac2_osfw.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ0512EFE064
MP_LINKER_FILE_OPTION=
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
${OBJECTDIR}/src/app/main.o: src/app/main.c  .generated_files/flags/default/a41b93583ead11d5589af995ee081678615e0d0a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/app" 
	@${RM} ${OBJECTDIR}/src/app/main.o.d 
	@${RM} ${OBJECTDIR}/src/app/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/app/main.o.d" -o ${OBJECTDIR}/src/app/main.o src/app/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/app/system.o: src/app/system.c  .generated_files/flags/default/6c2182e149affe69eb975629a7e18b7ba8b59854 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/app" 
	@${RM} ${OBJECTDIR}/src/app/system.o.d 
	@${RM} ${OBJECTDIR}/src/app/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/app/system.o.d" -o ${OBJECTDIR}/src/app/system.o src/app/system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/usb/usb_LL.o: src/drivers/usb/usb_LL.c  .generated_files/flags/default/f3e97b0ba1fa9ff2465a7d17453818baa8b34e41 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/usb" 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_LL.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_LL.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/usb/usb_LL.o.d" -o ${OBJECTDIR}/src/drivers/usb/usb_LL.o src/drivers/usb/usb_LL.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/usb/usb_PL.o: src/drivers/usb/usb_PL.c  .generated_files/flags/default/fedcc1af4d8284ddec18f02f83d1070183cca229 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/usb" 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_PL.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_PL.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/usb/usb_PL.o.d" -o ${OBJECTDIR}/src/drivers/usb/usb_PL.o src/drivers/usb/usb_PL.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/usb/usb_descriptors.o: src/drivers/usb/usb_descriptors.c  .generated_files/flags/default/50fd7862ce8dfeec8e69a014783860d4357f7d33 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/usb" 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/usb/usb_descriptors.o.d" -o ${OBJECTDIR}/src/drivers/usb/usb_descriptors.o src/drivers/usb/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/i2s/i2s.o: src/drivers/i2s/i2s.c  .generated_files/flags/default/a6ee06a1cb53796622e57ea543d6f623234ce03b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/i2s" 
	@${RM} ${OBJECTDIR}/src/drivers/i2s/i2s.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/i2s/i2s.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/i2s/i2s.o.d" -o ${OBJECTDIR}/src/drivers/i2s/i2s.o src/drivers/i2s/i2s.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/port/port.o: src/drivers/port/port.c  .generated_files/flags/default/6f7c3a8262fcb88a6bc27a6bd08ebff8bbfc9b93 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/port" 
	@${RM} ${OBJECTDIR}/src/drivers/port/port.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/port/port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/port/port.o.d" -o ${OBJECTDIR}/src/drivers/port/port.o src/drivers/port/port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/tmr/tmr.o: src/drivers/tmr/tmr.c  .generated_files/flags/default/668732ca58a40a90a47007d2f6cfdd546e30c689 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/tmr" 
	@${RM} ${OBJECTDIR}/src/drivers/tmr/tmr.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/tmr/tmr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/tmr/tmr.o.d" -o ${OBJECTDIR}/src/drivers/tmr/tmr.o src/drivers/tmr/tmr.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/dma/dma.o: src/drivers/dma/dma.c  .generated_files/flags/default/909c351d3cefb7f750b69e39ee9c484d4d9a74f1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/dma" 
	@${RM} ${OBJECTDIR}/src/drivers/dma/dma.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/dma/dma.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/dma/dma.o.d" -o ${OBJECTDIR}/src/drivers/dma/dma.o src/drivers/dma/dma.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/clk/clk.o: src/drivers/clk/clk.c  .generated_files/flags/default/5aabc30b847e39b6fd51f9700c7e51b859179c04 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/clk" 
	@${RM} ${OBJECTDIR}/src/drivers/clk/clk.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/clk/clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/clk/clk.o.d" -o ${OBJECTDIR}/src/drivers/clk/clk.o src/drivers/clk/clk.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/src/app/main.o: src/app/main.c  .generated_files/flags/default/ee30e935ebc80adc19905f7849ea6411e6d3c4dc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/app" 
	@${RM} ${OBJECTDIR}/src/app/main.o.d 
	@${RM} ${OBJECTDIR}/src/app/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/app/main.o.d" -o ${OBJECTDIR}/src/app/main.o src/app/main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/app/system.o: src/app/system.c  .generated_files/flags/default/158199c93950dd789ca718a6bd125eec4f1b8515 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/app" 
	@${RM} ${OBJECTDIR}/src/app/system.o.d 
	@${RM} ${OBJECTDIR}/src/app/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/app/system.o.d" -o ${OBJECTDIR}/src/app/system.o src/app/system.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/usb/usb_LL.o: src/drivers/usb/usb_LL.c  .generated_files/flags/default/134d9919d287957b829d8ccad083d28273965492 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/usb" 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_LL.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_LL.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/usb/usb_LL.o.d" -o ${OBJECTDIR}/src/drivers/usb/usb_LL.o src/drivers/usb/usb_LL.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/usb/usb_PL.o: src/drivers/usb/usb_PL.c  .generated_files/flags/default/4bf74c43281db0bdd482e76f9cab9f7dac2f0193 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/usb" 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_PL.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_PL.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/usb/usb_PL.o.d" -o ${OBJECTDIR}/src/drivers/usb/usb_PL.o src/drivers/usb/usb_PL.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/usb/usb_descriptors.o: src/drivers/usb/usb_descriptors.c  .generated_files/flags/default/86326c8d0e854d4c5217a500718d5c751f88eb48 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/usb" 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/usb/usb_descriptors.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/usb/usb_descriptors.o.d" -o ${OBJECTDIR}/src/drivers/usb/usb_descriptors.o src/drivers/usb/usb_descriptors.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/i2s/i2s.o: src/drivers/i2s/i2s.c  .generated_files/flags/default/d493a6b6d1bf8c66cff39d9c387848140528d26f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/i2s" 
	@${RM} ${OBJECTDIR}/src/drivers/i2s/i2s.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/i2s/i2s.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/i2s/i2s.o.d" -o ${OBJECTDIR}/src/drivers/i2s/i2s.o src/drivers/i2s/i2s.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/port/port.o: src/drivers/port/port.c  .generated_files/flags/default/26e74579ba255f729a69e6b85aa38d5621cb0ad9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/port" 
	@${RM} ${OBJECTDIR}/src/drivers/port/port.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/port/port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/port/port.o.d" -o ${OBJECTDIR}/src/drivers/port/port.o src/drivers/port/port.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/tmr/tmr.o: src/drivers/tmr/tmr.c  .generated_files/flags/default/af73f907d7fc0262f41691c5f9e9220973db96e6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/tmr" 
	@${RM} ${OBJECTDIR}/src/drivers/tmr/tmr.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/tmr/tmr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/tmr/tmr.o.d" -o ${OBJECTDIR}/src/drivers/tmr/tmr.o src/drivers/tmr/tmr.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/dma/dma.o: src/drivers/dma/dma.c  .generated_files/flags/default/73552f106381a8c1d1cc2e3addfee1c348dda7d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/dma" 
	@${RM} ${OBJECTDIR}/src/drivers/dma/dma.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/dma/dma.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/dma/dma.o.d" -o ${OBJECTDIR}/src/drivers/dma/dma.o src/drivers/dma/dma.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/src/drivers/clk/clk.o: src/drivers/clk/clk.c  .generated_files/flags/default/5470621a75eab63f6f4bababe81ecaf453cb0110 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/drivers/clk" 
	@${RM} ${OBJECTDIR}/src/drivers/clk/clk.o.d 
	@${RM} ${OBJECTDIR}/src/drivers/clk/clk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O2 -MP -MMD -MF "${OBJECTDIR}/src/drivers/clk/clk.o.d" -o ${OBJECTDIR}/src/drivers/clk/clk.o src/drivers/clk/clk.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/york_pic32mz_uac2_osfw.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/york_pic32mz_uac2_osfw.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x37F   -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=4096,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/york_pic32mz_uac2_osfw.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/york_pic32mz_uac2_osfw.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=4096,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/york_pic32mz_uac2_osfw.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
