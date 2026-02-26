################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FEE/eeprom.c 

OBJS += \
./FEE/eeprom.o 

C_DEPS += \
./FEE/eeprom.d 


# Each subdirectory must supply rules for building sources it contributes
FEE/%.o FEE/%.su FEE/%.cyclo: ../FEE/%.c FEE/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-FEE

clean-FEE:
	-$(RM) ./FEE/eeprom.cyclo ./FEE/eeprom.d ./FEE/eeprom.o ./FEE/eeprom.su

.PHONY: clean-FEE

