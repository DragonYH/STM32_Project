#include "three_phrase_pll.h"
#include "user_global.h"
#include "svpwm.h"
#include "fir.h"
#include "ad7606.h"
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "dac.h"
#include "arm_math.h"
#include "usbd_cdc_if.h"

static void calcEffectiveValue(void);
static void normalize(void);
static void getVoltageCurrent(void);

/**
 * @brief  GPIO触发中断
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == AD7606_BUSY_Pin)
    {
        getVoltageCurrent();     // 获取电压电流
        calcEffectiveValue();    // 计算有效值
        normalize();             // 标幺化
        pll_Control_V(signal_V); // 锁相控制

#if RectifierOrInverter
        // 电流内环控制
        if (runState == RUN)
            pll_Control_I(signal_I, signal_V, 1.2f, 1.f);
#endif

        svpwm_Control(signal_I); // svpwm调制

        // DAC输出
        // uint32_t dacValue = (uint32_t)((__HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1) - 3000.f) / 3000.f * 2000.f + 2048.f);
        uint32_t dacValue = (uint32_t)(signal_V->basic->input_a * 2000.f + 2048.f);
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue);
    }
}

/**
 * @brief  计算电压电流有效值
 */
static void calcEffectiveValue(void)
{
    volatile static uint16_t cnt = 0;

    static float V_rms_a[400] = {0};
    static float V_rms_b[400] = {0};
    static float V_rms_c[400] = {0};
    static float I_rms_a[400] = {0};
    static float I_rms_b[400] = {0};
    static float I_rms_c[400] = {0};

    V_rms_a[cnt] = signal_V->basic->input_a;
    V_rms_b[cnt] = signal_V->basic->input_b;
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

/**
 * @brief  标幺化
 */
static void normalize(void)
{
    signal_V->basic->input_a /= Ubase;
    signal_V->basic->input_b /= Ubase;
    signal_V->basic->input_c /= Ubase;
    signal_I->basic->input_a /= Ibase;
    signal_I->basic->input_b /= Ibase;
    signal_I->basic->input_c /= Ibase;
}

/**
 * @brief  获取相电压和电流
 */
static void getVoltageCurrent(void)
{
    // 读取AD7606数据
    float adcValue[8] = {0};
    ad7606_GetValue(&hspi2, 7, adcValue);

    // 处理电压数据，将线电压转为相电压
    float Uab = adcValue[2];
    float Ubc = adcValue[4];
    float Uca = adcValue[6];

    float samp_Va = 38.334749f * (Uab - Uca) / 3.f;
    float samp_Vb = 38.527397f * (Ubc - Uab) / 3.f;
    float samp_Vc = 38.525180f * (Uca - Ubc) / 3.f;

    // 处理电流数据
    float samp_Ia = adcValue[1] * 2.178571f;
    float samp_Ib = adcValue[3] * 2.250774f;
    float samp_Ic = adcValue[5] * 2.172956f;

    signal_V->basic->input_a = samp_Va;
    signal_V->basic->input_b = samp_Vb;
    signal_V->basic->input_c = samp_Vc;
    signal_I->basic->input_a = samp_Ia;
    signal_I->basic->input_b = samp_Ib;
    signal_I->basic->input_c = samp_Ic;

    // fir滤波
    // arm_fir_f32(fir_Va, &samp_Va, &signal_V->basic->input_a, 1);
    // arm_fir_f32(fir_Vb, &samp_Vb, &signal_V->basic->input_b, 1);
    // arm_fir_f32(fir_Vc, &samp_Vc, &signal_V->basic->input_c, 1);
    // arm_fir_f32(fir_Ia, &samp_Ia, &signal_I->basic->input_a, 1);
    // arm_fir_f32(fir_Ib, &samp_Ib, &signal_I->basic->input_b, 1);
    // arm_fir_f32(fir_Ic, &samp_Ic, &signal_I->basic->input_c, 1);
}
