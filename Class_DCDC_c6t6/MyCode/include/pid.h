#ifndef PID_H
#define PID_H
#include "main.h"
//PID结构体定义
typedef struct
{
	float kp,ki,kd;
	float err[3];		//err[0]为当前误差，err[1]为上一次误差，err[2]为上上次误差
	int feedback;		//输出反馈
	uint32_t value_now;	//当前采样值
}PID;
//PID初始化
void PID_Init(PID *pid, double kp, double ki, double kd);
//PID计算反馈
int PID_Calculate(TIM_HandleTypeDef htim, PID *pid, int value_set, int value_now);

#endif
