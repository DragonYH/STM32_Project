#include "pid.h"
#include "math.h"

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
	pid->Max = max;
	pid->Min = min;
	pid->err[0] = 0.f;
	pid->err[1] = 0.f;
	pid->err[2] = 0.f;
	pid->out = 0.f;
}
/**
 * @brief 增量式PID控制器
 * @param pid pid指针
 * @param target 目标值
 * @param sample 测量值
 * @note 根据增量式离散PID公式
 * @note dac+=kp[e(0)-e(1)]+ki*e(0)+kd[e(0)-2e(1)+e(2)]
 * @note e(0)代表本次偏差
 * @note e(1)代表上一次的偏差  以此类推
 * @note result代表增量输出
 */
void pid(PID *pid, float target, float sample)
{
	pid->err[0] = target - sample;
	pid->out += pid->kp * (pid->err[0] - pid->err[1]) + pid->ki * pid->err[0] + pid->kd * (pid->err[0] - 2 * pid->err[1] + pid->err[2]);
	pid->err[2] = pid->err[1];
	pid->err[1] = pid->err[0];
	// 输出限幅
	pid->out = fminf(pid->Max, fmaxf(pid->out, pid->Min));
}
