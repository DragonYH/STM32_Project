################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mycode/oled_spi/oled_spi.c 

OBJS += \
./Mycode/oled_spi/oled_spi.o 

C_DEPS += \
./Mycode/oled_spi/oled_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Mycode/oled_spi/%.o Mycode/oled_spi/%.su Mycode/oled_spi/%.cyclo: ../Mycode/oled_spi/%.c Mycode/oled_spi/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Mycode/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Mycode-2f-oled_spi

clean-Mycode-2f-oled_spi:
	-$(RM) ./Mycode/oled_spi/oled_spi.cyclo ./Mycode/oled_spi/oled_spi.d ./Mycode/oled_spi/oled_spi.o ./Mycode/oled_spi/oled_spi.su

.PHONY: clean-Mycode-2f-oled_spi

