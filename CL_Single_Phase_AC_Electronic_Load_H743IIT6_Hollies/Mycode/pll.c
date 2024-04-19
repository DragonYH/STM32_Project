#include "pll.h"
#include "stdlib.h"

/**
 * @brief 信号参数初始化
 * @param signal 信号指针
 * @param config 配置指针
 * @param f 信号频率(典型值:50)
 * @param F 采样频率(典型值:20000)
 */
void pll_Init(pll_Signal *signal, pll_Config *config, float f, uint16_t F)
{
    // 初始化赋值
    signal->u_0 = 0.f;
    signal->u_1 = 0.f;
    signal->u_2 = 0.f;

    signal->sogi_a_1 = 0.f;
    signal->sogi_a_2 = 0.f;
    signal->sogi_b_1 = 0.f;
    signal->sogi_b_2 = 0.f;

    signal->theta = 0.f;

    config->omiga = 2 * PI * f; // f典型值50
    config->Ts = 1.f / F;       // F典型值20000
    config->k = 1.414f;         // 阻尼比典型值1.414
    config->phase = 0.f;        // 设定相位差
    // 计算中间量
    signal->lamda = 0.5f * config->omiga * config->Ts;
    signal->x = 2.f * config->k * config->omiga * config->Ts;
    signal->y = config->omiga * config->Ts * config->omiga * config->Ts;

    signal->b0 = signal->x / (signal->x + signal->y + 4);
    signal->a1 = (8 - 2.f * signal->y) / (signal->x + signal->y + 4);
    signal->a2 = (signal->x - signal->y - 4) / (signal->x + signal->y + 4);
    // 初始化pid参数
    config->pid_kp = 70.73586f;
    config->pid_ki = 15715.9305749f;
    config->pid_kd = 0.f; // 只需要PI控制
    signal->pid_err[1] = 0.f;
    signal->pid_out = 0.f;
}
/**
 * @brief 锁相控制
 * @param signal_In 信号指针
 * @param config 配置指针
 * @param signal_Target 目标信号指针
 */
void pll_Control(pll_Signal *signal_In, pll_Config *config, pll_Signal *signal_Target)
{
    // 对信号先进行sogi变换，得到两个相位相差90度的信号
    pll_Sogi(signal_In);
    // 再对信号sogi变换后的信号进行park变换
    arm_park_f32(signal_In->sogi_a_0, signal_In->sogi_b_0 / 382 * 3, &signal_In->park_d, &signal_In->park_q, arm_sin_f32(signal_Target->theta), arm_cos_f32(signal_Target->theta));
    // 将park变换后的q送入PI控制器  输入值为设定值和采样值的误差
    pll_Pid(signal_In, config);
    // 更新theta
    signal_In->theta += (signal_In->pid_out + config->omiga) * config->Ts;
    signal_In->theta = (float)fmod(signal_In->theta, 2 * PI);
    // 反park变换
    // arm_inv_park_f32(signal_In->park_d, signal_In->park_q, &signal_In->park_inv_a, &signal_In->park_inv_b, arm_sin_f32(signal_Target->theta), arm_cos_f32(signal_Target->theta));
}
/**
 * @brief PI控制器
 * @param signal 信号指针
 * @param config 配置指针
 */
void pll_Pid(pll_Signal *signal, pll_Config *config)
{
    signal->pid_err[0] = signal->park_q - config->phase;
    signal->pid_out += config->pid_kp * (signal->pid_err[0] - signal->pid_err[1]) + config->pid_ki * signal->pid_err[0];
    signal->pid_err[1] = signal->pid_err[0];
    // 限制调参幅度，防止跑飞
    if (signal->pid_out > 50 * PI)
        signal->pid_out = 50 * PI;
    else if (signal->pid_out < -50 * PI)
        signal->pid_out = -50 * PI;
}
/**
 * @brief 回收空间
 * @param signal 信号指针
 * @param config 配置指针
 */
void pll_Clear(pll_Signal *signal, pll_Config *config)
{
    free(signal);
    free(config);
}

/**
 * @brief Sogi变换
 * @param signal 信号指针
 */
void pll_Sogi(pll_Signal *signal)
{
    signal->sogi_a_0 = signal->b0 * signal->u_0 - signal->b0 * signal->u_2 + signal->a1 * signal->sogi_a_1 + signal->a2 * signal->sogi_a_2;
    signal->sogi_b_0 = signal->b0 * signal->u_0 + 2.0f * signal->b0 * signal->u_1 + signal->b0 * signal->u_2 + signal->a1 * signal->sogi_b_1 + signal->a2 * signal->sogi_b_2;

    signal->u_2 = signal->u_1;
    signal->u_1 = signal->u_0;
    signal->sogi_a_2 = signal->sogi_a_1;
    signal->sogi_a_1 = signal->sogi_a_0;
    signal->sogi_b_2 = signal->sogi_b_1;
    signal->sogi_b_1 = signal->sogi_b_0;
}
