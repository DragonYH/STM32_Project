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
#include "user_global.h"
#include "ad7606.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "ina228.h"
#include "arm_math.h"
#include "three_phrase_pll.h"
#include "dac.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "pid.h"
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
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for oledShow */
osThreadId_t oledShowHandle;
const osThreadAttr_t oledShow_attributes = {
  .name = "oledShow",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for dcSamp */
osThreadId_t dcSampHandle;
const osThreadAttr_t dcSamp_attributes = {
  .name = "dcSamp",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for usartDebug */
osThreadId_t usartDebugHandle;
const osThreadAttr_t usartDebug_attributes = {
  .name = "usartDebug",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for acVControl */
osThreadId_t acVControlHandle;
const osThreadAttr_t acVControl_attributes = {
  .name = "acVControl",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void appOLEDShow();
void appTaskStackShow();
void appACVControl();
/* USER CODE END FunctionPrototypes */

void StartStateLED(void *argument);
void StartOledShow(void *argument);
void StartDcSamp(void *argument);
void StartUsartDebug(void *argument);
void StartACVContorl(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  OLED_Init();
  ad7606_Init();
  INA228_config(INA228_0);
  pll_Init_V(&signal_V, 50, 20000, 10.f);
  pll_Init_I(&signal_I, 50, 20000);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
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

  /* creation of dcSamp */
  dcSampHandle = osThreadNew(StartDcSamp, NULL, &dcSamp_attributes);

  /* creation of usartDebug */
  usartDebugHandle = osThreadNew(StartUsartDebug, NULL, &usartDebug_attributes);

  /* creation of acVControl */
  acVControlHandle = osThreadNew(StartACVContorl, NULL, &acVControl_attributes);

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
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartStateLED */
  /* Infinite loop */
  for (;;)
  {
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    osDelay(100);
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    osDelay(200);
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    osDelay(100);
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    osDelay(200);
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_RESET);
    osDelay(1000);
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
    appOLEDShow();
    osDelay(100);
  }
  /* USER CODE END StartOledShow */
}

/* USER CODE BEGIN Header_StartDcSamp */
/**
 * @brief Function implementing the dcSamp thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDcSamp */
void StartDcSamp(void *argument)
{
  /* USER CODE BEGIN StartDcSamp */
  /* Infinite loop */
  for (;;)
  {
    U = INA228_getVBUS_V(INA228_0);
    I = INA228_getCURRENT_A(INA228_0);
    osDelay(10);
  }
  /* USER CODE END StartDcSamp */
}

/* USER CODE BEGIN Header_StartUsartDebug */
/**
 * @brief Function implementing the usartDebug thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartUsartDebug */
void StartUsartDebug(void *argument)
{
  /* USER CODE BEGIN StartUsartDebug */
  static uint8_t text[32] = {0};
  /* Infinite loop */
  for (;;)
  {
    sprintf((char *)text, "%5.2f %5.2f\n", signal_V->basic->rms_a, signal_V->basic->input_a);
    CDC_Transmit_FS(text, 32);
    memset(text, 0, 32);
    osDelay(500);
  }
  /* USER CODE END StartUsartDebug */
}

/* USER CODE BEGIN Header_StartACVContorl */
/**
 * @brief Function implementing the acVControl thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartACVContorl */
void StartACVContorl(void *argument)
{
  /* USER CODE BEGIN StartACVContorl */
  /* Infinite loop */
  for (;;)
  {
    appACVControl();
    osDelay(25);
  }
  /* USER CODE END StartACVContorl */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
 * @brief OLED显示
 */
void appOLEDShow()
{
  static uint8_t text[32] = {0};
  sprintf((char *)text, "Ua: %5.2f Ub: %5.2f", signal_V->basic->rms_a, signal_V->basic->rms_b);
  OLED_ShowString(0, 0, text, 12);
  sprintf((char *)text, "Uc: %5.2f Ia: %5.2f", signal_V->basic->rms_c, signal_I->basic->rms_a);
  OLED_ShowString(0, 12, text, 12);
  sprintf((char *)text, "Ib: %5.2f Ic: %5.2f", signal_I->basic->rms_b, signal_I->basic->rms_c);
  OLED_ShowString(0, 24, text, 12);
  sprintf((char *)text, "U: %5.2f I: %5.2f", U, I);
  OLED_ShowString(0, 36, text, 12);
  float n = (signal_V->basic->rms_a * signal_I->basic->rms_a + signal_V->basic->rms_b * signal_I->basic->rms_b + signal_V->basic->rms_c * signal_I->basic->rms_c) / (U * I) * 100.f;
  if (n > 100.f)
  {
    n = 100.f;
  }
  else if (n < 0.f)
  {
    n = 0.f;
  }
  sprintf((char *)text, "cnt: %4ld n: %5.2f%%", __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1), n);
  OLED_ShowString(0, 48, text, 12);
  OLED_Refresh();
}

/**
 * @brief 任务剩余堆栈输出
 */
void appTaskStackShow()
{
  osThreadId_t currentTaskId[] = {stateLEDHandle, oledShowHandle, dcSampHandle, usartDebugHandle};
  osThreadAttr_t currentTaskAttr[] = {stateLED_attributes, oledShow_attributes, dcSamp_attributes, usartDebug_attributes};
  static int i = 0;

  static uint8_t text[32] = {0};
  sprintf((char *)text, "%d.%-10s: %4ld / %4ld \r\n", i, pcTaskGetTaskName(currentTaskId[i]), uxTaskGetStackHighWaterMark(currentTaskId[i]), currentTaskAttr[i].stack_size);
  CDC_Transmit_FS(text, 32);
  memset(text, 0, 32);
  i++;
  if (i >= 4)
  {
    i = 0;
  }
}

/**
 * @brief 交流逆变电压控制
 */
void appACVControl()
{
  static PID pidACV;
  pid_Init(&pidACV, 0.01f, 0.1f, 0, 1.f, 0.f);
  pid(&pidACV, 5.f, signal_V->basic->rms_a);
  M = pidACV.out;
}

/* USER CODE END Application */

