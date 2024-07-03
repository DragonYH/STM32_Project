#include "ad7606.h"
#include "main.h"
#include "spi.h"
#include "user_global.h"
#include "tim.h"
// #include "stm32h743xx.h"

/**
 * @brief  GPIO触发中断
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == AD7606_BUSY_Pin)
    {
        ad7606_GetValue(&hspi2, 8, adcValue);
    }
}
/**
 * @brief  定时器中断
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {
        HAL_IncTick();
    }
}
