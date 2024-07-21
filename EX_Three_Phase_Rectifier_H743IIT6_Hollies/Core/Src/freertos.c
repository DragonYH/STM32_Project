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
// #include "oled.h"
#include "three_phrase_pll.h"
#include "user_global.h"
#include "ad7606.h"
#include "ina228.h"
#include "pid.h"
#include "oled.h"
#include "stdio.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "arm_math.h"
#include "dac.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USER_DEBUG 0
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
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for oledShow */
osThreadId_t oledShowHandle;
const osThreadAttr_t oledShow_attributes = {
    .name = "oledShow",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for dcSamp */
osThreadId_t dcSampHandle;
const osThreadAttr_t dcSamp_attributes = {
    .name = "dcSamp",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityHigh1,
};
/* Definitions for usartDebug */
osThreadId_t usartDebugHandle;
const osThreadAttr_t usartDebug_attributes = {
    .name = "usartDebug",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for acVControl */
osThreadId_t acVControlHandle;
const osThreadAttr_t acVControl_attributes = {
    .name = "acVControl",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for circuitProtect */
osThreadId_t circuitProtectHandle;
const osThreadAttr_t circuitProtect_attributes = {
    .name = "circuitProtect",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};
/* Definitions for dcVControl */
osThreadId_t dcVControlHandle;
const osThreadAttr_t dcVControl_attributes = {
    .name = "dcVControl",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void appOLEDShow();
#if USER_DEBUG
static void appTaskStackShow();
#endif
#if !Rectifier_Or_Inverter
static void appACVControl();
#endif
/* USER CODE END FunctionPrototypes */

void StartStateLED(void *argument);
void StartOledShow(void *argument);
void StartDcSamp(void *argument);
void StartUsartDebug(void *argument);
void StartACVContorl(void *argument);
void StartCircuitProtect(void *argument);
void StartDCVControl(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  ad7606_Init();
  pll_Init_V(&signal_V, 50, 20000);
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

  /* creation of circuitProtect */
  circuitProtectHandle = osThreadNew(StartCircuitProtect, NULL, &circuitProtect_attributes);

  /* creation of dcVControl */
  dcVControlHandle = osThreadNew(StartDCVControl, NULL, &dcVControl_attributes);

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
    switch (runState)
    {
    case START: // 启动状态
      HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
      break;
    case RUN: // 正常运行
      HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
      osDelay(100);
      HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
      osDelay(200);
      HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
      osDelay(100);
      HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
      osDelay(200);
      HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET);
      osDelay(1000);
      break;
    case FAULT: // 保护状态
      HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
      osDelay(100);
      break;
    default:
      break;
    }
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
  OLED_Init();
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
  INA228_config(INA228_0);
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
  /* Infinite loop */
  for (;;)
  {
#if USER_DEBUG
    appTaskStackShow();
#else
    vTaskDelete(NULL);
#endif
    osDelay(100);
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
#if Rectifier_Or_Inverter
    vTaskDelete(NULL);
#else
    appACVControl();
#endif
    osDelay(25);
  }
  /* USER CODE END StartACVContorl */
}

/* USER CODE BEGIN Header_StartCircuitProtect */
/**
 * @brief Function implementing the circuitProtect thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartCircuitProtect */
void StartCircuitProtect(void *argument)
{
  /* USER CODE BEGIN StartCircuitProtect */
  /* Infinite loop */
  for (;;)
  {
    if (U > protection_Udc || I > protection_Idc ||
        signal_V->basic->rms_a > protection_Uac || signal_I->basic->rms_a > protection_Iac ||
        signal_V->basic->rms_b > protection_Uac || signal_I->basic->rms_b > protection_Iac ||
        signal_V->basic->rms_c > protection_Uac || signal_I->basic->rms_c > protection_Iac)
    {
      runState = FAULT;
      osDelay(1000);
      HAL_GPIO_WritePin(IR2104_SD_GPIO_Port, IR2104_SD_Pin, GPIO_PIN_RESET);
    }
    else if (runState == START &&
             runState != FAULT &&
             fabs(signal_V->basic->park_q) < 0.02f &&
             signal_V->basic->rms_a > 5.f &&
             signal_I->basic->rms_a > 0.5f)
    {
      osDelay(1000); // 延迟时间是否合适需根据具体情况调整
      // 防止上电瞬间误判
      if (runState == START &&
          runState != FAULT &&
          fabs(signal_V->basic->park_q) < 0.02f &&
          signal_V->basic->rms_a > 5.f &&
          signal_I->basic->rms_a > 0.5f)
      {
        runState = RUN;
        HAL_GPIO_WritePin(IR2104_SD_GPIO_Port, IR2104_SD_Pin, GPIO_PIN_SET);
      }
    }
    osDelay(10);
  }
  /* USER CODE END StartCircuitProtect */
}

/* USER CODE BEGIN Header_StartDCVControl */
/**
 * @brief Function implementing the dcVControl thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDCVControl */
void StartDCVControl(void *argument)
{
  /* USER CODE BEGIN StartDCVControl */
  static PID pidDCV;
  pid_Init(&pidDCV, 0.1f, 0.01f, 0, 2.5f, 0.5f);
  /* Infinite loop */
  for (;;)
  {
#if !DC_V_Ctrl
    vTaskDelete(NULL);
#else
    pid(&pidDCV, Utarget, U);
    Iref = pidDCV.out;
#endif
    osDelay(20);
  }
  /* USER CODE END StartDCVControl */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
 * @brief OLED显示
 */
static void appOLEDShow()
{
  static uint8_t text[32] = {0};
  // 显示交流电压电流有效值
  sprintf((char *)text, "Ua: %5.2f Ub: %5.2f", signal_V->basic->rms_a, signal_V->basic->rms_b);
  OLED_ShowString(0, 0, text, 12);
  sprintf((char *)text, "Uc: %5.2f Ia: %5.2f", signal_V->basic->rms_c, signal_I->basic->rms_a);
  OLED_ShowString(0, 12, text, 12);
  sprintf((char *)text, "Ib: %5.2f Ic: %5.2f", signal_I->basic->rms_b, signal_I->basic->rms_c);
  // 显示直流电压电流
  OLED_ShowString(0, 24, text, 12);
  sprintf((char *)text, "U: %5.2f I: %5.2f", U, I);
  OLED_ShowString(0, 36, text, 12);
  // 计算效率
  float n = (signal_V->basic->rms_a * signal_I->basic->rms_a + signal_V->basic->rms_b * signal_I->basic->rms_b + signal_V->basic->rms_c * signal_I->basic->rms_c) / fabsf(U * I) * 100.f;
  n = (n > 100.f) ? 100.f : ((n < 0.f) ? 0.f : n);
  // 显示当前运行状态
  const char *stateText;
  switch (runState)
  {
  case START:
    stateText = "START";
    break;
  case RUN:
    stateText = "RUN";
    break;
  case FAULT:
    stateText = "FAULT";
    break;
  default:
    stateText = "UNKNOWN";
    break;
  }
  sprintf((char *)text, "n: %5.2f%% %s", n, stateText);
  OLED_ShowString(0, 48, text, 12);
  OLED_Refresh();
}

#if USER_DEBUG
/**
 * @brief 任务剩余堆栈输出
 */
static void appTaskStackShow()
{
  osThreadId_t currentTaskId[] = {stateLEDHandle, oledShowHandle, dcSampHandle, usartDebugHandle, acVControlHandle, circuitProtectHandle};
  osThreadAttr_t currentTaskAttr[] = {stateLED_attributes, oledShow_attributes, dcSamp_attributes, usartDebug_attributes, acVControl_attributes, circuitProtect_attributes};
  static int i = 0;

  static uint8_t text[32] = {0};
  sprintf((char *)text, "%d.%-10s: %4ld / %4ld \r\n", i, pcTaskGetTaskName(currentTaskId[i]), uxTaskGetStackHighWaterMark(currentTaskId[i]), currentTaskAttr[i].stack_size);
  CDC_Transmit_FS(text, 32);
  memset(text, 0, 32);
  if (++i >= sizeof(currentTaskId) / sizeof(currentTaskId[0]))
  {
    i = 0;
  }
}
#endif

#if !Rectifier_Or_Inverter
/**
 * @brief 交流逆变电压控制
 */
static void appACVControl()
{
  static PID pidACV;
  pid_Init(&pidACV, 0.01f, 0.1f, 0, 1.f, 0.f);
  pid(&pidACV, 5.f, signal_V->basic->rms_a);
  M = pidACV.out;
}
#endif

/* USER CODE END Application */
