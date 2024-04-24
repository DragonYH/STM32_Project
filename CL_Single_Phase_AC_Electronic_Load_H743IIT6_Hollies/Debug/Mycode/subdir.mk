################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mycode/ad7606.c \
../Mycode/iir.c \
../Mycode/ina238.c \
../Mycode/oled.c \
../Mycode/pid.c \
../Mycode/pll.c 

OBJS += \
./Mycode/ad7606.o \
./Mycode/iir.o \
./Mycode/ina238.o \
./Mycode/oled.o \
./Mycode/pid.o \
./Mycode/pll.o 

C_DEPS += \
./Mycode/ad7606.d \
./Mycode/iir.d \
./Mycode/ina238.d \
./Mycode/oled.d \
./Mycode/pid.d \
./Mycode/pll.d 


# Each subdirectory must supply rules for building sources it contributes
Mycode/%.o Mycode/%.su Mycode/%.cyclo: ../Mycode/%.c Mycode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DARM_MATH_CM7 -c -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/DSP/Include -I../Mycode/Include -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Mycode

clean-Mycode:
	-$(RM) ./Mycode/ad7606.cyclo ./Mycode/ad7606.d ./Mycode/ad7606.o ./Mycode/ad7606.su ./Mycode/iir.cyclo ./Mycode/iir.d ./Mycode/iir.o ./Mycode/iir.su ./Mycode/ina238.cyclo ./Mycode/ina238.d ./Mycode/ina238.o ./Mycode/ina238.su ./Mycode/oled.cyclo ./Mycode/oled.d ./Mycode/oled.o ./Mycode/oled.su ./Mycode/pid.cyclo ./Mycode/pid.d ./Mycode/pid.o ./Mycode/pid.su ./Mycode/pll.cyclo ./Mycode/pll.d ./Mycode/pll.o ./Mycode/pll.su

.PHONY: clean-Mycode

