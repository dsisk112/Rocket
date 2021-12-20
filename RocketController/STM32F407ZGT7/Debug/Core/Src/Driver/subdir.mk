################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Driver/lsm6ds3.c 

C_DEPS += \
./Core/Src/Driver/lsm6ds3.d 

OBJS += \
./Core/Src/Driver/lsm6ds3.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Driver/%.o: ../Core/Src/Driver/%.c Core/Src/Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Driver

clean-Core-2f-Src-2f-Driver:
	-$(RM) ./Core/Src/Driver/lsm6ds3.d ./Core/Src/Driver/lsm6ds3.o

.PHONY: clean-Core-2f-Src-2f-Driver

