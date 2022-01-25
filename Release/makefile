################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include PINS/subdir.mk
-include MK_USART/subdir.mk
-include FONTS/subdir.mk
-include DISPLAY/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
display_new.lss \

FLASH_IMAGE += \
display_new.hex \

EEPROM_IMAGE += \
display_new.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: display_new.elf secondary-outputs

# Tool invocations
display_new.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,display_new.map -mmcu=atmega8 -o "display_new.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

display_new.lss: display_new.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S display_new.elf  >"display_new.lss"
	@echo 'Finished building: $@'
	@echo ' '

display_new.hex: display_new.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex display_new.elf  "display_new.hex"
	@echo 'Finished building: $@'
	@echo ' '

display_new.eep: display_new.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex display_new.elf  "display_new.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: display_new.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 display_new.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) display_new.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
