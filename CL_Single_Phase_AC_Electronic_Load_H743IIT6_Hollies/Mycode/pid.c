#include "pid.h"

/**
 * @brief pid参数初始化
 * @param pid pid指针
 * @param kp 比例系数
 * @param ki 积分系数
 * @param kd 微分系数
 * @param max 输出最大值
 * @param min 输出最小值
 */
void pid_Init(PID *pid, float kp, float ki, float kd, float max, float min) // PID参数初始化
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->resultMax = max;
	pid->resultMin = min;
	pid->bias[0] = 0;
	pid->bias[1] = 0;
	pid->bias[2] = 0;
}
/**
 * @brief 增量式PID控制器
 * @param pid pid指针
 * @param measured 测量值
 * @param target 目标值
 * @return pid输出值
 * @note 根据增量式离散PID公式
 * @note dac+=kp[e(0)-e(1)]+ki*e(0)+kd[e(0)-2e(1)+e(2)]
 * @note e(0)代表本次偏差
 * @note e(1)代表上一次的偏差  以此类推
 * @note result代表增量输出
 */
float pid(PID *pid, float Measured, float Target)
{
	pid->bias[0] = Target - Measured;
	pid->result += pid->kp * (pid->bias[0] - pid->bias[1]) + pid->ki * pid->bias[0] + pid->kd * (pid->bias[0] - 2 * pid->bias[1] + pid->bias[2]);
	pid->bias[2] = pid->bias[1];
	pid->bias[1] = pid->bias[0];
	// 输出限幅
	if (pid->result > pid->resultMax)
		pid->result = pid->resultMax;
	else if (pid->result < pid->resultMin)
		pid->result = pid->resultMin;

	return pid->result;
}
