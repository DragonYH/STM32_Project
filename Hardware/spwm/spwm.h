#ifndef __SPWM_H
#define __SPWM_H

#include "main.h"
#include "math.h"
#include "tim.h"
#include "Flash.h"
#include "dwt_delay.h"

#define COUNT 200 //点的个数
#define TIM_spwm htim3 //spwm对应定时器
#define TIM_spwm_ TIM3  //spwm对应时钟
#define TIM_spwm_HZ 84000000 //spwm对应定时器频率
#define TIM_pwm htim4 //pwm对应定时器
#define TIM_pwm_ TIM4 //pwm对应时钟
#define TIM_pwm_HZ 84000000 //pwm对应定时器频率
/**
 * @brief spwm相关数值
 * 
 * @param[in] f 载波比
 * @param[in] M 调制度
 */
typedef struct spwm_data
{
    double f;
    double M;
    int HZ;
    int pre_HZ;
    int i;
    int turn;
}spwm_data;

void spwm_value_Init(void);
void spwm_Init(void);
void spwm_Unipolarity(TIM_HandleTypeDef *htim);
void spwm_Bipolar(TIM_HandleTypeDef *htim);
void change_HZ(void);
void Restart(void);

extern int spwm_value_Unipolarity[COUNT];
extern int spwm_value_Bipolar[COUNT];
extern spwm_data spwm;


#endif
