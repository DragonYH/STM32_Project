#include "svpwm.h"
#include "tim.h"
#include "dac.h"

/**
 * @brief  SVPWM控制
 * @param  signal PLL信号输入结构体指针
 * @param  Udc 直流母线电压
 */
void svpwm_Control(pll_Signal_V *signal, float Udc)
{
    // 电压标幺化
    float Ts = signal->basic->Ts;
    float Ubase = Udc / 1.7320508f;
    float Ualpha = signal->basic->clarke_alpha / Ubase;
    float Ubeta = signal->basic->clarke_beta / Ubase;
    // 计算中间变量
    float Ualpha_ = 1.7320508f * Ualpha * Ts;
    float Ubeta_ = Ubeta * Ts;
    // 计算XYZ
    float X = Ubeta_;
    float Y = 0.5f * Ualpha_ + 0.5f * Ubeta_;
    float Z = 0.5f * Ubeta_ - 0.5f * Ualpha_;
    // 通过XYZ做扇区判断
    uint8_t sector = 0;
    if (Y < 0)
    {
        if (Z < 0)
        {
            sector = 5;
        }
        else
        {
            if (X < 0)
            {
                sector = 4;
            }
            else
            {
                sector = 3;
            }
        }
    }
    else
    {
        if (Z > 0)
        {
            sector = 2;
        }
        else
        {
            if (X < 0)
            {
                sector = 6;
            }
            else
            {
                sector = 1;
            }
        }
    }
    // 计算每一周期对应扇区各相的占用时间
    float Ta = 0, Tb = 0, Tc = 0;
    switch (sector)
    {
    case 1:
        Ta = Ts / 2.f + (-Z + X) / 2.f;
        Tb = Ta + Z;
        Tc = Tb - X;
        break;
    case 2:
        Tb = Ts / 2.f + (Z + Y) / 2.f;
        Ta = Tb - Z;
        Tc = Ta - Y;
        break;
    case 3:
        Tb = Ts / 2.f + (X - Y) / 2.f;
        Tc = Tb - X;
        Ta = Tc + Y;
        break;
    case 4:
        Tc = Ts / 2.f + (-X + Z) / 2.f;
        Tb = Tc + X;
        Ta = Tb - Z;
        break;
    case 5:
        Tc = Ts / 2.f + (-Y - Z) / 2.f;
        Ta = Tc + Y;
        Tb = Ta + Z;
        break;
    case 6:
        Ta = Ts / 2.f + (Y - X) / 2.f;
        Tc = Ta - Y;
        Tb = Tc + X;
        break;
    }
    // 计算占空比并更新TIM寄存器
    uint32_t ccr_a = (uint32_t)(Ta / Ts * 5999);
    uint32_t ccr_b = (uint32_t)(Tb / Ts * 5999);
    uint32_t ccr_c = (uint32_t)(Tc / Ts * 5999);

    if (ccr_a > 5999)
        ccr_a = 5999;
    else if (ccr_a < 0)
        ccr_a = 0;
    if (ccr_b > 5999)
        ccr_b = 5999;
    else if (ccr_b < 0)
        ccr_b = 0;
    if (ccr_c > 5999)
        ccr_c = 5999;
    else if (ccr_c < 0)
        ccr_c = 0;

    TIM1->CCR1 = ccr_a;
    TIM1->CCR2 = ccr_b;
    TIM1->CCR3 = ccr_c;
}
