
# Makefile for Memory Test Project
# Author : Atharva Nandanwar
# Date: 10/31/2019

#####################################################################
# Build Variables

# Program for removing files
RM := rm -rf

# Program for making directories
MK := mkdir -p

# ARM compiler
ARM_CC := arm-none-eabi-gcc

# ARM linker
ARM_LL := arm-none-eabi-gcc

#####################################################################
# ARM Compiler Flags
ARM_FLAGS := -c \
			 -std=c99 \
			 -O0 \
			 -g3 \
			 -ffunction-sections \
			 -fmessage-length=0 \
			 -fno-common \
			 -fdata-sections \
			 -fno-builtin \
			 -mcpu=cortex-m0plus \
			 -mthumb
			 
# ARM Linker Flags
ARM_LL_FLAGS := -v \
				-nostdlib \
			    -Xlinker -Map="./Debug/pes_project_4.map" \
			    -Xlinker --gc-sections \
			    -Xlinker -print-memory-usage \
			    -Xlinker --sort-section=alignment \
			    -Xlinker --cref \
			    -mcpu=cortex-m0plus \
			    -mthumb \
			    -T linkerfile.ld \
			    -o $(EXE)
			
# ARM Defines
ARM_DEFS := \
		   -D__REDLIB__ \
		   -DCPU_MKL25Z128VLK4 \
		   -DCPU_MKL25Z128VLK4_cm0plus \
		   -DSDK_OS_BAREMETAL \
		   -DFSL_RTOS_BM \
		   -DCR_INTEGER_PRINTF \
		   -DPRINTF_FLOAT_ENABLE=0 \
		   -DSCANF_FLOAT_ENABLE=0 \
		   -DPRINTF_ADVANCED_ENABLE=0 \
		   -DSCANF_ADVANCED_ENABLE=0 \
		   -D__MCUXPRESSO \
		   -D__USE_CMSIS \
		   -DDEBUG \
		   -DFRDM_KL25Z \
		   -DFREEDOM \
		   -specs=redlib.specs \
		   -DSDK_DEBUGCONSOLE=0 \
		   -DSDK_DEBUGCONSOLE_UART 
		   
# Build Folders
SOURCE := ./source
DEBUG := ./Debug

# ARM Include Files
ARM_INCS := \
		   -I"$(SOURCE)" \
		   -I"$(SOURCE)/i2c" \
		   -I"$(SOURCE)/led_control" \
		   -I"$(SOURCE)/logger" \
		   -I"$(SOURCE)/POST" \
		   -I"$(SOURCE)/state_machine" \
		   -I"$(SOURCE)/test" \
		   -I"$(SOURCE)/uctest" \
		   -I"board" \
		   -I"CMSIS" \
		   -I"drivers" \
		   -I"startup" \
		   -I"utilities" \
		   
# ARM Object Files 
ARM_OBJS := \
		   $(DEBUG)/source/i2c/i2c.o \
		   $(DEBUG)/source/i2c/tmp102.o \
		   $(DEBUG)/source/led_control/led_control.o \
		   $(DEBUG)/source/logger/logger.o \
		   $(DEBUG)/source/logger/errno.o \
		   $(DEBUG)/source/POST/post.o \
		   $(DEBUG)/source/state_machine/state_machine.o \
		   $(DEBUG)/source/uctest/System.o \
		   $(DEBUG)/startup/startup_mkl25z4.o \
		   $(DEBUG)/CMSIS/system_MKL25Z4.o \
		   $(DEBUG)/board/board.o \
		   $(DEBUG)/board/clock_config.o \
		   $(DEBUG)/board/peripherals.o \
		   $(DEBUG)/board/pin_mux.o \
		   $(DEBUG)/drivers/fsl_clock.o \
		   $(DEBUG)/drivers/fsl_common.o \
		   $(DEBUG)/drivers/fsl_flash.o \
		   $(DEBUG)/drivers/fsl_gpio.o \
		   $(DEBUG)/drivers/fsl_lpsci.o \
		   $(DEBUG)/drivers/fsl_smc.o \
		   $(DEBUG)/drivers/fsl_uart.o \
		   $(DEBUG)/utilities/fsl_debug_console.o
	
# ARM Dependencies Files	   
ARM_DEPS := \
		   $(DEBUG)/source/i2c/i2c.d \
		   $(DEBUG)/source/i2c/tmp102.d \
		   $(DEBUG)/source/led_control/led_control.d \
		   $(DEBUG)/source/logger/logger.d \
		   $(DEBUG)/source/logger/errno.d \
		   $(DEBUG)/source/POST/post.d \
		   $(DEBUG)/source/state_machine/state_machine.d \
		   $(DEBUG)/source/uctest/System.d \
		   $(DEBUG)/startup/startup_mkl25z4.d \
		   $(DEBUG)/CMSIS/system_MKL25Z4.d \
		   $(DEBUG)/board/board.d \
		   $(DEBUG)/board/clock_config.d \
		   $(DEBUG)/board/peripherals.d \
		   $(DEBUG)/board/pin_mux.d \
		   $(DEBUG)/drivers/fsl_clock.d \
		   $(DEBUG)/drivers/fsl_common.d \
		   $(DEBUG)/drivers/fsl_flash.d \
		   $(DEBUG)/drivers/fsl_gpio.d \
		   $(DEBUG)/drivers/fsl_lpsci.d \
		   $(DEBUG)/drivers/fsl_smc.d \
		   $(DEBUG)/drivers/fsl_uart.d \
		   $(DEBUG)/utilities/fsl_debug_console.d
		   
# Executable file
EXE := $(DEBUG)/pes_project_4.axf

#####################################################################
# Build Rules
# Rules for making all
all : $(EXE)

#####################################################################
# Selecting Platform
ifeq ($(BUILD), TEST)
build_option := test
else ifeq ($(BUILD), LOG)
build_option := log
else ifeq ($(BUILD), RUN)
build_option := run
endif
#####################################################################

$(EXE) : $(build_option)
	
#####################################################################
# Rule for compiling tests
test : directories $(ARM_OBJS) $(SOURCE)/test/test.c
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) ./source/test/test.c -o $(DEBUG)/source/test/test.o
	@arm-none-eabi-gcc -nostdlib -Xlinker -Map="./Debug/pes_project_4.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T linkerfile.ld -o ./Debug/pes_project_4.axf $(ARM_OBJS) $(DEBUG)/source/test/test.o
	@echo "Testing Code Compiled"

#####################################################################
# Rule for compiling detailed debug log
log : directories $(ARM_OBJS) $(SOURCE)/main.c
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -DLOG $(SOURCE)/main.c -o $(DEBUG)/source/main.o
	@arm-none-eabi-gcc -nostdlib -Xlinker -Map="./Debug/pes_project_4.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T linkerfile.ld -o ./Debug/pes_project_4.axf $(ARM_OBJS) $(DEBUG)/source/main.o
	@echo "KL25Z with logging on"

#####################################################################
# Rule for compiling program with normal logging	
run : directories $(ARM_OBJS) $(SOURCE)/main.c
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -DRUN $(SOURCE)/main.c -o $(DEBUG)/source/main.o
	@arm-none-eabi-gcc -nostdlib -Xlinker -Map="./Debug/pes_project_4.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T linkerfile.ld -o ./Debug/pes_project_4.axf $(ARM_OBJS) $(DEBUG)/source/main.o
	@echo "KL25Z with logging off"

#####################################################################
# Essesntial Source Files
$(DEBUG)/source/i2c/%.o: ./source/i2c/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
$(DEBUG)/source/led_control/%.o: ./source/led_control/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/source/logger/%.o: ./source/logger/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
$(DEBUG)/source/POST/%.o: ./source/POST/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/source/state_machine/%.o: ./source/state_machine/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/source/uctest/%.o: ./source/uctest/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

#####################################################################
# Essesntial ARM Object Files
$(DEBUG)/board/%.o: ./board/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/CMSIS/%.o: ./CMSIS/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(DEBUG)/drivers/%.o: ./drivers/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
$(DEBUG)/startup/%.o: ./startup/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
$(DEBUG)/utilities/%.o: ./utilities/%.c
	@echo 'Building file: $<'
	@$(ARM_CC) $(ARM_FLAGS) $(ARM_DEFS) $(ARM_INCS) -MMD -MP -MF"./$(@:%.o=%.d)" -MT"./$(@:%.o=%.o)" -MT"./$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
	
#####################################################################
# Making directories
.PHONY : directories
directories :
	$(MK) \
	$(DEBUG) \
	$(DEBUG)/board \
	$(DEBUG)/CMSIS \
	$(DEBUG)/drivers \
	$(DEBUG)/startup \
	$(DEBUG)/utilities \
	$(DEBUG)/source/i2c \
	$(DEBUG)/source/led_control \
	$(DEBUG)/source/logger \
	$(DEBUG)/source/POST \
	$(DEBUG)/source/state_machine \
	$(DEBUG)/source/test \
	$(DEBUG)/source/uctest \
	
# Clean target
clean:
	@$(RM) \
	$(DEBUG)/board \
	$(DEBUG)/CMSIS \
	$(DEBUG)/drivers \
	$(DEBUG)/startup \
	$(DEBUG)/utilities \
	$(DEBUG)/source \
	$(DEBUG)/pes_project_4.axf \
	$(DEBUG)/pes_project_4.map
	@echo "Build cleaned"
	