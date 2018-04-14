################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv8/tools/compiler/ti-cgt-arm_16.9.7.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./callbacks.obj" \
"./main.obj" \
"./startup_ccs.obj" \
"./FreeRTOS/Source/croutine.obj" \
"./FreeRTOS/Source/event_groups.obj" \
"./FreeRTOS/Source/list.obj" \
"./FreeRTOS/Source/queue.obj" \
"./FreeRTOS/Source/stream_buffer.obj" \
"./FreeRTOS/Source/tasks.obj" \
"./FreeRTOS/Source/timers.obj" \
"./FreeRTOS/Source/portable/CCS/ARM_CM4F/port.obj" \
"./FreeRTOS/Source/portable/CCS/ARM_CM4F/portasm.obj" \
"./FreeRTOS/Source/portable/Common/mpu_wrappers.obj" \
"./FreeRTOS/Source/portable/MemMang/heap_1.obj" \
"../blinky_ccs.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/ti/TivaWare_C_Series-2.1.3.156/driverlib/ccs/Debug/driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include FreeRTOS/Source/subdir_vars.mk
-include FreeRTOS/Source/portable/CCS/ARM_CM4F/subdir_vars.mk
-include FreeRTOS/Source/portable/Common/subdir_vars.mk
-include FreeRTOS/Source/portable/MemMang/subdir_vars.mk
-include subdir_rules.mk
-include FreeRTOS/Source/subdir_rules.mk
-include FreeRTOS/Source/portable/CCS/ARM_CM4F/subdir_rules.mk
-include FreeRTOS/Source/portable/Common/subdir_rules.mk
-include FreeRTOS/Source/portable/MemMang/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
prob4.out \

EXE_OUTPUTS__QUOTED += \
"prob4.out" \

BIN_OUTPUTS += \
prob4.bin \

BIN_OUTPUTS__QUOTED += \
"prob4.bin" \


# All Target
all: prob4.out

# Tool invocations
prob4.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_16.9.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual -z -m"blinky_ccs.map" --stack_size=512 --heap_size=0 -i"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_16.9.7.LTS/lib" -i"C:/Users/Ravi/ccs_workspace_v8/prob4/FreeRTOS/Source/portable/CCS/ARM_CM4F" -i"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_16.9.7.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="prob4_linkInfo.xml" --rom_model -o "prob4.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

prob4.bin: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_16.9.7.LTS/bin/armhex"  -o "prob4.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "callbacks.obj" "main.obj" "startup_ccs.obj" "FreeRTOS\Source\croutine.obj" "FreeRTOS\Source\event_groups.obj" "FreeRTOS\Source\list.obj" "FreeRTOS\Source\queue.obj" "FreeRTOS\Source\stream_buffer.obj" "FreeRTOS\Source\tasks.obj" "FreeRTOS\Source\timers.obj" "FreeRTOS\Source\portable\CCS\ARM_CM4F\port.obj" "FreeRTOS\Source\portable\CCS\ARM_CM4F\portasm.obj" "FreeRTOS\Source\portable\Common\mpu_wrappers.obj" "FreeRTOS\Source\portable\MemMang\heap_1.obj" 
	-$(RM) "callbacks.d" "main.d" "startup_ccs.d" "FreeRTOS\Source\croutine.d" "FreeRTOS\Source\event_groups.d" "FreeRTOS\Source\list.d" "FreeRTOS\Source\queue.d" "FreeRTOS\Source\stream_buffer.d" "FreeRTOS\Source\tasks.d" "FreeRTOS\Source\timers.d" "FreeRTOS\Source\portable\CCS\ARM_CM4F\port.d" "FreeRTOS\Source\portable\Common\mpu_wrappers.d" "FreeRTOS\Source\portable\MemMang\heap_1.d" 
	-$(RM) "FreeRTOS\Source\portable\CCS\ARM_CM4F\portasm.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"C:/ti/ccsv8/utils/tiobj2bin/tiobj2bin" "prob4.out" "prob4.bin" "C:/ti/ccsv8/tools/compiler/ti-cgt-arm_16.9.7.LTS/bin/armofd" "C:/ti/ccsv8/tools/compiler/ti-cgt-arm_16.9.7.LTS/bin/armhex" "C:/ti/ccsv8/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

