#ifndef _DS18B20_H
#define _DS18B20_H

#include "main.h"

//DQ_GPIO
#define DQ_GPIO_Port  GPIOE
#define DQ_Pin        GPIO_PIN_2

/***************************** 延时函数 ************************************************************/

#include "core_delay.h"                            //包含内核延时头文件
#define HAL_Delay_Us(us)  CPU_TS_Tmr_Delay_US(us)

/****************************************************************************************************/

//总线的电平
typedef enum 
{
	DQ_LEVEL_LOW = 0,
	DQ_LEVEL_HIGHT
}DQ_LEVEL_Type;

/* 引脚写高低电平 */
#define DQ_Write(Level)     if(Level) \
										HAL_GPIO_WritePin(DQ_GPIO_Port,DQ_Pin,SET); \
						    else  \
										HAL_GPIO_WritePin(DQ_GPIO_Port,DQ_Pin,RESET); 
//读取总线电平
#define DQ_Read        HAL_GPIO_ReadPin(DQ_GPIO_Port,DQ_Pin)												

//ROM指令
#define SKIP_ROM         0xCC     //跳过 ROM
#define CONVERT_TEMP     0x44    //转换温度
#define READ_SCRATCHPAD  0xBE   //读取暂存器内容
												
/*************************相关函数************************************************/	

void DQ_GPIO_Init(void);								
uint8_t Ds18b20_Init();
float Ds18b20_Get_Temp(void);												

#endif /* _DS18B20_H */
