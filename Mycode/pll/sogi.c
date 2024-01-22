#include "sogi.h"
#include "pll.h"
/*
SOGI变换
调用频繁，使用内联函数可增加效率
*/
void sogi(pll_Signal *signal)
{
    signal->sogi_d_0 = signal->b0 * signal->u_0 - signal->b0 * signal->u_2 + signal->a1 * signal->sogi_d_1 + signal->a2 * signal->sogi_d_2;
    signal->sogi_q_0 = signal->b0 * signal->u_0 + 2 * signal->b0 * signal->u_1 + signal->b0 * signal->u_2 + signal->a1 * signal->sogi_q_1 + signal->a2 * signal->sogi_q_2;

    signal->u_2 = signal->u_1;
    signal->u_1 = signal->u_0;
    signal->sogi_d_2 = signal->sogi_d_1;
    signal->sogi_d_1 = signal->sogi_d_0;
    signal->sogi_q_2 = signal->sogi_q_1;
    signal->sogi_q_1 = signal->sogi_q_0;
}
