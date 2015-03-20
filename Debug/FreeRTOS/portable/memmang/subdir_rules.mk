################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/memmang/heap_1.obj: ../FreeRTOS/portable/memmang/heap_1.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/Code Composer Studio/ccsv6/tools/compiler/msp430_4.3.3/bin/cl430" -vmsp --abi=eabi --include_path="D:/Code Composer Studio/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/d.kisel.NETWORK/Documents/CCS Workspace/Emmu Remote cpp/FreeRTOS" --include_path="C:/Users/d.kisel.NETWORK/Documents/CCS Workspace/Emmu Remote cpp/FreeRTOS/include" --include_path="C:/Users/d.kisel.NETWORK/Documents/CCS Workspace/Emmu Remote cpp/FreeRTOS/portable" --include_path="C:/Users/d.kisel.NETWORK/Documents/CCS Workspace/Emmu Remote cpp/FreeRTOS/portable/memmang" --include_path="D:/Code Composer Studio/ccsv6/tools/compiler/msp430_4.3.3/include" --advice:power=all -g --define=__MSP430F2272__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="FreeRTOS/portable/memmang/heap_1.pp" --obj_directory="FreeRTOS/portable/memmang" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


