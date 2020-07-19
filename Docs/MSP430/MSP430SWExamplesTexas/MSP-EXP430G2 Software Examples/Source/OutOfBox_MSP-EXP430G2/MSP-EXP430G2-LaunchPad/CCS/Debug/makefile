################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := 

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: MSP-EXP430G2-Launchpad.out

# Tool invocations
MSP-EXP430G2-Launchpad.out: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	-m"MSP-EXP430G2-Launchpad.map" --stack_size=50 --heap_size=0 -i"/msp430/include" -i"/lib" -i"/include"  "MSP-EXP430G2-Launchpad.out" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) MSP-EXP430G2-Launchpad.out
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
