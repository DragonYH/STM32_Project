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
#include "ad7606.h"
#include "iir.h"
// IDE版本错误临时修复
__attribute__((weak)) void _close(void) {}
__attribute__((weak)) void _lseek(void) {}
__attribute__((weak)) void _read(void) {}
__attribute__((weak)) void _write(void) {}
__attribute__((weak)) void _fstat(void) {}
__attribute__((weak)) void _getpid(void) {}
__attribute__((weak)) void _isatty(void) {}
__attribute__((weak)) void _kill(void) {}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USER_DEBUG 0                                                              // 串口调试
#define circuit_Connect() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)      // 电路连接
#define circuit_Disconnect() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET) // 电路断开
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
// 定义交流电压和电流信号和信号配置变量
// 将控制变量放到DTCM中，防止数据传输拖慢运行速度
__attribute__((section("._DTCM_Area"))) pll_Signal_V *signal_V;
__attribute__((section("._DTCM_Area"))) pll_Signal_I *signal_I;
__attribute__((section("._DTCM_Area"))) arm_biquad_casd_df1_inst_f32 *iir_V; // IIR滤波器参数结构体
__attribute__((section("._DTCM_Area"))) arm_biquad_casd_df1_inst_f32 *iir_I; // IIR滤波器参数结构体
PID *pid_DC_V;                                                               // 直流稳压PID控制器
// 创建ADC数据空间
__attribute__((section("._DTCM_Area"))) float adcBuf[3] = {0};
// 直流侧电压电流
float dcVolt = 0.f;
float dcCurrent = 0.f;
// 芯片核心温度
float chipTemp = 0;
// 显示
uint8_t textBuf[256] = {0};
void oled_Show(void)
{
#if !USER_DEBUG
  // IN:  输入
  sprintf((char *)textBuf, "IN :%7.2fV %6.2fA", signal_V->rms, signal_I->rms);
  OLED_ShowString(0, 0, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  // DC:  直流
  sprintf((char *)textBuf, "DC :%7.2fV %6.2fA", dcVolt, dcCurrent);
  OLED_ShowString(0, 12, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  // OUT: 输出
  sprintf((char *)textBuf, "OUT:%7.2fV %6.2fA", signal_V->input[0], signal_I->input[0]);
  OLED_ShowString(0, 24, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  // FAC: 功率因数
  sprintf((char *)textBuf, "FAC:%6.2f %9.0f", phase_set, pid_DC_V->out);
  // sprintf((char *)textBuf, "FAC:%5.2f %8.0f", arm_cos_f32(0), signal_I->park_inv_a);
  // sprintf((char *)textBuf, "FAC: %4.2f %8.0f", arm_cos_f32(0), signal_I->pr->out[0]);
  OLED_ShowString(0, 36, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));

  // EFF: 效率
  float eff = (dcVolt * dcCurrent) / ((signal_V->rms * iirScale_20Hz / 1.4f) * (signal_I->rms * iirScale_20Hz / 1.414f)) * 100.f;
  if (eff > 100.f)
    eff = 100.f;
  else if (eff < 0.f)
    eff = 0.f;
  sprintf((char *)textBuf, "EFF:%7.2f%%%7.1fC", eff, chipTemp);
  OLED_ShowString(0, 48, textBuf, 12);
  // CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));
  OLED_Refresh();
#else
  // 串口调试
  //  sprintf((char *)textBuf, "x=0,V=%.2f,I=%.2f,park_Iq=%.2f,park_Id=%.2f,park_Vd=%.2f,park_Vq=%.2f,sogi_Va=%.2f,sogi_Vb=%.2f,sogi_Ia=%.2f,sogi_Ib=%.2f,\n", signal_V->input[0], signal_I->input[0], signal_I->park_q, signal_I->park_d, signal_V->park_d, signal_V->park_q, signal_V->sogi->a[0], signal_V->sogi->b[0], signal_I->sogi->a[0], signal_I->sogi->b[0]);
  sprintf((char *)textBuf, "x=0,V=%.2f,park_Vd=%.2f,park_Vq=%.2f,sogi_Va=%.2f,sogi_Vb=%.2f,\n", signal_V->input[0], signal_V->park_d, signal_V->park_q, signal_V->sogi->a[0], signal_V->sogi->b[0]);
  CDC_Transmit_FS((uint8_t *)textBuf, sizeof(textBuf));
#endif
}
// 按键控制
void key_Control(void)
{
  static uint8_t flag = 0;
  if (HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_7) == GPIO_PIN_RESET)
  {
    while (HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_7) == GPIO_PIN_RESET)
      ;
    if (flag == 0)
      phase_set -= 0.1f;
    else
      phase_set += 0.1f;
    if (phase_set >= 2.7f || phase_set <= -2.7f)
    {
      flag = !flag;
    }
  }
}
// 电路开关
void circuit_Control(void)
{
  if (signal_I->rms > 0.2f)
    circuit_Connect();
  else
    circuit_Disconnect();
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
  MX_DAC1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM8_Init();
  MX_USB_DEVICE_Init();
  MX_I2C3_Init();
  MX_TIM3_Init();
  MX_ADC3_Init();
  MX_TIM12_Init();
  /* USER CODE BEGIN 2 */
  // 锁相环初始化
  pll_Init_V(&signal_V, 50, 20000, 30 * 1.414); // 电压锁相
#if PRorPI
  pll_Init_I(signal_I, 50, 20000, 0.5f, 7600.f, 0.001f, 0.1f); // 电流环 1.414-7600
#else
  pll_Init_I(&signal_I, 50, 20000); // 电流环
#endif
  // 外设初始化
  OLED_Init();            // OLED初始化
  ad7606_Init();          // 初始化AD7606
  ina238_Init(&hi2c3, 0); // INA238初始化
  // ADC校准
  HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED, ADC_CALIB_OFFSET);
  HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED, ADC_CALIB_OFFSET_LINEARITY);
  // iir滤波器初始化
  iir_V = (arm_biquad_casd_df1_inst_f32 *)malloc(sizeof(arm_biquad_casd_df1_inst_f32));
  iir_I = (arm_biquad_casd_df1_inst_f32 *)malloc(sizeof(arm_biquad_casd_df1_inst_f32));
  arm_biquad_cascade_df1_init_f32(iir_V, iirNumStages, iirCoeffs_20Hz, signal_V->iirState);
  arm_biquad_cascade_df1_init_f32(iir_I, iirNumStages, iirCoeffs_20Hz, signal_I->iirState);
  // DC稳压PID初始化
  pid_DC_V = (PID *)malloc(sizeof(PID));
  pid_Init(pid_DC_V, 0.2f, 0.05f, 0, 11999.f, 8000.f);
  // DAC模拟输出初始化
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2048);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
  // 打开PWM波
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
  // 开启中断
  HAL_TIM_Base_Start_IT(&htim3);
  ad7606_Start(&htim2, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    circuit_Control();
    key_Control();
    oled_Show();
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  ad7606_Stop(&htim2, TIM_CHANNEL_1);
  // 释放内存
  free(iir_V);
  free(iir_I);
  pll_Free_V(signal_V);
  pll_Free_I(signal_I);
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
/**
 * @brief 外部中断触发示例
 * @param GPIO_Pin 中断引脚
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == AD7606_BUSY_Pin)
  {
    ad7606_GetValue(&hspi2, 3, adcBuf);
    // 缓存adcBuf
    signal_V->input[0] = adcBuf[1] / 0.02373021108f;
    signal_I->input[0] = adcBuf[2] * 2.464809491f;
    // 锁相控制
    pll_Control_V(signal_V);
#if PRorPI
    pll_Control_I(signal_I, signal_V, 50.f, dcVolt); // 电流环
    // 调节SPWM占空比
    if (signal_I->pr->out[0] > 0)
    {
      __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, signal_I->pr->out[0]);
      __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 0);
    }
    else
    {
      __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
      __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, -1.f * signal_I->pr->out[0]);
    }
#else
    pll_Control_I(signal_I, signal_V, 2.f, phase_set);
    // 调节SPWM占空比
    if (signal_I->park_inv_a > 0)
    {
      __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, signal_I->park_inv_a / 200.f / 1.1f * 12000.f);
      __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 0);
    }
    else
    {
      __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
      __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, -signal_I->park_inv_a / 200.f / 1.1f * 12000.f);
    }
    // 直流稳压
    pid(pid_DC_V, dcVolt, 60.f);
    if (arm_cos_f32(signal_V->theta) > 0)
    {
      __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, pid_DC_V->out * arm_cos_f32(signal_V->theta));
      __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, 0);
    }
    else
    {
      __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, 0);
      __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, -pid_DC_V->out * arm_cos_f32(signal_V->theta));
    }
#endif
  }
  // 输出有效值滤波
  float filter_temp = 0.f;
  arm_sqrt_f32(signal_V->park_d * signal_V->park_d + signal_V->park_q * signal_V->park_q, &signal_V->peak);
  arm_sqrt_f32(signal_I->park_d * signal_I->park_d + signal_I->park_q * signal_I->park_q, &signal_I->peak);
  arm_biquad_cascade_df1_f32(iir_V, &signal_V->peak, &filter_temp, iirBlockSize);
  signal_V->rms = filter_temp * iirScale_20Hz / 1.41421356237f;
  arm_biquad_cascade_df1_f32(iir_I, &signal_I->peak, &filter_temp, iirBlockSize);
  signal_I->rms = filter_temp * iirScale_20Hz / 1.41421356237f;
  // 调试输出
#if USER_DEBUG
  oled_Show();
#endif
  // DAC模拟输出，便于调试，不需要时可关闭
  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2000.f * arm_cos_f32(signal_V->theta) + 2048.f);
}
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
  if (htim->Instance == TIM3) // 100Hz
  {
    // 采集直流电压电流
    dcVolt = ina238_GetVolt(&hi2c3);
    dcCurrent = ina238_GetCurrent(&hi2c3);
    // 采集芯片核心温度
    HAL_ADC_Start(&hadc3);
    if (HAL_ADC_PollForConversion(&hadc3, 10) == HAL_OK) // 判断是否转换完成
    {
      uint16_t temprature = HAL_ADC_GetValue(&hadc3); // 读出转换结果
      chipTemp = ((110.0 - 30.0) / (*(unsigned short *)(0x1FF1E840) - *(unsigned short *)(0x1FF1E820))) * (temprature - *(unsigned short *)(0x1FF1E820)) + 30;
    }
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
    // 闪烁灯
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
    HAL_Delay(100);
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
