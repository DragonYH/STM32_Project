################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Src/ad7606.c \
../User/Src/ina228.c \
../User/Src/oled.c \
../User/Src/pid.c \
../User/Src/single_phrase_pll.c \
../User/Src/user_exit.c \
../User/Src/user_task.c 

OBJS += \
./User/Src/ad7606.o \
./User/Src/ina228.o \
./User/Src/oled.o \
./User/Src/pid.o \
./User/Src/single_phrase_pll.o \
./User/Src/user_exit.o \
./User/Src/user_task.o 

C_DEPS += \
./User/Src/ad7606.d \
./User/Src/ina228.d \
./User/Src/oled.d \
./User/Src/pid.d \
./User/Src/single_phrase_pll.d \
./User/Src/user_exit.d \
./User/Src/user_task.d 


# Each subdirectory must supply rules for building sources it contributes
User/Src/%.o User/Src/%.su User/Src/%.cyclo: ../User/Src/%.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DARM_MATH_CM7 -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../User/Inc -I../Drivers/CMSIS/DSP/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-Src

clean-User-2f-Src:
	-$(RM) ./User/Src/ad7606.cyclo ./User/Src/ad7606.d ./User/Src/ad7606.o ./User/Src/ad7606.su ./User/Src/ina228.cyclo ./User/Src/ina228.d ./User/Src/ina228.o ./User/Src/ina228.su ./User/Src/oled.cyclo ./User/Src/oled.d ./User/Src/oled.o ./User/Src/oled.su ./User/Src/pid.cyclo ./User/Src/pid.d ./User/Src/pid.o ./User/Src/pid.su ./User/Src/single_phrase_pll.cyclo ./User/Src/single_phrase_pll.d ./User/Src/single_phrase_pll.o ./User/Src/single_phrase_pll.su ./User/Src/user_exit.cyclo ./User/Src/user_exit.d ./User/Src/user_exit.o ./User/Src/user_exit.su ./User/Src/user_task.cyclo ./User/Src/user_task.d ./User/Src/user_task.o ./User/Src/user_task.su

.PHONY: clean-User-2f-Src

