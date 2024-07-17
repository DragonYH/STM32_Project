#ifndef __USER_GLOBAL_H
#define __USER_GLOBAL_H

#include "ina228.h"
#include "three_phrase_pll.h"

// todo: 根据模式自动切换整流和逆变
#define Rectifier_Or_Inverter 1 // 1:整流器 0:逆变器
#define DC_V_Ctrl 1             // 1:开启直流电压控制 0:关闭直流电压控制

#define Ubase (15.f * 1.414f) // 电压基准
#define Ibase (6.f * 1.414f)  // 电流基准

#define protection_Udc (90.f) // 电压保护阈值
#define protection_Idc (10.f) // 电流保护阈值
#define protection_Uac (30.f) // 交流电压保护阈值
#define protection_Iac (10.f) // 交流电流保护阈值阈值

extern const INA228_Handle INA228_0;
extern float U;
extern float I;
extern float M;

extern pll_Signal_V *signal_V;
extern pll_Signal_I *signal_I;

// 运行状态
enum state
{
    START = 0,
    RUN = 1,
    FAULT = 2
};

extern volatile enum state runState;

#endif
