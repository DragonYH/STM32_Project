#include "sogi.h"
#include "pll.h"
/*
SOGI变换
调用频繁，使用内联函数可增加效率
*/
void sogi(pll_Signal *signal)
{
    signal->d_0 = signal->b0 * signal->u_0 - signal->b0 * signal->u_2 + signal->a1 * signal->d_1 + signal->a2 * signal->d_2;
    signal->q_0 = signal->b0 * signal->u_0 + 2 * signal->b0 * signal->u_1 + signal->b0 * signal->u_2 + signal->a1 * signal->q_1 + signal->a2 * signal->q_2;

    signal->u_2 = signal->u_1;
    signal->u_1 = signal->u_0;
    signal->d_2 = signal->d_1;
    signal->d_1 = signal->d_0;
    signal->q_2 = signal->q_1;
    signal->q_1 = signal->q_0;
}
