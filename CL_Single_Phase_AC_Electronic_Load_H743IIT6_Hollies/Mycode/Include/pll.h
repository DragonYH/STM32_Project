#ifndef __PLL_H
#define __PLL_H

#include "main.h"
#include "arm_math.h"
#include "pid.h"

#define MAX_COMPARE 11999 // 计时器最大计数值

// 电压信号数据
typedef struct pll_Signal_V
{
    // 基本变量
    float input[3];  // 输入序列
    float sogi_a[3]; // 输出序列alpha
    float sogi_b[3]; // 输出序列beta	滞后90度于序列alpha
    // sogi中间变量
    float sogi_lamda;
    float sogi_x;
    float sogi_y;
    float sogi_b0;
    float sogi_a1;
    float sogi_a2;
    // park变换相关变量
    float park_d; // 有功分量
    float park_q; // 无功分量
    float theta;  // 当前角度
    // PI控制器相关变量
    PID *pid; // pid指针
    // 配置参数
    float omiga0; // 无阻尼自然频率，2*pi*频率
    float Ts;     // 采样周期
    float k;      // 阻尼比 典型值1.41
    float phase;  // 设置相位
} pll_Signal_V;
// 电流信号数据
typedef struct pll_Signal_I
{
    // 基本变量
    float input[3];  // 输入序列
    float sogi_a[3]; // 输出序列alpha
    float sogi_b[3]; // 输出序列beta	滞后90度于序列alpha
    // sogi中间变量
    float sogi_lamda;
    float sogi_x;
    float sogi_y;
    float sogi_b0;
    float sogi_a1;
    float sogi_a2;
    // park变换相关变量
    float park_d; // 有功分量
    float park_q; // 无功分量
    // PR控制器相关变量
    float pr_kp;     // 比例系数
    float pr_kr;     // 积分系数
    float pr_out[3]; // 输出
    float pr_err[3]; // 误差
    // PI控制器相关变量
    PID *pid; // pid指针
    // pr中间变量
    float pr_a0;
    float pr_a1;
    float pr_a2;
    float pr_b0;
    float pr_b1;
    float pr_b2;
    // 配置参数
    float omiga0; // 无阻尼自然频率，2*pi*频率
    float omigaC; // 带宽2*pi*带宽
    float k;      // 阻尼比 典型值1.41
    float Ts;     // 采样周期
} pll_Signal_I;

void pll_Init_V(pll_Signal_V *signal, float f, uint16_t F, float Umax);
void pll_Init_I(pll_Signal_I *signal, float f, uint16_t F, float pr_kp, float pr_kr, float pi_kp, float pi_ki);
void pll_Control_V(pll_Signal_V *signal_V);
void pll_Control_I(pll_Signal_I *signal_I, pll_Signal_V *signal_V, float Uset, float Udc);
void pll_Pr(pll_Signal_I *signal, float target, float sample);
void pll_Sogi(pll_Signal_V *signal);
void pll_Sogi_I(pll_Signal_I *signal);

#endif
