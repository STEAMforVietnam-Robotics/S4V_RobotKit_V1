################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MIDWR/FEE/FEE.c 

OBJS += \
./MIDWR/FEE/FEE.o 

C_DEPS += \
./MIDWR/FEE/FEE.d 


# Each subdirectory must supply rules for building sources it contributes
MIDWR/FEE/%.o MIDWR/FEE/%.su MIDWR/FEE/%.cyclo: ../MIDWR/FEE/%.c MIDWR/FEE/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MIDWR-2f-FEE

clean-MIDWR-2f-FEE:
	-$(RM) ./MIDWR/FEE/FEE.cyclo ./MIDWR/FEE/FEE.d ./MIDWR/FEE/FEE.o ./MIDWR/FEE/FEE.su

.PHONY: clean-MIDWR-2f-FEE

