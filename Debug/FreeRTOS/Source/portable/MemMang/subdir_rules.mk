################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/MemMang/heap_1.obj: ../FreeRTOS/Source/portable/MemMang/heap_1.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_16.9.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -Ooff --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_16.9.7.LTS/include" --include_path="C:/Users/Ravi/ccs_workspace_v8/prob4" --include_path="C:/Users/Ravi/ccs_workspace_v8/prob4/FreeRTOS/Source/include" --include_path="C:/Users/Ravi/ccs_workspace_v8/prob4/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/ti/TivaWare_C_Series-2.1.3.156" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C1294NCPDT --define=TARGET_IS_TM4C129_RA1 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="FreeRTOS/Source/portable/MemMang/heap_1.d_raw" --obj_directory="FreeRTOS/Source/portable/MemMang" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


