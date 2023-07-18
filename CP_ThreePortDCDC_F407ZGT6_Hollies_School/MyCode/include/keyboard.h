#ifndef __keyboard_H__
#define __keyboard_H__

#include "main.h"
#include "stm32f4xx_hal.h"
#include <string.h>

char KEY_SCAN(void);
char KEY_ROW_SCAN(void);
void HW_KEY_FUNCTION(void);

#define KEY_CLO0_OUT_LOW  HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_RESET)  
#define KEY_CLO1_OUT_LOW  HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_RESET)  
#define KEY_CLO2_OUT_LOW  HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_RESET) 
#define KEY_CLO3_OUT_LOW  HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin , GPIO_PIN_RESET) 

#define KEY_CLO0_OUT_HIGH  HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_SET) 
#define KEY_CLO1_OUT_HIGH  HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_SET)
#define KEY_CLO2_OUT_HIGH  HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET)
#define KEY_CLO3_OUT_HIGH  HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET)

#endif /* __keyboard_H__ */
