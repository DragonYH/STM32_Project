/*
 * @Author       : DragonYH 1016633827@qq.com
 * @Date         : 2024-07-20 20:11:13
 * @LastEditors  : DragonYH 1016633827@qq.com
 * @LastEditTime : 2024-07-24 09:46:42
 * @FilePath     : \EX_Single_Phase_Rectifier_H743IIT6_Hollies\User\Src\user_exit.c
 * @Description  : 用户中断
 *
 * Copyright (c) 2024 by DragonYH, All Rights Reserved.
 */

#include "user_task.h"
#include "single_phrase_pll.h"
#include "user_exit.h"
#include "ad7606.h"
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "dac.h"
#include "arm_math.h"

pll_Signal_V *signal_V;
pll_Signal_I *signal_I;

static void getVoltageCurrent(void);
static void calcEffectiveValue(void);
static void normalize(void);
static void spwmContral(void);

/**
 * @brief    GPIO触发中断
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == AD7606_BUSY_Pin)
    {
        getVoltageCurrent();     /* 获取电压电流 */
        calcEffectiveValue();    /* 计算有效值 */
        normalize();             /* 标幺化 */
        pll_Control_V(signal_V); /* 锁相控制 */

        /* 电流内环控制 */
        if (deviceState == RUN)
            pll_Control_I(signal_I, signal_V, Itarget, 1.f);

        /* spwm输出 */
        spwmContral();

        /* DAC输出 */
        // uint32_t dacValue = (uint32_t)(signal_I->basic->input[0] * 2000.f + 2048.f);
        uint32_t dacValue = (uint32_t)(arm_cos_f32(signal_V->theta) * 2000.f + 2048.f);
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacValue);
    }
}

/**
 * @brief    获取相电压和电流
 */
static void getVoltageCurrent(void)
{
    /* 读取AD7606数据 */
    float adcValue[8] = {0};
    ad7606_GetValue(&hspi2, 3, adcValue);

    /* 处理电流电压数据 */
    signal_V->basic->input[0] = adcValue[1] * 41.795775f;
    signal_I->basic->input[0] = adcValue[2] * 2.5223214f;
}

/**
 * @brief    计算电压电流有效值
 */
static void calcEffectiveValue(void)
{
    volatile static uint16_t cnt = 0;

    static float Vrms[400] = {0};
    static float Irms[400] = {0};

    Vrms[cnt] = signal_V->basic->input[0];
    Irms[cnt] = signal_I->basic->input[0];

    if (++cnt == 400)
    {
        cnt = 0;
        arm_rms_f32(Vrms, 400, &signal_V->basic->rms);
        arm_rms_f32(Irms, 400, &signal_I->basic->rms);
    }
}

/**
 * @brief    标幺化
 */
static void normalize(void)
{
    signal_V->basic->input[0] /= Ubase;
    signal_I->basic->input[0] /= Ibase;
}

/**
 * @brief    spwm控制
 * @return
 */
static void spwmContral(void)
{
    /* 计算比较值 */
    float COMPARE = signal_I->park_inv_alpha * (TIM_PERIOD - 1);

    /* 调节SPWM占空比 */
    if (signal_I->park_inv_alpha > 0)
    {
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, COMPARE);
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 0);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, -COMPARE);
    }
}
