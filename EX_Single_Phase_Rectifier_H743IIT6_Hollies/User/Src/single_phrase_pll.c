/*
 * @Author       : DragonYH 1016633827@qq.com
 * @Date         : 2024-07-20 20:10:11
 * @LastEditors  : DragonYH 1016633827@qq.com
 * @LastEditTime : 2024-07-21 19:08:19
 * @FilePath     : \EX_Single_Phase_Rectifier_H743IIT6_Hollies\User\Src\single_phrase_pll.c
 * @Description  : 单相锁相环控制
 *
 * Copyright (c) 2024 by DragonYH, All Rights Reserved.
 */

#include "single_phrase_pll.h"
#include "pid.h"
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include "arm_math.h"

static void pll_Sogi(SOGI *sogi, float *input);

/**
 * @brief 电压信号参数初始化
 * @param signal 信号指针
 * @param f 信号频率(典型值:50)
 * @param F 采样频率(典型值:20000)
 * @param Umax 交流电压峰值
 */
void pll_Init_V(pll_Signal_V **signal, float f, uint16_t F)
{
    /* 分配内存空间 */
    (*signal) = (pll_Signal_V *)malloc(sizeof(pll_Signal_V));
    (*signal)->basic = (pll_Signal_Basic *)malloc(sizeof(pll_Signal_Basic));
    (*signal)->pid = (PID *)malloc(sizeof(PID));
    (*signal)->basic->sogi = (SOGI *)malloc(sizeof(SOGI));

    /* 初始化赋值 */
    (*signal)->basic->input[0] = 0.f;
    (*signal)->basic->input[1] = 0.f;
    (*signal)->basic->input[2] = 0.f;

    (*signal)->basic->rms = 0.f;

    (*signal)->basic->park_d = 0.f;
    (*signal)->basic->park_q = 0.f;

    (*signal)->theta = 0.f;
    (*signal)->basic->omiga0 = 2 * PI * f; /* f典型值50 */
    (*signal)->basic->Ts = 1.f / F;        /* F典型值20000 */

    /* 初始化pid参数 */
    float ki = (*signal)->basic->omiga0 * (*signal)->basic->omiga0;
    float kp = sqrt(2) * sqrt(ki);
    pid_Init((*signal)->pid, kp, ki, 0, 50 * PI, -20 * PI);

    /* 计算sogi中间量 */
    (*signal)->basic->sogi->k = 1.414f;
    (*signal)->basic->sogi->lamda = 0.5f * (*signal)->basic->omiga0 * (*signal)->basic->Ts;
    (*signal)->basic->sogi->x = 2.f * (*signal)->basic->sogi->k * (*signal)->basic->omiga0 * (*signal)->basic->Ts;
    (*signal)->basic->sogi->y = (*signal)->basic->omiga0 * (*signal)->basic->Ts * (*signal)->basic->omiga0 * (*signal)->basic->Ts;
    (*signal)->basic->sogi->b0 = (*signal)->basic->sogi->x / ((*signal)->basic->sogi->x + (*signal)->basic->sogi->y + 4);
    (*signal)->basic->sogi->a1 = (8 - 2.f * (*signal)->basic->sogi->y) / ((*signal)->basic->sogi->x + (*signal)->basic->sogi->y + 4);
    (*signal)->basic->sogi->a2 = ((*signal)->basic->sogi->x - (*signal)->basic->sogi->y - 4) / ((*signal)->basic->sogi->x + (*signal)->basic->sogi->y + 4);
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
    (*signal)->basic->sogi = (SOGI *)malloc(sizeof(SOGI));

    (*signal)->pid_d = (PID *)malloc(sizeof(PID));
    (*signal)->pid_q = (PID *)malloc(sizeof(PID));
    /* 初始化赋值 */
    (*signal)->basic->input[0] = 0.f;
    (*signal)->basic->input[1] = 0.f;
    (*signal)->basic->input[2] = 0.f;

    (*signal)->basic->rms = 0.f;

    (*signal)->basic->park_d = 0.f;
    (*signal)->basic->park_q = 0.f;

    (*signal)->park_inv_alpha = 0.f;
    (*signal)->park_inv_beta = 0.f;

    (*signal)->basic->omiga0 = 2.f * PI * f; /* f典型值50 */
    (*signal)->basic->Ts = 1.f / F;          /* F典型值20000 */

    (*signal)->CorL = 0;   /* 0:感性 1:容性 */
    (*signal)->L = 0.001f; /* 1mH */

    /* 在调整取值范围时看实际输出值逐渐逼近，防止上电瞬间电流过大 */
    pid_Init((*signal)->pid_d, 1.8f, 0.03f, 0, 0.2f, -0.4f);
    pid_Init((*signal)->pid_q, 1.8f, 0.03f, 0, 0.2f, -0.2f);

    /* 计算sogi中间量 */
    (*signal)->basic->sogi->k = 1.414f;
    (*signal)->basic->sogi->lamda = 0.5f * (*signal)->basic->omiga0 * (*signal)->basic->Ts;
    (*signal)->basic->sogi->x = 2.f * (*signal)->basic->sogi->k * (*signal)->basic->omiga0 * (*signal)->basic->Ts;
    (*signal)->basic->sogi->y = (*signal)->basic->omiga0 * (*signal)->basic->Ts * (*signal)->basic->omiga0 * (*signal)->basic->Ts;
    (*signal)->basic->sogi->b0 = (*signal)->basic->sogi->x / ((*signal)->basic->sogi->x + (*signal)->basic->sogi->y + 4);
    (*signal)->basic->sogi->a1 = (8 - 2.f * (*signal)->basic->sogi->y) / ((*signal)->basic->sogi->x + (*signal)->basic->sogi->y + 4);
    (*signal)->basic->sogi->a2 = ((*signal)->basic->sogi->x - (*signal)->basic->sogi->y - 4) / ((*signal)->basic->sogi->x + (*signal)->basic->sogi->y + 4);
}

/**
 * @brief 电压锁相控制
 * @param signal_V 电压信号指针
 */
void pll_Control_V(pll_Signal_V *signal_V)
{
    /* 对信号先进行sogi变换，得到两个相位相差90度的信号 */
    pll_Sogi(signal_V->basic->sogi, signal_V->basic->input);

    /* 再对信号sogi变换后的信号进行park变换 */
    float sinTheta = arm_sin_f32(signal_V->theta);
    float cosTheta = arm_cos_f32(signal_V->theta);
    arm_park_f32(signal_V->basic->sogi->alpha[0], signal_V->basic->sogi->beta[0], &signal_V->basic->park_d, &signal_V->basic->park_q, sinTheta, cosTheta);

    /* 将park变换后的q送入PI控制器  输入值为设定值和采样值的误差 */
    pid(signal_V->pid, signal_V->basic->park_q, 0); /* pid的输出值为旋转坐标系角速度 */

    /* 更新theta */
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
    /* 对信号先进行sogi变换，得到两个相位相差90度的信号 */
    pll_Sogi(signal_I->basic->sogi, signal_I->basic->input);

    /* 在电压的系上得出电流的dq值 */
    float sinTheta = arm_sin_f32(signal_V->theta);
    float cosTheta = arm_cos_f32(signal_V->theta);

    arm_park_f32(signal_I->basic->sogi->alpha[0], signal_I->basic->sogi->beta[0], &signal_I->basic->park_d, &signal_I->basic->park_q, sinTheta, cosTheta);

    /* PI控制 */
    float PFTheta = asinf(PF);

    float Ivalue = Iset * 1.414f / Ibase;
    pid(signal_I->pid_d, Ivalue * arm_sin_f32(PFTheta), signal_I->basic->park_d); /* 电流大小 */

    float Iphase = Ivalue * arm_cos_f32(PFTheta) * (signal_I->CorL ? 1 : -1);
    pid(signal_I->pid_q, Iphase, signal_I->basic->park_q); /* 电流相位 */

    /* 解耦调制 */
    float Uabd = signal_V->basic->park_d - signal_I->pid_d->out + signal_I->basic->park_q * signal_I->basic->omiga0 * signal_I->L;
    float Uabq = signal_V->basic->park_q - signal_I->pid_q->out - signal_I->basic->park_d * signal_I->basic->omiga0 * signal_I->L;

    /* park逆变换 */
    arm_inv_park_f32(Uabd, Uabq, &signal_I->park_inv_alpha, &signal_I->park_inv_beta, sinTheta, cosTheta);

    /* 限幅 */
    signal_I->park_inv_alpha = fmaxf(-0.95f, fminf(0.95f, signal_I->park_inv_alpha));
    signal_I->park_inv_beta = fmaxf(-0.95f, fminf(0.95f, signal_I->park_inv_beta));
}

/**
 * @brief Sogi变换
 * @param input 输入信号
 * @param sogi sogi指针
 */
static void pll_Sogi(SOGI *sogi, float *input)
{
    sogi->alpha[0] = sogi->b0 * input[0] - sogi->b0 * input[2] + sogi->a1 * sogi->alpha[1] + sogi->a2 * sogi->alpha[2];
    sogi->beta[0] = sogi->lamda * sogi->b0 * (input[0] + 2 * input[1] + input[2]) + sogi->a1 * sogi->beta[1] + sogi->a2 * sogi->beta[2];

    input[2] = input[1];
    input[1] = input[0];
    sogi->alpha[2] = sogi->alpha[1];
    sogi->alpha[1] = sogi->alpha[0];
    sogi->beta[2] = sogi->beta[1];
    sogi->beta[1] = sogi->beta[0];
}

/**
 * @brief 释放内存
 * @param signal 信号指针
 */
void pll_Free_V(pll_Signal_V *signal)
{
    free(signal->basic->sogi);
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
    free(signal->basic->sogi);
    free(signal->basic);
    free(signal->pid_d);
    free(signal->pid_q);
    free(signal);
}
