/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
extern SPI_HandleTypeDef hspi1;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define C1_Pin GPIO_PIN_3
#define C1_GPIO_Port GPIOF
#define R4_Pin GPIO_PIN_4
#define R4_GPIO_Port GPIOF
#define C2_Pin GPIO_PIN_5
#define C2_GPIO_Port GPIOF
#define R3_Pin GPIO_PIN_6
#define R3_GPIO_Port GPIOF
#define C3_Pin GPIO_PIN_7
#define C3_GPIO_Port GPIOF
#define R2_Pin GPIO_PIN_8
#define R2_GPIO_Port GPIOF
#define C4_Pin GPIO_PIN_9
#define C4_GPIO_Port GPIOF
#define R1_Pin GPIO_PIN_10
#define R1_GPIO_Port GPIOF
#define OLED_SCLK_Pin GPIO_PIN_5
#define OLED_SCLK_GPIO_Port GPIOA
#define OLED_SDIN_Pin GPIO_PIN_7
#define OLED_SDIN_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_5
#define OLED_RST_GPIO_Port GPIOC
#define OLED_DC_Pin GPIO_PIN_1
#define OLED_DC_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_11
#define OLED_CS_GPIO_Port GPIOF

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
