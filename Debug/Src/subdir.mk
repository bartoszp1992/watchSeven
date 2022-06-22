################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/7seg.c \
../Src/adc.c \
../Src/backup.c \
../Src/bme280.c \
../Src/eeprom_emul.c \
../Src/flash_interface.c \
../Src/interface.c \
../Src/interrupts.c \
../Src/lis3mdl.c \
../Src/main.c \
../Src/menu.c \
../Src/rtc.c \
../Src/stm32g0xx_hal_msp.c \
../Src/stm32g0xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32g0xx.c 

OBJS += \
./Src/7seg.o \
./Src/adc.o \
./Src/backup.o \
./Src/bme280.o \
./Src/eeprom_emul.o \
./Src/flash_interface.o \
./Src/interface.o \
./Src/interrupts.o \
./Src/lis3mdl.o \
./Src/main.o \
./Src/menu.o \
./Src/rtc.o \
./Src/stm32g0xx_hal_msp.o \
./Src/stm32g0xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32g0xx.o 

C_DEPS += \
./Src/7seg.d \
./Src/adc.d \
./Src/backup.d \
./Src/bme280.d \
./Src/eeprom_emul.d \
./Src/flash_interface.d \
./Src/interface.d \
./Src/interrupts.d \
./Src/lis3mdl.d \
./Src/main.d \
./Src/menu.d \
./Src/rtc.d \
./Src/stm32g0xx_hal_msp.d \
./Src/stm32g0xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32g0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G071xx -DUSE_FULL_LL_DRIVER -c -I../Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/7seg.d ./Src/7seg.o ./Src/adc.d ./Src/adc.o ./Src/backup.d ./Src/backup.o ./Src/bme280.d ./Src/bme280.o ./Src/eeprom_emul.d ./Src/eeprom_emul.o ./Src/flash_interface.d ./Src/flash_interface.o ./Src/interface.d ./Src/interface.o ./Src/interrupts.d ./Src/interrupts.o ./Src/lis3mdl.d ./Src/lis3mdl.o ./Src/main.d ./Src/main.o ./Src/menu.d ./Src/menu.o ./Src/rtc.d ./Src/rtc.o ./Src/stm32g0xx_hal_msp.d ./Src/stm32g0xx_hal_msp.o ./Src/stm32g0xx_it.d ./Src/stm32g0xx_it.o ./Src/syscalls.d ./Src/syscalls.o ./Src/sysmem.d ./Src/sysmem.o ./Src/system_stm32g0xx.d ./Src/system_stm32g0xx.o

.PHONY: clean-Src

