#include "pidPlus.h"
#include "math.h"
/*
变积分算法
防止刚开始的时候由于误差较大导致的积分项过大——超调
*/
float VariableIntegralCoefficient(float error, float absMax, float absMin)
{
    float factor = 0.0f;

    if (fabs(error) <= absMin)
    {
        factor = 1.0f;
    }
    else if (fabs(error) > absMax)
    {
        factor = 0.1f;
    }
    else
    {
        // 这里可以设置为一个常数类似于Ki的设置，选取量是需要比Kp小1-2个数量级的
        factor = (absMax - fabs(error)) / (absMax - absMin);
    }

    return factor;
}
// 初始化
void PID_init(PID* pid, TIM_HandleTypeDef* htim, float kp, float ki, float kd, float target)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->target = target;
    pid->maximum = 0.9;
    pid->minimum = 0.1;
    pid->deadBand = 0.001;
    pid->alpha = 0;
    pid->timCount = __HAL_TIM_GET_AUTORELOAD(htim);
    pid->errorAbsMax = 5;       /*偏差绝对值最大值*/
    pid->errorAbsMin = 1;       /*偏差绝对值最小值*/
    pid->lastError = 0;
    pid->preError = 0;
}

// 带死区、抗积分饱和、梯形积分、变积分算法以及不完全微分算法的增量型PID控
// 制器。
float PID_regulator(PID* pid, float messure)
{
    float thisError; 
    float factor; // 变积分系数  0-1.0
    float increment;
    float pError, dError, iError;

    thisError = pid->target - messure; // 得到偏差值
    if (fabs(thisError) > (pid->target * pid->deadBand))
    {
        pError = thisError - pid->lastError;
        iError = (thisError + pid->lastError) / 2.0f;
        dError = thisError - 2.0f * (pid->lastError) + pid->preError;
        // 变积分系数获取
        factor = VariableIntegralCoefficient(thisError, pid->errorAbsMax, pid->errorAbsMin);
        // 计算微分项增量带不完全微分
        pid->deltaDiff = pid->kd * (1.0f - pid->alpha) * dError + pid->alpha * pid->deltaDiff;
        // 增量计算
        increment = pid->kp * pError + pid->ki * factor * iError + pid->deltaDiff; 
    }
    else
    {
        increment = 0.0f;
    }

    pid->result = pid->result + increment;

    //防止超过限制
    if (pid->result >= (pid->timCount * pid->maximum))
    {
    	//清除误差积分，防止在限制处长时间停留
    	thisError = 0;
    	pid->lastError = 0;
    	pid->preError = 0;
    	pid->deltaDiff = 0;
        pid->result = pid->timCount * pid->maximum;
    }
    if (pid->result <= (pid->timCount * pid->minimum))
    {
    	thisError = 0;
    	pid->lastError = 0;
    	pid->preError = 0;
    	pid->deltaDiff = 0;
        pid->result = pid->timCount * pid->minimum;
    }

    pid->preError = pid->lastError; // 存放偏差用于下次运算
    pid->lastError = thisError;
    return pid->result;
}
