#include "sogi.h"
/*
SOGI变换
调用频繁，使用内联函数可增加效率
*/
float k = 1.41f;    //阻尼比
signal_PLL sogi(signal_PLL signal)
{
    signal_PLL temp = signal;
    temp.temp.lamda = 0.5f * temp.omiga * temp.Ts;
    temp.temp.x = 2 * k * temp.omiga * temp.Ts;
    temp.temp.y = temp.omiga * temp.Ts * temp.omiga * temp.Ts;

    temp.temp.b0 = temp.temp.x / (temp.temp.x + temp.temp.y +4);
    temp.temp.a1 = (8-2 * temp.temp.y) / (temp.temp.x + temp.temp.y + 4);
    temp.temp.a2 = (temp.temp.x - temp.temp.y - 4) / (temp.temp.x + temp.temp.y + 4);

    temp.d_0 = temp.temp.b0 * temp.u_0 - temp.temp.b0 * temp.u_2 + temp.temp.a1 * temp.d_1 + temp.temp.a2 * temp.d_2;
    temp.q_0 = temp.temp.b0 * temp.u_0 +2 * temp.temp.b0 * temp.u_1 + temp.temp.b0 * temp.u_2 + temp.temp.a1 * temp.q_1 + temp.temp.a2 * temp.q_2;

    temp.u_2 = temp.u_1;
    temp.u_1 = temp.u_0;
    temp.d_2 = temp.d_1;
    temp.d_1 = temp.d_0;
    temp.q_2 = temp.q_1;
    temp.q_1 = temp.q_0;
    return temp;
}
