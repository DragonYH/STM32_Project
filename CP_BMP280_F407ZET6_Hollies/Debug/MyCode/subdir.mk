################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/bmp280.c \
../MyCode/oled_spi.c 

OBJS += \
./MyCode/bmp280.o \
./MyCode/oled_spi.o 

C_DEPS += \
./MyCode/bmp280.d \
./MyCode/oled_spi.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/%.o MyCode/%.su MyCode/%.cyclo: ../MyCode/%.c MyCode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../MyCode/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MyCode

clean-MyCode:
	-$(RM) ./MyCode/bmp280.cyclo ./MyCode/bmp280.d ./MyCode/bmp280.o ./MyCode/bmp280.su ./MyCode/oled_spi.cyclo ./MyCode/oled_spi.d ./MyCode/oled_spi.o ./MyCode/oled_spi.su

.PHONY: clean-MyCode

