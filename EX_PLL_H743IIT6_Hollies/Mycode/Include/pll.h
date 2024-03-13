#ifndef __PLL_H
#define __PLL_H

#include "main.h"
#include "arm_math.h"
#include "tim.h"

// pll相关配置参数
typedef struct pll_Config
{
    float omiga; // 无阻尼自然频率，2*pi*频率
    float Ts;    // 采样周期
    float k;     // 阻尼比 典型值1.41

    float phase; // 设置相位

    float pid_kp, pid_ki, pid_kd; // PID参数

    uint16_t maxCount; // 最大计数值
    uint16_t minCount; // 最小计数值

} pll_Config;

// pll相关信号数据
typedef struct pll_Signal
{
    // 基本变量
    float u_0, u_1, u_2;                // 输入序列
    float sogi_d_0, sogi_d_1, sogi_d_2; // 输出序列d
    float sogi_q_0, sogi_q_1, sogi_q_2; // 输出序列q	滞后90度于序列d
    // park变换相关变量
    float park_d; // 幅值
    float park_q; // 相位差
    float theta;
    // PI控制器相关变量
    float pid_out;
    float pid_err[2];
    // sogi中间变量
    float lamda;
    float x;
    float y;
    float b0;
    float a1;
    float a2;
} pll_Signal;

void pll_Init(pll_Signal *signal, pll_Config *config, float f, uint16_t F);
void pll_Control(pll_Signal *signal, pll_Config *config);
void pll_Pid(pll_Signal *signal, pll_Config *config);
void pll_Clear(pll_Signal *signal, pll_Config *config);

#endif
