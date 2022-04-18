################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Menu/menu.c 

OBJS += \
./Src/Menu/menu.o 

C_DEPS += \
./Src/Menu/menu.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Menu/%.o: ../Src/Menu/%.c Src/Menu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_HAL_DRIVER -DSTM32G071xx -c -I../Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-Menu

clean-Src-2f-Menu:
	-$(RM) ./Src/Menu/menu.d ./Src/Menu/menu.o

.PHONY: clean-Src-2f-Menu

