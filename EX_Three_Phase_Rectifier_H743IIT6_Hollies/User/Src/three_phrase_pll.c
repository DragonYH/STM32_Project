#include "three_phrase_pll.h"
#include "user_global.h"
#include "pid.h"
#include "math.h"
#include "stdlib.h"
#include "malloc.h"
#include "arm_math.h"

static void pll_Clarke(pll_Signal_Basic *basic);

/**
 * @brief 电压信号参数初始化
 * @param signal 信号指针
 * @param f 信号频率(典型值:50)
 * @param F 采样频率(典型值:20000)
 * @param Umax 交流电压峰值
 */
void pll_Init_V(pll_Signal_V **signal, float f, uint16_t F)
{
    // 分配内存空间
    (*signal) = (pll_Signal_V *)malloc(sizeof(pll_Signal_V));
    (*signal)->basic = (pll_Signal_Basic *)malloc(sizeof(pll_Signal_Basic));
    (*signal)->pid = (PID *)malloc(sizeof(PID));

    // 初始化赋值
    (*signal)->basic->input_a = 0.f;
    (*signal)->basic->input_b = 0.f;
    (*signal)->basic->input_c = 0.f;

    (*signal)->basic->rms_a = 0.f;
    (*signal)->basic->rms_b = 0.f;
    (*signal)->basic->rms_c = 0.f;

    (*signal)->basic->clarke_alpha = 0.f;
    (*signal)->basic->clarke_beta = 0.f;

    (*signal)->basic->park_d = 0.f;
    (*signal)->basic->park_q = 0.f;

    (*signal)->theta = 0.f;
    (*signal)->basic->omiga0 = 2 * PI * f; // f典型值50
    (*signal)->basic->Ts = 1.f / F;        // F典型值20000

    // 初始化pid参数
    float ki = (*signal)->basic->omiga0 * (*signal)->basic->omiga0;
    float kp = sqrt(2) * sqrt(ki);
    pid_Init((*signal)->pid, kp, ki, 0, 50 * PI, -20 * PI);
}
/**
 * @brief 电流信号参数初始化
 * @param signal 信号指针
 * @param f 信号频率(典型值:50)
 * @param F 采样频率(典型值:20000)
 * @param pi_kp PI控制器kp参数
 * @param pi_ki PI控制器ki参数
 */
void pll_Init_I(pll_Signal_I **signal, float f, uint16_t F)
{
    (*signal) = (pll_Signal_I *)malloc(sizeof(pll_Signal_I));
    (*signal)->basic = (pll_Signal_Basic *)malloc(sizeof(pll_Signal_Basic));

    (*signal)->pid_d = (PID *)malloc(sizeof(PID));
    (*signal)->pid_q = (PID *)malloc(sizeof(PID));
    // 初始化赋值
    (*signal)->basic->input_a = 0.f;
    (*signal)->basic->input_b = 0.f;
    (*signal)->basic->input_c = 0.f;

    (*signal)->basic->rms_a = 0.f;
    (*signal)->basic->rms_b = 0.f;
    (*signal)->basic->rms_c = 0.f;

    (*signal)->basic->clarke_alpha = 0.f;
    (*signal)->basic->clarke_beta = 0.f;

    (*signal)->basic->park_d = 0.f;
    (*signal)->basic->park_q = 0.f;

    (*signal)->park_inv_alpha = 0.f;
    (*signal)->park_inv_beta = 0.f;

    (*signal)->basic->omiga0 = 2.f * PI * f; // f典型值50
    (*signal)->basic->Ts = 1.f / F;          // F典型值20000

    (*signal)->CorL = 0;   // 0:感性 1:容性
    (*signal)->L = 0.001f; // 1mH

    // 在调整取值范围时看实际输出值逐渐逼近，防止上电瞬间电流过大
    pid_Init((*signal)->pid_d, 1.8f, 0.03f, 0, 0.2f, -0.4f);
    pid_Init((*signal)->pid_q, 1.8f, 0.03f, 0, 0.2f, -0.2f);
}
/**
 * @brief 电压锁相控制
 * @param signal_V 电压信号指针
 */
void pll_Control_V(pll_Signal_V *signal_V)
{
    // 先对信号进行clarke变换
    pll_Clarke(signal_V->basic);

    // 再对信号sogi变换后的信号进行park变换
    arm_park_f32(signal_V->basic->clarke_alpha, signal_V->basic->clarke_beta, &signal_V->basic->park_d, &signal_V->basic->park_q, arm_sin_f32(signal_V->theta), arm_cos_f32(signal_V->theta));

    // 将park变换后的q送入PI控制器  输入值为设定值和采样值的误差
    pid(signal_V->pid, signal_V->basic->park_q, 0); // pid的输出值为旋转坐标系角速度

    // 更新theta
    signal_V->theta += (signal_V->pid->out + signal_V->basic->omiga0) * signal_V->basic->Ts;
    signal_V->theta = (float)fmod(signal_V->theta, 2 * PI);
}
/**
 * @brief 电流内环控制
 * @param signal_I 电流信号指针
 * @param signal_V 电压信号指针
 * @param Iset 电流设定值(有效值)
 * @param PF 功率因数
 */
void pll_Control_I(pll_Signal_I *signal_I, pll_Signal_V *signal_V, float Iset, float PF)
{
    // 先对信号进行clarke变换
    pll_Clarke(signal_I->basic);

    // 在电压的系上得出电流的dq值
    float sinTheta = arm_sin_f32(signal_V->theta);
    float cosTheta = arm_cos_f32(signal_V->theta);

    arm_park_f32(signal_I->basic->clarke_alpha, signal_I->basic->clarke_beta, &signal_I->basic->park_d, &signal_I->basic->park_q, sinTheta, cosTheta);

    // PI控制
    float PFTheta = asinf(PF);

    float Ivalue = Iset * 1.414f / Ibase;
    pid(signal_I->pid_d, Ivalue * arm_sin_f32(PFTheta), signal_I->basic->park_d); // 电流大小

    float Iphase = Ivalue * arm_cos_f32(PFTheta) * (signal_I->CorL ? 1 : -1);
    pid(signal_I->pid_q, Iphase, signal_I->basic->park_q); // 电流相位

    // 解耦调制
    float Uabd = signal_V->basic->park_d - signal_I->pid_d->out + signal_I->basic->park_q * signal_I->basic->omiga0 * signal_I->L;
    float Uabq = signal_V->basic->park_q - signal_I->pid_q->out - signal_I->basic->park_d * signal_I->basic->omiga0 * signal_I->L;

    // 限幅
    Uabd = fmaxf(-0.95f, fminf(0.95f, Uabd));
    Uabq = fmaxf(-0.95f, fminf(0.95f, Uabq));

    // park逆变换
    arm_inv_park_f32(Uabd, Uabq, &signal_I->park_inv_alpha, &signal_I->park_inv_beta, sinTheta, cosTheta);
}
/**
 * @brief clarke变换
 * @param basic 信号基本变量指针
 */
static void pll_Clarke(pll_Signal_Basic *basic)
{
    basic->clarke_alpha = (2.f * basic->input_a - basic->input_b - basic->input_c) / 3.f;
    basic->clarke_beta = 0.57735026f * (basic->input_b - basic->input_c);
}
/**
 * @brief 释放内存
 * @param signal 信号指针
 */
void pll_Free_V(pll_Signal_V *signal)
{
    free(signal->basic);
    free(signal->pid);
    free(signal);
}
/**
 * @brief 释放内存
 * @param signal 信号指针
 */
void pll_Free_I(pll_Signal_I *signal)
{
    free(signal->basic);
    free(signal->pid_d);
    free(signal->pid_q);
    free(signal);
}
