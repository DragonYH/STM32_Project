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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AMOLED_CLK_Pin GPIO_PIN_10
#define AMOLED_CLK_GPIO_Port GPIOF
#define AMOLED_TE_Pin GPIO_PIN_0
#define AMOLED_TE_GPIO_Port GPIOA
#define AMOLED_IO0_Pin GPIO_PIN_2
#define AMOLED_IO0_GPIO_Port GPIOH
#define AMOLED_IO1_Pin GPIO_PIN_3
#define AMOLED_IO1_GPIO_Port GPIOH
#define AMOLED_IM_Pin GPIO_PIN_3
#define AMOLED_IM_GPIO_Port GPIOA
#define AMOLED_PWR_Pin GPIO_PIN_4
#define AMOLED_PWR_GPIO_Port GPIOA
#define AMOLED_RST_Pin GPIO_PIN_5
#define AMOLED_RST_GPIO_Port GPIOA
#define AMOLED_IO2_Pin GPIO_PIN_9
#define AMOLED_IO2_GPIO_Port GPIOE
#define AMOLED_IO3_Pin GPIO_PIN_10
#define AMOLED_IO3_GPIO_Port GPIOE
#define AMOLED_CS_Pin GPIO_PIN_11
#define AMOLED_CS_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
