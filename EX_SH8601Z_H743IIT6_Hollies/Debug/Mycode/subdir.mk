################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mycode/amoled.c \
../Mycode/inter_temp.c 

OBJS += \
./Mycode/amoled.o \
./Mycode/inter_temp.o 

C_DEPS += \
./Mycode/amoled.d \
./Mycode/inter_temp.d 


# Each subdirectory must supply rules for building sources it contributes
Mycode/%.o Mycode/%.su Mycode/%.cyclo: ../Mycode/%.c Mycode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Mycode/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Mycode

clean-Mycode:
	-$(RM) ./Mycode/amoled.cyclo ./Mycode/amoled.d ./Mycode/amoled.o ./Mycode/amoled.su ./Mycode/inter_temp.cyclo ./Mycode/inter_temp.d ./Mycode/inter_temp.o ./Mycode/inter_temp.su

.PHONY: clean-Mycode

