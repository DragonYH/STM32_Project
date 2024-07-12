#ifndef __USER_GLOBAL_H
#define __USER_GLOBAL_H

#include "ina228.h"
#include "three_phrase_pll.h"

// todo: 根据模式自动切换整流和逆变
#define RectifierOrInverter 1 // 1:整流器 0:逆变器

#define Ubase (30.f * 1.414f) // 电压基准
#define Ibase (5.f * 1.414f)  // 电流基准

#define protection_Udc (90.f) // 电压保护阈值
#define protection_Idc (10.f) // 电流保护阈值
#define protection_Uac (40.f) // 交流电压保护阈值
#define protection_Iac (10.f) // 交流电流保护阈值阈值

extern const INA228_Handle INA228_0;
extern float U;
extern float I;
extern float M;

extern pll_Signal_V *signal_V;
extern pll_Signal_I *signal_I;

extern uint8_t runState;

#endif
