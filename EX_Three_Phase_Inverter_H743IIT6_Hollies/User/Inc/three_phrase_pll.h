#ifndef __THREE_PHRASE_PLL_H
#define __THREE_PHRASE_PLL_H

#include "pid.h"
#include "iir.h"
#include "main.h"
#include "arm_math.h"

typedef struct pll_Signal_Basic
{
    // 基本变量
    float input_a; // a相输入
    float input_b; // b相输入
    float rms;     // 电压有效值
    // clarke变换相关变量
    float clarke_alpha; // alpha分量
    float clarke_beta;  // beta分量
    // park变换相关变量
    float park_d; // 有功分量
    float park_q; // 无功分量
    // 配置参数
    float omiga0; // 无阻尼自然频率，2*pi*频率
    float Ts;     // 采样周期
    // iir滤波器
    float iirState[4 * iirNumStages]; // IIR滤波器状态变量

} pll_Signal_Basic;

// 电压信号数据
typedef struct pll_Signal_V
{
    // 基本变量
    pll_Signal_Basic *basic;
    float theta; // 当前角度
    // 控制参数
    PID *pid; // 锁相pid指针
} pll_Signal_V;
// 电流信号数据
typedef struct pll_Signal_I
{
    // 基本变量
    pll_Signal_Basic *basic;
    // park逆变换相关变量
    float park_inv_alpha; // 逆变换后的alpha
    float park_inv_beta;  // 逆变换后的beta
    // clarke逆变换/输出 相关变量
    float output_a; // 逆变换后的a相
    float output_b; // 逆变换后的b相
    float output_c; // 逆变换后的c相
    // 控制参数
    uint8_t CorL; // 0:感性 1:容性
    float L;      // 电感
    PID *pid_d;   // 控制电流最大值pi指针
    PID *pid_q;   // 控制相位pi指针
} pll_Signal_I;

void pll_Init_V(pll_Signal_V **signal, float f, uint16_t F, float Umax);
void pll_Control_V(pll_Signal_V *signal_V);
void pll_Init_I(pll_Signal_I **signal, float f, uint16_t F);
void pll_Control_I(pll_Signal_I *signal_I, pll_Signal_V *signal_V, float Iset, float PF);
void pll_Clarke(pll_Signal_Basic *basic);
void pll_Inv_Clarke(pll_Signal_I *signal_I);
void pll_Free_V(pll_Signal_V *signal);
void pll_Free_I(pll_Signal_I *signal);

#endif
