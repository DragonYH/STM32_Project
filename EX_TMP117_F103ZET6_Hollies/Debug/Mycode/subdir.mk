################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mycode/bmp280.c \
../Mycode/oled_spi.c \
../Mycode/tmp117.c 

OBJS += \
./Mycode/bmp280.o \
./Mycode/oled_spi.o \
./Mycode/tmp117.o 

C_DEPS += \
./Mycode/bmp280.d \
./Mycode/oled_spi.d \
./Mycode/tmp117.d 


# Each subdirectory must supply rules for building sources it contributes
Mycode/%.o Mycode/%.su Mycode/%.cyclo: ../Mycode/%.c Mycode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Mycode/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Mycode

clean-Mycode:
	-$(RM) ./Mycode/bmp280.cyclo ./Mycode/bmp280.d ./Mycode/bmp280.o ./Mycode/bmp280.su ./Mycode/oled_spi.cyclo ./Mycode/oled_spi.d ./Mycode/oled_spi.o ./Mycode/oled_spi.su ./Mycode/tmp117.cyclo ./Mycode/tmp117.d ./Mycode/tmp117.o ./Mycode/tmp117.su

.PHONY: clean-Mycode

