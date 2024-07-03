#include "ad7606.h"
#include "main.h"
#include "spi.h"
#include "user_global.h"

// GPIO触发中断
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == AD7606_BUSY_Pin)
    {
        ad7606_GetValue(&hspi2, 8, adcValue);
    }
}
