################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/TFTLCD/tftlcd.c 

OBJS += \
./MyCode/TFTLCD/tftlcd.o 

C_DEPS += \
./MyCode/TFTLCD/tftlcd.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/TFTLCD/%.o MyCode/TFTLCD/%.su MyCode/TFTLCD/%.cyclo: ../MyCode/TFTLCD/%.c MyCode/TFTLCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../MyCode/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MyCode-2f-TFTLCD

clean-MyCode-2f-TFTLCD:
	-$(RM) ./MyCode/TFTLCD/tftlcd.cyclo ./MyCode/TFTLCD/tftlcd.d ./MyCode/TFTLCD/tftlcd.o ./MyCode/TFTLCD/tftlcd.su

.PHONY: clean-MyCode-2f-TFTLCD

