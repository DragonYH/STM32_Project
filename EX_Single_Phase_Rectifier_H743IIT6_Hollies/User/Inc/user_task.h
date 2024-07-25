#ifndef __USER_TASK_H
#define __USER_TASK_H

/* 保护阈值  */
#define protection_Udc (80.f) /* 直流电压保护阈值 */
#define protection_Idc (10.f) /* 直流电流保护阈值 */
#define protection_Uac (30.f) /* 交流电压保护阈值 */
#define protection_Iac (10.f) /* 交流电流保护阈值 */

/* 运行状态 */
enum state
{
    START = 0,
    RUN,
    FAULT
};

extern enum state deviceState;
extern float Itarget;

void UserInit(void);
void StartStateLED(void *argument);
void StartMcuTemperature(void *argument);
void StartOledDisplay(void *argument);
void StartDCSampling(void *argument);
void StartCircuitProtection(void *argument);
void StartDCControl(void *argument);

#endif
