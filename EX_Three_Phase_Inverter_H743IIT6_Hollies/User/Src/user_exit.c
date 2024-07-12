#include "ad7606.h"
#include "main.h"
#include "spi.h"
#include "user_global.h"
#include "tim.h"
#include "three_phrase_pll.h"
#include "svpwm.h"
#include "dac.h"
#include "arm_math.h"

/**
 * @brief  GPIO触发中断
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == AD7606_BUSY_Pin)
    {
        // 读取AD7606数据
        float adcValue[8] = {0};
        float temp_V1, temp_V2;
        ad7606_GetValue(&hspi2, 7, adcValue);
        // 处理电流数据
        signal_I->basic->input_a = adcValue[1] * 2.2258065f;
        signal_I->basic->input_b = adcValue[3] * 2.2258065f;
        signal_I->basic->input_c = adcValue[5] * 2.2258065f;
        // 处理电压数据，将线电压转为相电压
        float Uab = adcValue[2] * 38.423350f;
        float Ubc = adcValue[4] * 38.669998f;
        float Uca = adcValue[6] * 38.560814f;

        signal_V->basic->input_a = (Uab - Uca) / 3.f;
        signal_V->basic->input_b = (Ubc - Uab) / 3.f;
        signal_V->basic->input_c = (Uca - Ubc) / 3.f;

        temp_V1 = signal_V->basic->input_a;
        temp_V2 = signal_V->basic->input_b;
        // 模拟三相输入
        volatile static uint16_t cnt = 0;
        signal_V->basic->input_a = 11.f * arm_sin_f32(cnt * 2 * PI / 400);
        signal_V->basic->input_b = 11.f * arm_sin_f32(cnt * 2 * PI / 400 - 2 * PI / 3);
        // 锁相控制
        pll_Control_V(signal_V);
        // 电流内环控制
        pll_Control_I(signal_I, signal_V, 0.5f, 1.f);
        // svpwm调制
        svpwm_Control(signal_V, 20.f);
        // DAC输出
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)((__HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1) - 3000.f) / 3000.f * 2000.f + 2048.f));
        // 计算有效值
        static float V_rms_a[400] = {0};
        static float V_rms_b[400] = {0};
        static float V_rms_c[400] = {0};
        static float I_rms_a[400] = {0};
        static float I_rms_b[400] = {0};
        static float I_rms_c[400] = {0};

        V_rms_a[cnt] = temp_V1;
        V_rms_b[cnt] = temp_V2;
        // V_rms_a[cnt] = signal_V->basic->input_a;
        // V_rms_b[cnt] = signal_V->basic->input_b;
        V_rms_c[cnt] = signal_V->basic->input_c;
        I_rms_a[cnt] = signal_I->basic->input_a;
        I_rms_b[cnt] = signal_I->basic->input_b;
        I_rms_c[cnt] = signal_I->basic->input_c;
        if (++cnt == 400)
        {
            cnt = 0;
            arm_rms_f32(V_rms_a, 400, &signal_V->basic->rms_a);
            arm_rms_f32(V_rms_b, 400, &signal_V->basic->rms_b);
            arm_rms_f32(V_rms_c, 400, &signal_V->basic->rms_c);
            arm_rms_f32(I_rms_a, 400, &signal_I->basic->rms_a);
            arm_rms_f32(I_rms_b, 400, &signal_I->basic->rms_b);
            arm_rms_f32(I_rms_c, 400, &signal_I->basic->rms_c);
        }
    }
}
