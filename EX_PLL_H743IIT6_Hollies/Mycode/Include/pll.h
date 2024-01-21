#ifndef __PLL_H
#define __PLL_H

#include "main.h"

// pll相关配置参数
typedef struct pll_Config
{
    float omiga;    // 无阻尼自然频率，2*pi*频率
    float Ts;   // 采样周期
    float k;	// 阻尼比 典型值1.41

    float phase;	//相位
}pll_Config;

// pll相关信号数据
typedef struct pll_Signal
{
	// 基本变量
    float u_0, u_1, u_2;	// 输入序列
    float d_0, d_1, d_2;	// 输出序列d
    float q_0, q_1, q_2;	// 输出序列q	滞后90度于序列d
    // 中间变量
    float lamda;
    float x;
    float y;
    float b0;
    float a1;
    float a2;
}pll_Signal;

extern pll_Signal *signal_1;
extern pll_Config *signal_cofig_1;

void pll_Init(pll_Signal *signal, pll_Config *config, float f, uint16_t F);
void pll_Clear(pll_Signal *signal, pll_Config *config);

#endif
