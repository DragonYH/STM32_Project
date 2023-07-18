################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mycode/ina238/ina238.c 

OBJS += \
./Mycode/ina238/ina238.o 

C_DEPS += \
./Mycode/ina238/ina238.d 


# Each subdirectory must supply rules for building sources it contributes
Mycode/ina238/%.o Mycode/ina238/%.su Mycode/ina238/%.cyclo: ../Mycode/ina238/%.c Mycode/ina238/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Document/STM32_Project/EX_INA238_F407ZGT6_Hollies/Mycode/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Mycode-2f-ina238

clean-Mycode-2f-ina238:
	-$(RM) ./Mycode/ina238/ina238.cyclo ./Mycode/ina238/ina238.d ./Mycode/ina238/ina238.o ./Mycode/ina238/ina238.su

.PHONY: clean-Mycode-2f-ina238

