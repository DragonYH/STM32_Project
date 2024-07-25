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
#include "user_task.h"
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

/* USER CODE END Variables */
/* Definitions for stateLED */
osThreadId_t stateLEDHandle;
const osThreadAttr_t stateLED_attributes = {
    .name = "stateLED",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for mcuTemperature */
osThreadId_t mcuTemperatureHandle;
const osThreadAttr_t mcuTemperature_attributes = {
    .name = "mcuTemperature",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for oledDisplay */
osThreadId_t oledDisplayHandle;
const osThreadAttr_t oledDisplay_attributes = {
    .name = "oledDisplay",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for dcSampling */
osThreadId_t dcSamplingHandle;
const osThreadAttr_t dcSampling_attributes = {
    .name = "dcSampling",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for circuitProtecti */
osThreadId_t circuitProtectiHandle;
const osThreadAttr_t circuitProtecti_attributes = {
    .name = "circuitProtecti",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for dcControl */
osThreadId_t dcControlHandle;
const osThreadAttr_t dcControl_attributes = {
    .name = "dcControl",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartStateLED(void *argument);
void StartMcuTemperature(void *argument);
void StartOledDisplay(void *argument);
void StartDCSampling(void *argument);
void StartCircuitProtection(void *argument);
void StartDCControl(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  UserInit();
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

  /* creation of mcuTemperature */
  mcuTemperatureHandle = osThreadNew(StartMcuTemperature, NULL, &mcuTemperature_attributes);

  /* creation of oledDisplay */
  oledDisplayHandle = osThreadNew(StartOledDisplay, NULL, &oledDisplay_attributes);

  /* creation of dcSampling */
  dcSamplingHandle = osThreadNew(StartDCSampling, NULL, &dcSampling_attributes);

  /* creation of circuitProtecti */
  circuitProtectiHandle = osThreadNew(StartCircuitProtection, NULL, &circuitProtecti_attributes);

  /* creation of dcControl */
  dcControlHandle = osThreadNew(StartDCControl, NULL, &dcControl_attributes);

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
__weak void StartStateLED(void *argument)
{
  /* USER CODE BEGIN StartStateLED */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartStateLED */
}

/* USER CODE BEGIN Header_StartMcuTemperature */
/**
 * @brief Function implementing the mcuTemperature thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMcuTemperature */
__weak void StartMcuTemperature(void *argument)
{
  /* USER CODE BEGIN StartMcuTemperature */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMcuTemperature */
}

/* USER CODE BEGIN Header_StartOledDisplay */
/**
 * @brief Function implementing the oledDisplay thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartOledDisplay */
__weak void StartOledDisplay(void *argument)
{
  /* USER CODE BEGIN StartOledDisplay */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartOledDisplay */
}

/* USER CODE BEGIN Header_StartDCSampling */
/**
 * @brief Function implementing the dcSampling thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDCSampling */
__weak void StartDCSampling(void *argument)
{
  /* USER CODE BEGIN StartDCSampling */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDCSampling */
}

/* USER CODE BEGIN Header_StartCircuitProtection */
/**
 * @brief Function implementing the circuitProtecti thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCircuitProtection */
__weak void StartCircuitProtection(void *argument)
{
  /* USER CODE BEGIN StartCircuitProtection */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCircuitProtection */
}

/* USER CODE BEGIN Header_StartDCControl */
/**
 * @brief Function implementing the dcControl thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDCControl */
__weak void StartDCControl(void *argument)
{
  /* USER CODE BEGIN StartDCControl */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDCControl */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
