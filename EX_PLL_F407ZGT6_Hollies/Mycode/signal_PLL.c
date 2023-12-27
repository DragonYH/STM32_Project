#include "signal_PLL.h"
//初始化结构体
signal_PLL signal_PLL_Init(signal_PLL pll)
{
    signal_PLL temp = pll;
    temp.d_1 = 0.f;
    temp.d_2 = 0.f;
    temp.q_1 = 0.f;
    temp.q_2 = 0.f;
    temp.omiga = 2 * 3.14f * 50;
    temp.Ts = 1 / 20000.f;
    return temp;
}