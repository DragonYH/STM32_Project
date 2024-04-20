#ifndef __PID_H_
#define __PID_H_

typedef struct // PID初始化参数，每次一个PID都要重新定义一个新名称。
{
	float kp;
	float ki;
	float kd;
	float bias[3];	 // 偏差结果
	float result;	 // 计算结果
	float resultMax; // 输出的最大值
	float resultMin; // 输出的最小值

} PID;

float pid(PID *pid, float target, float sample);							 // PID计算
void pid_Init(PID *pid, float kp, float ki, float kd, float max, float min); // 初始化结构体PID

#endif
