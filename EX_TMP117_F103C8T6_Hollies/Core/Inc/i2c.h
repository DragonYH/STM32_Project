/* USER CODE BEGIN Header */
/**
  ******************************************************************************
<<<<<<<< HEAD:TP_OLED_H743IIT6_Hollies/Core/Inc/adc.h
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
========
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
>>>>>>>> f657530a52697faa9852737ea3f404cec24ed99d:EX_TMP117_F103C8T6_Hollies/Core/Inc/i2c.h
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
<<<<<<<< HEAD:TP_OLED_H743IIT6_Hollies/Core/Inc/adc.h
#ifndef __ADC_H__
#define __ADC_H__
========
#ifndef __I2C_H__
#define __I2C_H__
>>>>>>>> f657530a52697faa9852737ea3f404cec24ed99d:EX_TMP117_F103C8T6_Hollies/Core/Inc/i2c.h

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

<<<<<<<< HEAD:TP_OLED_H743IIT6_Hollies/Core/Inc/adc.h
extern ADC_HandleTypeDef hadc3;
========
extern I2C_HandleTypeDef hi2c1;
>>>>>>>> f657530a52697faa9852737ea3f404cec24ed99d:EX_TMP117_F103C8T6_Hollies/Core/Inc/i2c.h

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

<<<<<<<< HEAD:TP_OLED_H743IIT6_Hollies/Core/Inc/adc.h
void MX_ADC3_Init(void);
========
void MX_I2C1_Init(void);
>>>>>>>> f657530a52697faa9852737ea3f404cec24ed99d:EX_TMP117_F103C8T6_Hollies/Core/Inc/i2c.h

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

<<<<<<<< HEAD:TP_OLED_H743IIT6_Hollies/Core/Inc/adc.h
#endif /* __ADC_H__ */
========
#endif /* __I2C_H__ */
>>>>>>>> f657530a52697faa9852737ea3f404cec24ed99d:EX_TMP117_F103C8T6_Hollies/Core/Inc/i2c.h

