#ifndef __PLL_H
#define __PLL_H

#include "main.h"
#include "arm_math.h"
#include "pid.h"
#include "iir.h"

#define COMPARE 12000           // 计时器计数值
#define MI 0.9f                 // 调制比
#define COMPARE_MAX MI *COMPARE // 计时器最大计数值
#define COMPARE_MIN 0           // 计时器最小计数值

// sogi结构体
typedef struct SOGI
{
    float a[3]; // 输出序列alpha
    float b[3]; // 输出序列beta	滞后90度于序列alpha
    // 中间变量
    float k; // 阻尼比 典型值1.41
    float lamda;
    float x;
    float y;
    float b0;
    float a1;
    float a2;
} SOGI;
// pr结构体
typedef struct PR
{
    float kp;     // 比例系数
    float kr;     // 积分系数
    float out[3]; // 输出
    float err[3]; // 误差
    // 中间变量
    float a0;
    float a1;
    float a2;
    float b0;
    float b1;
    float b2;
} PR;
// 电压信号数据
typedef struct pll_Signal_V
{
    // 基本变量
    float input[3]; // 输入序列
    float rms;      // 电压有效值
    // park变换相关变量
    float park_d; // 有功分量
    float park_q; // 无功分量
    float theta;  // 当前角度
    float phase;  // 设置相位
    // 配置参数
    float omiga0; // 无阻尼自然频率，2*pi*频率
    float Ts;     // 采样周期
    // iir滤波器
    float iirState[4 * iirNumStages]; // IIR滤波器状态变量

    SOGI *sogi; // sogi指针
    PID *pid;   // pid指针

} pll_Signal_V;
// 电流信号数据
typedef struct pll_Signal_I
{
    // 基本变量
    float input[3]; // 输入序列
    float rms;      // 电流有效值
    // park变换相关变量
    float park_d; // 有功分量
    float park_q; // 无功分量
    // 配置参数
    float omiga0; // 无阻尼自然频率，2*pi*频率
    float omigaC; // 带宽2*pi*带宽
    float Ts;     // 采样周期
    // iir滤波器
    float iirState[4 * iirNumStages]; // IIR滤波器状态变量

    SOGI *sogi; // sogi指针
    PID *pid;   // pid指针
    PR *pr;     // pr指针
} pll_Signal_I;

void pll_Init_V(pll_Signal_V *signal, float f, uint16_t F, float Umax);
void pll_Init_I(pll_Signal_I *signal, float f, uint16_t F, float pr_kp, float pr_kr, float pi_kp, float pi_ki);
void pll_Control_V(pll_Signal_V *signal_V);
void pll_Control_I(pll_Signal_I *signal_I, pll_Signal_V *signal_V, float Uset, float Udc);
void pll_Pr(PR *signal, float target, float sample);
void pll_Sogi(float *input, SOGI *sogi);

#endif
