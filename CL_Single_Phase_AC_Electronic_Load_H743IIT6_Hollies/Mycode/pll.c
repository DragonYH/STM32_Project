#include "pll.h"
#include "stdlib.h"

/**
 * @brief 电压信号参数初始化
 * @param signal 信号指针
 * @param f 信号频率(典型值:50)
 * @param F 采样频率(典型值:20000)
 * @param Umax 交流电压峰值
 */
void pll_Init_V(pll_Signal_V *signal, float f, uint16_t F, float Umax)
{
    // 初始化赋值
    signal->input[0] = 0.f;
    signal->input[1] = 0.f;
    signal->input[2] = 0.f;

    signal->sogi->a[1] = 0.f;
    signal->sogi->a[2] = 0.f;
    signal->sogi->b[1] = 0.f;
    signal->sogi->b[2] = 0.f;

    signal->theta = 0.f;
    signal->omiga0 = 2 * PI * f; // f典型值50
    signal->Ts = 1.f / F;        // F典型值20000
    signal->k = 1.414f;          // 阻尼比典型值1.414
    signal->phase = 0.f;         // 设定相位差
    // 初始化pid参数
    float ki = signal->omiga0 * signal->omiga0 / Umax;
    float kp = sqrt(2) * sqrt(Umax * ki) / Umax;

    pid_Init(signal->pid, kp, ki, 0, MAX_COMPARE, 0);
    // 计算sogi中间量
    signal->sogi->lamda = 0.5f * signal->omiga0 * signal->Ts;
    signal->sogi->x = 2.f * signal->k * signal->omiga0 * signal->Ts;
    signal->sogi->y = signal->omiga0 * signal->Ts * signal->omiga0 * signal->Ts;
    signal->sogi->b0 = signal->sogi->x / (signal->sogi->x + signal->sogi->y + 4);
    signal->sogi->a1 = (8 - 2.f * signal->sogi->y) / (signal->sogi->x + signal->sogi->y + 4);
    signal->sogi->a2 = (signal->sogi->x - signal->sogi->y - 4) / (signal->sogi->x + signal->sogi->y + 4);
}
/**
 * @brief 电流信号参数初始化
 * @param signal 信号指针
 * @param f 信号频率(典型值:50)
 * @param F 采样频率(典型值:20000)
 * @param kp PR控制器kp参数
 * @param kr PR控制器kr参数
 */
void pll_Init_I(pll_Signal_I *signal, float f, uint16_t F, float pr_kp, float pr_kr, float pi_kp, float pi_ki)
{
    // 初始化赋值
    signal->input[0] = 0.f;
    signal->input[1] = 0.f;
    signal->input[2] = 0.f;

    signal->sogi->a[1] = 0.f;
    signal->sogi->a[2] = 0.f;
    signal->sogi->b[1] = 0.f;
    signal->sogi->b[2] = 0.f;

    signal->omiga0 = 2 * PI * f;   // f典型值50
    signal->omigaC = 2 * PI * 0.2; // 带宽2*pi*带宽
    signal->Ts = 1.f / F;          // F典型值20000
    // 初始化pr参数
    signal->pr->out[1] = 0.f;
    signal->pr->out[2] = 0.f;
    signal->pr->err[1] = 0.f;
    signal->pr->err[2] = 0.f;
    // 初始化pid参数
    pid_Init(signal->pid, pi_kp, pi_ki, 0, 50 * PI, -20 * PI);
    // 计算pr中间量
    signal->pr->a0 = 4 * pr_kp / (signal->Ts * signal->Ts) + 4 * signal->omigaC * (pr_kp + pr_kr) / signal->Ts + pr_kp * signal->omiga0 * signal->omiga0;
    signal->pr->a1 = -8 * pr_kp / (signal->Ts * signal->Ts) + 2 * pr_kp * signal->omiga0 * signal->omiga0;
    signal->pr->a2 = 4 * pr_kp / (signal->Ts * signal->Ts) - 4 * signal->omigaC * (pr_kp + pr_kr) / signal->Ts + pr_kp * signal->omiga0 * signal->omiga0;
    signal->pr->b0 = 4 / (signal->Ts * signal->Ts) + 4 * signal->omigaC / signal->Ts + signal->omiga0 * signal->omiga0;
    signal->pr->b1 = -8 / (signal->Ts * signal->Ts) + 2 * signal->omiga0 * signal->omiga0;
    signal->pr->b2 = 4 / (signal->Ts * signal->Ts) - 4 * signal->omigaC / signal->Ts + signal->omiga0 * signal->omiga0;
    // 计算sogi中间量
    signal->sogi->lamda = 0.5f * signal->omiga0 * signal->Ts;
    signal->sogi->x = 2.f * signal->k * signal->omiga0 * signal->Ts;
    signal->sogi->y = signal->omiga0 * signal->Ts * signal->omiga0 * signal->Ts;
    signal->sogi->b0 = signal->sogi->x / (signal->sogi->x + signal->sogi->y + 4);
    signal->sogi->a1 = (8 - 2.f * signal->sogi->y) / (signal->sogi->x + signal->sogi->y + 4);
    signal->sogi->a2 = (signal->sogi->x - signal->sogi->y - 4) / (signal->sogi->x + signal->sogi->y + 4);
}
/**
 * @brief 电压锁相控制
 * @param signal_V 电压信号指针
 */
void pll_Control_V(pll_Signal_V *signal_V)
{
    // 对信号先进行sogi变换，得到两个相位相差90度的信号
    pll_Sogi(signal_V->input, signal_V->sogi);
    // 再对信号sogi变换后的信号进行park变换
    arm_park_f32(signal_V->sogi->a[0], signal_V->sogi->b[0], &signal_V->park_d, &signal_V->park_q, arm_sin_f32(signal_V->theta), arm_cos_f32(signal_V->theta));
    // 将park变换后的q送入PI控制器  输入值为设定值和采样值的误差
    pid(signal_V->pid, signal_V->park_q, signal_V->phase);
    // 更新theta
    signal_V->theta += (signal_V->pid->result + signal_V->omiga0) * signal_V->Ts;
    signal_V->theta = (float)fmod(signal_V->theta, 2 * PI);
}
/**
 * @brief 电流内环控制
 * @param signal_I 电流信号指针
 * @param signal_V 电压信号指针
 * @param Udc 直流测采样电压
 * @param Uset 直流设定电压
 */
void pll_Control_I(pll_Signal_I *signal_I, pll_Signal_V *signal_V, float Uset, float Udc)
{
    // 对信号先进行sogi变换，得到两个相位相差90度的信号
    pll_Sogi(signal_I->input, signal_I->sogi);
    // 再对信号sogi变换后的信号进行park变换
    arm_park_f32(signal_I->sogi->a[0], signal_I->sogi->b[0], &signal_I->park_d, &signal_I->park_q, arm_sin_f32(signal_V->theta), arm_cos_f32(signal_V->theta));
    // 对直流电压进行PI控制
    pid(signal_I->pid, Uset, Udc); // 电压内环
    // PR控制
    // pll_Pr(signal_I, signal_I->pid->result * arm_sin_f32(signal_V->theta), signal_I->input);
    pll_Pr(signal_I, 1.f * arm_sin_f32(signal_V->theta), signal_I->input[0]);
}
/**
 * @brief PR控制器
 * @param signal 信号指针
 * @param config 配置指针
 */
void pll_Pr(pll_Signal_I *signal, float target, float sample)
{
    signal->pr->err[0] = target - sample;
    signal->pr->out[0] = -signal->pr->b1 * signal->pr->out[1] - signal->pr->b2 * signal->pr->out[2] + signal->pr->a0 * signal->pr->err[0] + signal->pr->a1 * signal->pr->err[1] + signal->pr->a2 * signal->pr->err[2];
    signal->pr->out[0] = signal->pr->out[0] / signal->pr->b0;
    // 限制调参幅度，防止跑飞
    if (signal->pr->out[0] > MAX_COMPARE)
        signal->pr->out[0] = MAX_COMPARE;
    else if (signal->pr->out[0] < 0)
        signal->pr->out[0] = 0;

    signal->pr->out[1] = signal->pr->out[0];
    signal->pr->out[2] = signal->pr->out[1];
}
/**
 * @brief Sogi变换
 * @param input 输入信号
 * @param sogi sogi指针
 */
void pll_Sogi(float *input, SOGI *sogi)
{
    sogi->a[0] = sogi->b0 * input[0] - sogi->b0 * input[2] + sogi->a1 * sogi->a[1] + sogi->a2 * sogi->a[2];
    sogi->b[0] = sogi->lamda * sogi->b0 * (input[0] + 2 * input[1] + input[2]) + sogi->a1 * sogi->b[1] + sogi->a2 * sogi->b[2];

    input[2] = input[1];
    input[1] = input[0];
    sogi->a[2] = sogi->a[1];
    sogi->a[1] = sogi->a[0];
    sogi->b[2] = sogi->b[1];
    sogi->b[1] = sogi->b[0];
}
