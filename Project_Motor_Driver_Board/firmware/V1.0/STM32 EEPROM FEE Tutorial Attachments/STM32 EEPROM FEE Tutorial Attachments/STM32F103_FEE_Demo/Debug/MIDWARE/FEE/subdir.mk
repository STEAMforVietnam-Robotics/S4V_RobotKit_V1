################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MIDWARE/FEE/FEE.c 

OBJS += \
./MIDWARE/FEE/FEE.o 

C_DEPS += \
./MIDWARE/FEE/FEE.d 


# Each subdirectory must supply rules for building sources it contributes
MIDWARE/FEE/%.o MIDWARE/FEE/%.su MIDWARE/FEE/%.cyclo: ../MIDWARE/FEE/%.c MIDWARE/FEE/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MIDWARE-2f-FEE

clean-MIDWARE-2f-FEE:
	-$(RM) ./MIDWARE/FEE/FEE.cyclo ./MIDWARE/FEE/FEE.d ./MIDWARE/FEE/FEE.o ./MIDWARE/FEE/FEE.su

.PHONY: clean-MIDWARE-2f-FEE

