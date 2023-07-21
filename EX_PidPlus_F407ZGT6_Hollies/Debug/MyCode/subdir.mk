################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/ina238.c \
../MyCode/oled_spi.c \
../MyCode/pidPlus.c 

OBJS += \
./MyCode/ina238.o \
./MyCode/oled_spi.o \
./MyCode/pidPlus.o 

C_DEPS += \
./MyCode/ina238.d \
./MyCode/oled_spi.d \
./MyCode/pidPlus.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/%.o MyCode/%.su MyCode/%.cyclo: ../MyCode/%.c MyCode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../MyCode/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MyCode

clean-MyCode:
	-$(RM) ./MyCode/ina238.cyclo ./MyCode/ina238.d ./MyCode/ina238.o ./MyCode/ina238.su ./MyCode/oled_spi.cyclo ./MyCode/oled_spi.d ./MyCode/oled_spi.o ./MyCode/oled_spi.su ./MyCode/pidPlus.cyclo ./MyCode/pidPlus.d ./MyCode/pidPlus.o ./MyCode/pidPlus.su

.PHONY: clean-MyCode

