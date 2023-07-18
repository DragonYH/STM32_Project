################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/keyboard/keyboard.c 

OBJS += \
./MyCode/keyboard/keyboard.o 

C_DEPS += \
./MyCode/keyboard/keyboard.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/keyboard/%.o MyCode/keyboard/%.su MyCode/keyboard/%.cyclo: ../MyCode/keyboard/%.c MyCode/keyboard/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/Document/STM32_Project/CP_ThreePortDCDC_F103ZET6_Hollies_Schoo/MyCode/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyCode-2f-keyboard

clean-MyCode-2f-keyboard:
	-$(RM) ./MyCode/keyboard/keyboard.cyclo ./MyCode/keyboard/keyboard.d ./MyCode/keyboard/keyboard.o ./MyCode/keyboard/keyboard.su

.PHONY: clean-MyCode-2f-keyboard

