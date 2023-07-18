################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/pid/pid.c 

OBJS += \
./MyCode/pid/pid.o 

C_DEPS += \
./MyCode/pid/pid.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/pid/%.o MyCode/pid/%.su MyCode/pid/%.cyclo: ../MyCode/pid/%.c MyCode/pid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"E:/Document/STM32_Project/CP_ThreePortDCDC_F103ZET6_Hollies_Schoo/MyCode/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyCode-2f-pid

clean-MyCode-2f-pid:
	-$(RM) ./MyCode/pid/pid.cyclo ./MyCode/pid/pid.d ./MyCode/pid/pid.o ./MyCode/pid/pid.su

.PHONY: clean-MyCode-2f-pid

