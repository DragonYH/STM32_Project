#include "pll.h"
#include "stdlib.h"

float phase_set = 0.f;

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
    signal->phase = 0.f;         // 设定相位差
    // 初始化pid参数
    float ki = signal->omiga0 * signal->omiga0 / Umax;
    float kp = sqrt(2) * sqrt(Umax * ki) / Umax;
    pid_Init(signal->pid, kp, ki, 0, 50 * PI, -20 * PI);
    // 计算sogi中间量
    signal->sogi->k = 1.414f;
    signal->sogi->lamda = 0.5f * signal->omiga0 * signal->Ts;
    signal->sogi->x = 2.f * signal->sogi->k * signal->omiga0 * signal->Ts;
    signal->sogi->y = signal->omiga0 * signal->Ts * signal->omiga0 * signal->Ts;
    signal->sogi->b0 = signal->sogi->x / (signal->sogi->x + signal->sogi->y + 4);
    signal->sogi->a1 = (8 - 2.f * signal->sogi->y) / (signal->sogi->x + signal->sogi->y + 4);
    signal->sogi->a2 = (signal->sogi->x - signal->sogi->y - 4) / (signal->sogi->x + signal->sogi->y + 4);
}
#if PRorPI
/**
 * @brief 电流信号参数初始化
 * @param signal 信号指针
 * @param f 信号频率(典型值:50)
 * @param F 采样频率(典型值:20000)
 * @param pr_kp PR控制器kp参数
 * @param pr_kr PR控制器kr参数
 * @param pi_kp PI控制器kp参数
 * @param pi_ki PI控制器ki参数
 */
void pll_Init_I(pll_Signal_I *signal, float f, uint16_t F, float pr_kp, float pr_kr, float pi_kp, float pi_ki)
#else
/**
 * @brief 电流信号参数初始化
 * @param signal 信号指针
 * @param f 信号频率(典型值:50)
 * @param F 采样频率(典型值:20000)
 * @param pi_kp PI控制器kp参数
 * @param pi_ki PI控制器ki参数
 */
void pll_Init_I(pll_Signal_I *signal, float f, uint16_t F, float pi_kp, float pi_ki)
#endif
{
    // 初始化赋值
    signal->input[0] = 0.f;
    signal->input[1] = 0.f;
    signal->input[2] = 0.f;

    signal->sogi->a[1] = 0.f;
    signal->sogi->a[2] = 0.f;
    signal->sogi->b[1] = 0.f;
    signal->sogi->b[2] = 0.f;

    signal->omiga0 = 2.f * PI * f; // f典型值50
    signal->Ts = 1.f / F;          // F典型值20000
#if PRorPI
    // 初始化pr参数
    signal->omigaC = 2.f * PI * 0.5f; // 带宽2*pi*带宽
    signal->pr->out[1] = 0.f;
    signal->pr->out[2] = 0.f;
    signal->pr->err[1] = 0.f;
    signal->pr->err[2] = 0.f;
    // 初始化pid参数
    pid_Init(signal->pid, pi_kp, pi_ki, 0, 0.f, 0.f);
    // 计算pr中间量
    signal->pr->a0 = signal->omiga0 * signal->omiga0 * signal->Ts * signal->Ts + 4 * signal->omigaC * signal->Ts + 4;
    signal->pr->a1 = (2 * signal->omiga0 * signal->omiga0 * signal->Ts * signal->Ts - 8) / signal->pr->a0;
    signal->pr->a2 = (signal->omiga0 * signal->omiga0 * signal->Ts * signal->Ts - 4 * signal->omigaC * signal->Ts + 4) / signal->pr->a0;
    signal->pr->b0 = (pr_kp * (signal->omiga0 * signal->omiga0 * signal->Ts * signal->Ts + 4 * signal->omigaC * signal->Ts + 4) + 4 * pr_kr * signal->omigaC * signal->Ts) / signal->pr->a0;
    signal->pr->b1 = (pr_kp * (2 * signal->omiga0 * signal->omiga0 * signal->Ts * signal->Ts - 8)) / signal->pr->a0;
    signal->pr->b2 = (pr_kp * (signal->omiga0 * signal->omiga0 * signal->Ts * signal->Ts - 4 * signal->omigaC * signal->Ts + 4) - 4 * pr_kr * signal->omigaC * signal->Ts) / signal->pr->a0;
#else
    signal->L = 0.0043f; // 4.3mH
    pid_Init(signal->pid_d, 0.05f, 0.002f, 0, 50.f, -50.f);
    pid_Init(signal->pid_q, pi_kp, pi_ki, 0, 90.f, -90.f);
#endif
    // 计算sogi中间量
    signal->sogi->k = 1.414f; // 阻尼比典型值1.414
    signal->sogi->lamda = 0.5f * signal->omiga0 * signal->Ts;
    signal->sogi->x = 2.f * signal->sogi->k * signal->omiga0 * signal->Ts;
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
    pll_Sogi(signal_V->sogi, signal_V->input);
    // 再对信号sogi变换后的信号进行park变换
    arm_park_f32(signal_V->sogi->a[0], signal_V->sogi->b[0], &signal_V->park_d, &signal_V->park_q, arm_sin_f32(signal_V->theta), arm_cos_f32(signal_V->theta));
    // 将park变换后的q送入PI控制器  输入值为设定值和采样值的误差
    pid(signal_V->pid, signal_V->park_q, signal_V->phase);
    // 更新theta
    signal_V->theta += (signal_V->pid->out + signal_V->omiga0) * signal_V->Ts;
    signal_V->theta = (float)fmod(signal_V->theta, 2 * PI);
}
#if PRorPI
/**
 * @brief 电流内环控制
 * @param signal_I 电流信号指针
 * @param signal_V 电压信号指针
 * @param Udc 直流测采样电压
 * @param Uset 直流设定电压
 */
void pll_Control_I(pll_Signal_I *signal_I, pll_Signal_V *signal_V, float Uset, float Udc)
#else
/**
 * @brief 电流内环控制
 * @param signal_I 电流信号指针
 * @param signal_V 电压信号指针
 * @param Iset 电流设定值(有效值)
 * @param phase 相位设置
 */
void pll_Control_I(pll_Signal_I *signal_I, pll_Signal_V *signal_V, float Iset, float phase)
#endif
{
#if PRorPI
    // 对直流电压进行PI控制
    // pid(signal_I->pid_dc, Uset, Udc); // 电压内环
    // PR控制
    // * pll_Pr(signal_I->pr, signal_I->input[0], signal_I->pid->out * arm_cos_f32(signal_V->theta));
    // * pll_Pr(signal_I->pr, signal_I->input[0], 1.414f * arm_cos_f32(signal_V->theta + 0.2 * PI)); // PF= 1.00
    pll_Pr(signal_I->pr, signal_I->input[0], 1.414f * arm_cos_f32(signal_V->theta + phase_set * PI));
#else
    static float Uabd;
    static float Uabq;
    // 对信号先进行sogi变换，得到两个相位相差90度的信号
    pll_Sogi(signal_I->sogi, signal_I->input);
    // 在电压的系上得出电流的dq值
    arm_park_f32(signal_I->sogi->a[0], signal_I->sogi->b[0], &signal_I->park_d, &signal_I->park_q, arm_sin_f32(signal_V->theta), arm_cos_f32(signal_V->theta));
    // PI控制
    pid(signal_I->pid_d, Iset, signal_I->peak);    // 电流大小
    pid(signal_I->pid_q, phase, signal_I->park_q); // 相位
    // 解耦调制
    Uabd = signal_V->park_d - signal_I->pid_d->out + signal_I->park_q * signal_I->omiga0 * signal_I->L;
    Uabq = signal_V->park_q - signal_I->pid_q->out - signal_I->park_d * signal_I->omiga0 * signal_I->L;
    // park逆变换
    arm_inv_park_f32(Uabd, Uabq, &signal_I->park_inv_a, &signal_I->park_inv_b, arm_sin_f32(signal_V->theta), arm_cos_f32(signal_V->theta));
    // 输出限幅
    if (signal_I->park_inv_a > 100.f)
        signal_I->park_inv_a = 100.f;
    else if (signal_I->park_inv_a < -100.f)
        signal_I->park_inv_a = -100.f;
#endif
}
#if PRorPI
/**
 * @brief PR控制器
 * @param pr PR控制器指针
 * @param target 目标值
 * @param sample 采样值
 */
void pll_Pr(PR *pr, float target, float sample)
{
    pr->err[0] = target - sample;
    pr->out[0] = pr->b0 * pr->err[0] + pr->b1 * pr->err[1] + pr->b2 * pr->err[2] - pr->a1 * pr->out[1] - pr->a2 * pr->out[2];
    // 限制调参幅度，防止跑飞
    if (pr->out[0] > COMPARE_MAX)
        pr->out[0] = COMPARE_MAX;
    else if (pr->out[0] < COMPARE_MIN)
        pr->out[0] = COMPARE_MIN;

    pr->out[2] = pr->out[1];
    pr->out[1] = pr->out[0];
    pr->err[2] = pr->err[1];
    pr->err[1] = pr->err[0];
}
#endif
/**
 * @brief Sogi变换
 * @param input 输入信号
 * @param sogi sogi指针
 */
void pll_Sogi(SOGI *sogi, float *input)
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
