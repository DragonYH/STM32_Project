#ifndef __SIGNAL_PLL_H
#define __SIGNAL_PLL_H

typedef struct signal_PLL
{
    float omiga;    // 无阻尼自然频率，2*pi*频率
    float Ts;   // 采样周期
    float d_0, d_1, d_2;
    float q_0, q_1, q_2;
    float u_0, u_1, u_2;	// 输入序列

    float lamda;
    float x;
    float y;
    float b0;
    float a1;
    float a2;
}signal_PLL;

void signal_PLL_Init(signal_PLL *signal);

#endif
