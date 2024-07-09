################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Src/ad7606.c \
../User/Src/iir.c \
../User/Src/ina228.c \
../User/Src/oled.c \
../User/Src/pid.c \
../User/Src/svpwm.c \
../User/Src/three_phrase_pll.c \
../User/Src/user_exit.c \
../User/Src/user_global.c 

OBJS += \
./User/Src/ad7606.o \
./User/Src/iir.o \
./User/Src/ina228.o \
./User/Src/oled.o \
./User/Src/pid.o \
./User/Src/svpwm.o \
./User/Src/three_phrase_pll.o \
./User/Src/user_exit.o \
./User/Src/user_global.o 

C_DEPS += \
./User/Src/ad7606.d \
./User/Src/iir.d \
./User/Src/ina228.d \
./User/Src/oled.d \
./User/Src/pid.d \
./User/Src/svpwm.d \
./User/Src/three_phrase_pll.d \
./User/Src/user_exit.d \
./User/Src/user_global.d 


# Each subdirectory must supply rules for building sources it contributes
User/Src/%.o User/Src/%.su User/Src/%.cyclo: ../User/Src/%.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM7 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../User/Inc -I../Drivers/CMSIS/DSP/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-Src

clean-User-2f-Src:
	-$(RM) ./User/Src/ad7606.cyclo ./User/Src/ad7606.d ./User/Src/ad7606.o ./User/Src/ad7606.su ./User/Src/iir.cyclo ./User/Src/iir.d ./User/Src/iir.o ./User/Src/iir.su ./User/Src/ina228.cyclo ./User/Src/ina228.d ./User/Src/ina228.o ./User/Src/ina228.su ./User/Src/oled.cyclo ./User/Src/oled.d ./User/Src/oled.o ./User/Src/oled.su ./User/Src/pid.cyclo ./User/Src/pid.d ./User/Src/pid.o ./User/Src/pid.su ./User/Src/svpwm.cyclo ./User/Src/svpwm.d ./User/Src/svpwm.o ./User/Src/svpwm.su ./User/Src/three_phrase_pll.cyclo ./User/Src/three_phrase_pll.d ./User/Src/three_phrase_pll.o ./User/Src/three_phrase_pll.su ./User/Src/user_exit.cyclo ./User/Src/user_exit.d ./User/Src/user_exit.o ./User/Src/user_exit.su ./User/Src/user_global.cyclo ./User/Src/user_global.d ./User/Src/user_global.o ./User/Src/user_global.su

.PHONY: clean-User-2f-Src

