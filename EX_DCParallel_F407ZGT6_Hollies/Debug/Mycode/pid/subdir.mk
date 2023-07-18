################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mycode/pid/pid.c 

OBJS += \
./Mycode/pid/pid.o 

C_DEPS += \
./Mycode/pid/pid.d 


# Each subdirectory must supply rules for building sources it contributes
Mycode/pid/%.o Mycode/pid/%.su Mycode/pid/%.cyclo: ../Mycode/pid/%.c Mycode/pid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Mycode/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Mycode-2f-pid

clean-Mycode-2f-pid:
	-$(RM) ./Mycode/pid/pid.cyclo ./Mycode/pid/pid.d ./Mycode/pid/pid.o ./Mycode/pid/pid.su

.PHONY: clean-Mycode-2f-pid

