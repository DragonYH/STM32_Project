#ifndef PID_H
#define PID_H
#include "main.h"

typedef struct
{
	double kp,ki,kd;
	int err_now;	//当前误差
	int err_pre;	//上次误差
	int err_int;	//误差积分
	int feedback;	//输出反馈
	int value_now;	//当前采样值
	int counter;	//对应时钟计数值
}PID;
void PID_Init(PID *pid, double kp, double ki, double kd, int counter);
int PID_Calculate(PID *pid, int value_set, int value_now);


#endif
