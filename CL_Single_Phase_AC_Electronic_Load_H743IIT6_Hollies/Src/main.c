/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "oled.h"
#include "pll.h"
#include "stdlib.h"
#include "usbd_cdc_if.h"
#include "stdio.h"
#include "ina238.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// IDE版本错误临时修复
__attribute__((weak)) void _close(void) {}
__attribute__((weak)) void _lseek(void) {}
__attribute__((weak)) void _read(void) {}
__attribute__((weak)) void _write(void) {}
__attribute__((weak)) void _fstat(void) {}
__attribute__((weak)) void _getpid(void) {}
__attribute__((weak)) void _isatty(void) {}
__attribute__((weak)) void _kill(void) {}
// 定义交流电压和电流信号和信号配置变量
// 将控制变量放到DTCM中，防止数据传输拖慢运行速度
__attribute__((section("._DTCM_Area"))) pll_Signal *signal_V;
__attribute__((section("._DTCM_Area"))) pll_Config *signal_config_V;
__attribute__((section("._DTCM_Area"))) pll_Signal *signal_I;
__attribute__((section("._DTCM_Area"))) pll_Config *signal_config_I;
// 创建ADC数据空间
__attribute__((section("._D3_Area"))) uint32_t adcBuf[2] = {0};
// SPWM波调制比
float MI = 0.9f;
// 直流电压
float dcVolt = 0.f;
float dcCurrent = 0.f;
PID *dcPid;
// 显示函数
uint8_t textBuf[256] = {0};
void oled_Show(void)
{
  // IN:  输入
  sprintf((char *)textBuf, "IN : %5.2fV %5.2fA", signal_V->u_0, signal_I->u_0);
  OLED_ShowString(0, 0, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  // DC:  直流
  sprintf((char *)textBuf, "DC : %5.2fV %5.2fA", dcVolt, dcCurrent);
  OLED_ShowString(0, 12, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  // OUT: 输出
  sprintf((char *)textBuf, "OUT: %5.2fV %5.2fA", signal_V->u_0, signal_I->u_0);
  OLED_ShowString(0, 24, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  // FAC: 功率因数
  sprintf((char *)textBuf, "FAC: %4.2f", arm_cos_f32(0));
  OLED_ShowString(0, 36, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  // EFF: 效率
  sprintf((char *)textBuf, "EFF: %5.2f%% %5.2f", (signal_V->u_0 * signal_I->u_0) / (signal_V->u_0 * signal_I->u_0) * 100.f, MI);
  OLED_ShowString(0, 48, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  // 串口调试
  // sprintf((char *)textBuf, "x=0,park_Iq=%.2f,park_Id=%.2f,park_Vd=%.2f,park_Vq=%.2f,sogi_Va=%.2f,sogi_Vb=%.2f,sogi_Ia=%.2f,sogi_Ib=%.2f,inv_a=%.2f\n", signal_I->park_q, signal_I->park_d, signal_V->park_d, signal_V->park_q, signal_V->sogi_a_0, signal_V->sogi_b_0 / 382 * 3, signal_I->sogi_a_0, signal_I->sogi_b_0 / 382 * 3, signal_I->park_inv_a);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  OLED_Refresh();
}
// 连接电路
void circuit_Connect(void)
{
}
void circuit_Disconnect(void)
{
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_DAC1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM8_Init();
  MX_USB_DEVICE_Init();
  MX_I2C3_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  // 给变量分配存储空间
  signal_V = (pll_Signal *)malloc(sizeof(pll_Signal));
  signal_config_V = (pll_Config *)malloc(sizeof(pll_Config));
  signal_I = (pll_Signal *)malloc(sizeof(pll_Signal));
  signal_config_I = (pll_Config *)malloc(sizeof(pll_Config));
  dcPid = (PID *)malloc(sizeof(PID));
  // 芯片温度
  // uint16_t temprature = 0;
  // float temp_result = 0;
  // 锁相环初始化
  pll_Init(signal_V, signal_config_V, 50, 20000); // 电压环
  pll_Init(signal_I, signal_config_I, 50, 20000); // 电流环
  // DAC模拟输出初始化
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  // ADC校准
  HAL_Delay(200);
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED); // 线性度校准
  HAL_Delay(500);
  // 打开PWM波
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  // OLED初始化
  OLED_Init();
  // INA238初始化
  ina238_Init(&hi2c3, 0);
  // pid初始化
  pid_Init(dcPid, 0.01, 0.1, 0.1, 0.9, 0.3);
  // 开启中断
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  // signal_config_I->phase = PI / 2.f;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    //    dcVolt = ina238_GetVolt(&hi2c3);
    oled_Show();
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
   */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
   */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI1;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 300;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
   */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
   */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x38000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
   */
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x20000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM2) // 20kHz
  {
    // 缓存adcBuf
    HAL_ADC_Start_DMA(&hadc1, adcBuf, 2);
    SCB_InvalidateDCache_by_Addr(adcBuf, sizeof(adcBuf));
    signal_V->u_0 = adcBuf[0] * 3.3f / 65536.0f - 1.5f;
    signal_I->u_0 = adcBuf[1] * 3.3f / 65536.0f - 1.5f;
    // 锁相控制
    pll_Control(signal_V, signal_config_V, signal_V); // 电压环
    pll_Control(signal_I, signal_config_I, signal_V); // 电流环
    // 调节SPWM占空比
    // 要想实现PFC，需要让电流相位与电压相位相同，而电压相位由电网控制，所以需要闭环控制的是电流相位
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, MI * (__HAL_TIM_GET_AUTORELOAD(&htim8) / 2.0f) * arm_sin_f32(signal_I->theta + PI / 2.f) + (__HAL_TIM_GET_AUTORELOAD(&htim8) / 2.0f));
    // 反馈侧
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, MI * (__HAL_TIM_GET_AUTORELOAD(&htim8) / 2.0f) * arm_sin_f32(signal_I->theta + PI / 2.f) + (__HAL_TIM_GET_AUTORELOAD(&htim8) / 2.0f));
    // DAC模拟输出，便于调试，不需要时可关闭
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2000.f * arm_sin_f32(signal_V->theta + PI / 2.f) + 2048.f);
  }
  if (htim->Instance == TIM3) // 100Hz
  {
    // 控制中间直流电压
    dcVolt = ina238_GetVolt(&hi2c3);
    dcCurrent = ina238_GetCurrent(&hi2c3);
    MI = pid(dcPid, dcVolt, 10.f);
  }
  /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
