#include "tim.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"
extern int V_flag,I_flag;
void V_protect(int V_threshold,int V_samping);	//过压保护：过压阈值，电压采样值
void I_protect(int I_threshold,int I_samping);	//过流保护：过流阈值，电流采样值
void P_switch(int V_threshold,int V_samping);	//开关切换：切换电压阈值，电压采样值

/*
电源默认VCC1低电平供电
默认高电平保护导通，无信号输入断开
*/
