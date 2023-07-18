#include "pid.h"

//PID初始化
void PID_Init(PID *pid, double kp, double ki, double kd)
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
	pid->feedback=0;
	pid->value_now=0;
	for(int i = 0; i < 3; i++)
	{
		pid->err[i]=0;
	}
}
//PID计算反馈
//value_set:设定值
//value_now:当前采样值
int PID_Calculate(TIM_HandleTypeDef htim,PID *pid, int value_set, int value_now)
{
	pid->err[0]=value_set-value_now;	//求出当前误差
	pid->feedback=pid->kp*(pid->err[0]-pid->err[1])+pid->ki*pid->err[0]+pid->kd*(pid->err[0]-2*pid->err[1]+pid->err[2]);
	pid->err[2]=pid->err[1];
	pid->err[1]=pid->err[0];
	if(pid->feedback > 200) pid->feedback = 200;	//限制最大增量
	if(pid->feedback < -200) pid->feedback = -200;
	return pid->feedback;
}
