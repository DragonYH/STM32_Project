#ifndef __PID_H_
#define __PID_H_


typedef struct     //PID初始化参数，每次一个PID都要重新定义一个新名称。
{
	float  Kp;
	float  Ki;
	float  Kd;
	float Bias;           //偏差结果
	float Now_bias;       //保存本次偏差结果
	float Previous_bias;  //保存上次偏差结果
	float Result;         //计算结果
	float Result_max;       //输出的最大值
	float Result_min;       //输出的最小值


} PID_TypeDef;

extern    PID_TypeDef  volt;
extern    PID_TypeDef  current;

float Incremental_PID(PID_TypeDef *PID,float Measured,float Target);   //PID计算
void PID_Init(void);   //初始化结构体PID_TypeDef






#endif
