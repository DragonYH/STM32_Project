################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mycode/bc26.c 

OBJS += \
./Mycode/bc26.o 

C_DEPS += \
./Mycode/bc26.d 


# Each subdirectory must supply rules for building sources it contributes
Mycode/%.o Mycode/%.su Mycode/%.cyclo: ../Mycode/%.c Mycode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Mycode/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Mycode

clean-Mycode:
	-$(RM) ./Mycode/bc26.cyclo ./Mycode/bc26.d ./Mycode/bc26.o ./Mycode/bc26.su

.PHONY: clean-Mycode

