################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mycode/ina238.c \
../Mycode/oled_spi.c \
../Mycode/pll.c 

OBJS += \
./Mycode/ina238.o \
./Mycode/oled_spi.o \
./Mycode/pll.o 

C_DEPS += \
./Mycode/ina238.d \
./Mycode/oled_spi.d \
./Mycode/pll.d 


# Each subdirectory must supply rules for building sources it contributes
Mycode/%.o Mycode/%.su Mycode/%.cyclo: ../Mycode/%.c Mycode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DARM_MATH_CM7 -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/DSP/Include -I../Mycode/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Mycode

clean-Mycode:
	-$(RM) ./Mycode/ina238.cyclo ./Mycode/ina238.d ./Mycode/ina238.o ./Mycode/ina238.su ./Mycode/oled_spi.cyclo ./Mycode/oled_spi.d ./Mycode/oled_spi.o ./Mycode/oled_spi.su ./Mycode/pll.cyclo ./Mycode/pll.d ./Mycode/pll.o ./Mycode/pll.su

.PHONY: clean-Mycode

