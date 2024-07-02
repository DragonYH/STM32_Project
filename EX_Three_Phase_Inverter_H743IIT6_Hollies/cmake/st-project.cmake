# THIS FILE IS AUTOMATICALLY GENERATED. DO NOT EDIT.
# BASED ON d:\User\Document\STM32_Project\EX_Three_Phase_Inverter_H743IIT6_Hollies

function(add_st_target_properties TARGET_NAME)

target_compile_definitions(
    ${TARGET_NAME} PRIVATE
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM>>:DEBUG>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:DEBUG>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:USE_HAL_DRIVER>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:STM32H743xx>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:USE_HAL_DRIVER>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:STM32H743xx>"
)

target_include_directories(
    ${TARGET_NAME} PRIVATE
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Core\\Inc>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\STM32H7xx_HAL_Driver\\Inc>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\STM32H7xx_HAL_Driver\\Inc\\Legacy>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\CMSIS\\Device\\ST\\STM32H7xx\\Include>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\CMSIS\\Include>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FreeRTOS\\Source\\include>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FreeRTOS\\Source\\CMSIS_RTOS_V2>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FreeRTOS\\Source\\portable\\GCC\\ARM_CM4F>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/User\\Inc>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Core\\Inc>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\STM32H7xx_HAL_Driver\\Inc>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\STM32H7xx_HAL_Driver\\Inc\\Legacy>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\CMSIS\\Device\\ST\\STM32H7xx\\Include>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\CMSIS\\Include>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FreeRTOS\\Source\\include>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FreeRTOS\\Source\\CMSIS_RTOS_V2>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FreeRTOS\\Source\\portable\\GCC\\ARM_CM4F>"
)

target_compile_options(
    ${TARGET_NAME} PRIVATE
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM>>:-g3>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g3>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g3>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:ASM>>:-g0>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:-g0>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:-g0>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:-Os>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:-Os>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:>"
    "$<$<CONFIG:Debug>:-mcpu=cortex-m7>"
    "$<$<CONFIG:Debug>:-mfpu=fpv5-d16>"
    "$<$<CONFIG:Debug>:-mfloat-abi=hard>"
    "$<$<NOT:$<CONFIG:Debug>>:-mcpu=cortex-m7>"
    "$<$<NOT:$<CONFIG:Debug>>:-mfpu=fpv5-d16>"
    "$<$<NOT:$<CONFIG:Debug>>:-mfloat-abi=hard>"
)

target_link_libraries(
    ${TARGET_NAME} PRIVATE
)

target_link_directories(
    ${TARGET_NAME} PRIVATE
)

target_link_options(
    ${TARGET_NAME} PRIVATE
    "$<$<CONFIG:Debug>:-mcpu=cortex-m7>"
    "$<$<CONFIG:Debug>:-mfpu=fpv5-d16>"
    "$<$<CONFIG:Debug>:-mfloat-abi=hard>"
    "$<$<NOT:$<CONFIG:Debug>>:-mcpu=cortex-m7>"
    "$<$<NOT:$<CONFIG:Debug>>:-mfpu=fpv5-d16>"
    "$<$<NOT:$<CONFIG:Debug>>:-mfloat-abi=hard>"
    -T
    "$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/STM32H743IITX_FLASH.ld>"
    "$<$<NOT:$<CONFIG:Debug>>:${PROJECT_SOURCE_DIR}/STM32H743IITX_FLASH.ld>"
)

target_sources(
    ${TARGET_NAME} PRIVATE
    "User\\Src\\ad7606.c"
    "User\\Src\\oled.c"
    "User\\Src\\user.c"
    "Core\\Src\\freertos.c"
    "Core\\Src\\gpio.c"
    "Core\\Src\\main.c"
    "Core\\Src\\memorymap.c"
    "Core\\Src\\spi.c"
    "Core\\Src\\stm32h7xx_hal_msp.c"
    "Core\\Src\\stm32h7xx_hal_timebase_tim.c"
    "Core\\Src\\stm32h7xx_it.c"
    "Core\\Src\\syscalls.c"
    "Core\\Src\\sysmem.c"
    "Core\\Src\\system_stm32h7xx.c"
    "Core\\Src\\tim.c"
    "Core\\Startup\\startup_stm32h743iitx.s"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\CMSIS_RTOS_V2\\cmsis_os2.c"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\croutine.c"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\event_groups.c"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\list.c"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\portable\\GCC\\ARM_CM4F\\port.c"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\portable\\MemMang\\heap_4.c"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\queue.c"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\stream_buffer.c"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\tasks.c"
    "Middlewares\\Third_Party\\FreeRTOS\\Source\\timers.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_cortex.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_dma_ex.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_dma.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_exti.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_flash_ex.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_flash.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_gpio.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_hsem.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_i2c_ex.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_i2c.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_mdma.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_pwr_ex.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_pwr.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_rcc_ex.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_rcc.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_spi_ex.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_spi.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_tim_ex.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal_tim.c"
    "Drivers\\STM32H7xx_HAL_Driver\\Src\\stm32h7xx_hal.c"
)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_NAME}>
)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex
    $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.hex
)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
    $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.bin
)

endfunction()