/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "stdio.h"
#include "user.h"
#include "ad7606.h"
#include "spi.h"
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t text[32] = {0};
/* USER CODE END Variables */
/* Definitions for stateLED */
osThreadId_t stateLEDHandle;
const osThreadAttr_t stateLED_attributes = {
    .name = "stateLED",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for oledShow */
osThreadId_t oledShowHandle;
const osThreadAttr_t oledShow_attributes = {
    .name = "oledShow",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartStateLED(void *argument);
void StartOledShow(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  OLED_Init();
  ad7606_Init();
  ad7606_Start(&htim2, TIM_CHANNEL_1);
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of stateLED */
  stateLEDHandle = osThreadNew(StartStateLED, NULL, &stateLED_attributes);

  /* creation of oledShow */
  oledShowHandle = osThreadNew(StartOledShow, NULL, &oledShow_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartStateLED */
/**
 * @brief  Function implementing the stateLED thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartStateLED */
void StartStateLED(void *argument)
{
  /* USER CODE BEGIN StartStateLED */
  /* Infinite loop */
  for (;;)
  {
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    osDelay(100);
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    osDelay(100);
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    osDelay(250);
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    osDelay(250);
  }
  /* USER CODE END StartStateLED */
}

/* USER CODE BEGIN Header_StartOledShow */
/**
 * @brief Function implementing the oledShow thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartOledShow */
void StartOledShow(void *argument)
{
  /* USER CODE BEGIN StartOledShow */

  /* Infinite loop */
  for (;;)
  {
    sprintf((char *)text, "0: %.3f", adcValue[0]);
    OLED_ShowString(0, 0, text, 12);
    sprintf((char *)text, "1: %.3f", adcValue[1]);
    OLED_ShowString(64, 0, text, 12);
    sprintf((char *)text, "2: %.3f", adcValue[2]);
    OLED_ShowString(0, 12, text, 12);
    sprintf((char *)text, "3: %.3f", adcValue[3]);
    OLED_ShowString(64, 12, text, 12);
    sprintf((char *)text, "4: %.3f", adcValue[4]);
    OLED_ShowString(0, 24, text, 12);
    sprintf((char *)text, "5: %.3f", adcValue[5]);
    OLED_ShowString(64, 24, text, 12);
    sprintf((char *)text, "6: %.3f", adcValue[6]);
    OLED_ShowString(0, 36, text, 12);
    sprintf((char *)text, "7: %.3f", adcValue[7]);
    OLED_ShowString(64, 36, text, 12);
    // 获取当前堆栈剩余空间
    sprintf((char *)text, "stack free: %ld", uxTaskGetStackHighWaterMark(NULL));
    OLED_ShowString(0, 48, text, 12);
    OLED_Refresh();
    osDelay(100);
  }
  /* USER CODE END StartOledShow */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// GPIO触发中断
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == AD7606_BUSY_Pin)
  {
    ad7606_GetValue(&hspi2, 8, adcValue);
  }
}
/* USER CODE END Application */
