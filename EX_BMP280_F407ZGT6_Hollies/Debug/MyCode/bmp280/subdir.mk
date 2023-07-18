################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/bmp280/bmp280.c 

OBJS += \
./MyCode/bmp280/bmp280.o 

C_DEPS += \
./MyCode/bmp280/bmp280.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/bmp280/%.o MyCode/bmp280/%.su MyCode/bmp280/%.cyclo: ../MyCode/bmp280/%.c MyCode/bmp280/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Document/STM32_Project/EX_BMP280_F407ZGT6_Hollies/MyCode/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MyCode-2f-bmp280

clean-MyCode-2f-bmp280:
	-$(RM) ./MyCode/bmp280/bmp280.cyclo ./MyCode/bmp280/bmp280.d ./MyCode/bmp280/bmp280.o ./MyCode/bmp280/bmp280.su

.PHONY: clean-MyCode-2f-bmp280

