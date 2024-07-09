#include "ad7606.h"
#include "main.h"
#include "spi.h"
#include "user_global.h"
#include "tim.h"
#include "three_phrase_pll.h"
#include "svpwm.h"
#include "dac.h"

/**
 * @brief  GPIO触发中断
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == AD7606_BUSY_Pin)
    {
        // 读取AD7606数据
        ad7606_GetValue(&hspi2, 5, adcValue);
        signal_V->basic->input_a = adcValue[1] * 10.f;
        signal_V->basic->input_b = adcValue[2] * 10.f;
        signal_I->basic->input_a = adcValue[3];
        signal_I->basic->input_b = adcValue[4];
        // 锁相控制
        pll_Control_V(signal_V);
        // 电流内环控制
        pll_Control_I(signal_I, signal_V, 0.5f, 1.f);
        // svpwm调制
        svpwm_Control(signal_V, 20.f);
        // DAC输出
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)((__HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1) - 3000.f) / 3000.f * 2000.f + 2048.f));
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
