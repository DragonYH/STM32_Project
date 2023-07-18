#include "pid.h"

//PID初始化
void PID_Init(PID *pid, double kp, double ki, double kd, int counter)
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
	pid->err_int=0;
	pid->err_now=0;
	pid->err_pre=0;
	pid->feedback=counter/2;
	pid->value_now=counter/2;
	pid->counter=counter;
}
//PID计算反馈
int PID_Calculate(PID *pid, int value_set, int value_now)
{
	pid->err_now=value_set-value_now;
	pid->err_int+=pid->err_now;
	pid->feedback=pid->kp*pid->err_now+pid->ki*pid->err_int+pid->kd*(pid->err_now-pid->err_pre)+pid->counter/2;
	pid->err_pre=pid->err_now;
	if (pid->feedback>pid->counter-400) pid->feedback=pid->counter-400;
	if (pid->feedback<200) pid->feedback=200;
	return pid->feedback;
}
