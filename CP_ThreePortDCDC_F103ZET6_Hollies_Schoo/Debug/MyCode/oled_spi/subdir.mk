################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/oled_spi/oled_spi.c 

OBJS += \
./MyCode/oled_spi/oled_spi.o 

C_DEPS += \
./MyCode/oled_spi/oled_spi.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/oled_spi/%.o MyCode/oled_spi/%.su MyCode/oled_spi/%.cyclo: ../MyCode/oled_spi/%.c MyCode/oled_spi/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/Document/STM32_Project/CP_ThreePortDCDC_F103ZET6_Hollies_Schoo/MyCode/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyCode-2f-oled_spi

clean-MyCode-2f-oled_spi:
	-$(RM) ./MyCode/oled_spi/oled_spi.cyclo ./MyCode/oled_spi/oled_spi.d ./MyCode/oled_spi/oled_spi.o ./MyCode/oled_spi/oled_spi.su

.PHONY: clean-MyCode-2f-oled_spi

