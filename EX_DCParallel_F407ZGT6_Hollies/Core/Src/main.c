/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled_spi.h"
#include "ina238.h"
#include "pid.h"
#include "math.h"
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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define BATCH_DATA_LEN 12
float M = 1 / 3;	//电流比例

uint32_t dmaDataBuffer[BATCH_DATA_LEN];
uint16_t p_count = 0 , p_count1 = 0;
float u1 = 0,u1_sum = 0,u1_show = 0;
void LED_Show()
{
	OLED_ShowNum(0, 0, INA238_getVolt(&hi2c1) * 1000, 5, 16);
	int i = INA238_getCurrent(&hi2c1) * 1000;
	if (i >= 0)
	{
		uint32_t t = i;
		OLED_ShowNum(0, 2, t, 4, 16);
	}
	else
	{
		uint32_t t = -i;
		OLED_ShowString(0, 2, (uint8_t*)"-");
		OLED_ShowNum(8, 2, t, 4, 8);
	}

	OLED_ShowNum(0, 4, INA238_getVolt(&hi2c2) * 1000, 5, 16);
	i = INA238_getCurrent(&hi2c2) * 1000;
	if (i >= 0)
	{
		uint32_t t = i;
		OLED_ShowNum(0, 6, t, 4, 16);
	}
	else
	{
		uint32_t t = -i;
		OLED_ShowString(0, 6, (uint8_t*)"-");
		OLED_ShowNum(8, 6, t, 4, 8);
	}
	OLED_ShowNum(64, 0, TIM8->CCR1, 4, 16);
	OLED_ShowNum(64, 6, TIM8->CCR2, 4, 16);
}
void Scan_Key()
{
	while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET
			|| HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
	{
		HAL_Delay(10);	//去抖�?
		if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
		{
			while (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET)
				;	//这句放在前为松开后执行，放在后为按下时执�?
			HAL_TIM_Base_Start_IT(&htim1);
		}
	}
}
#define ADC_SAMPLE_PNUM 12
//uint16_t AD_Value[6][2];
uint16_t ADC_Filter(uint8_t Channel)		//中位值平均滤波法（又称防脉冲干扰平均滤波法）
{
	uint8_t count, i, j;
	uint32_t sum = 0;
	uint16_t temp = 0;
	uint16_t tempbuf[ADC_SAMPLE_PNUM];
	for (i = 0; i < ADC_SAMPLE_PNUM; i++)
	{
		tempbuf[i] = dmaDataBuffer[i];
	}
	for (j = 0; j < ADC_SAMPLE_PNUM; j++)     //冒泡�???
	{
		for (i = j; i < ADC_SAMPLE_PNUM; i++)
		{
			if (tempbuf[i] > tempbuf[j])
			{
				temp = tempbuf[i];
				tempbuf[i] = tempbuf[j];
				tempbuf[j] = temp;
			}
		}
	}
	for (count = 1; count < ADC_SAMPLE_PNUM - 1; count++)
		sum = sum + tempbuf[count];
	return (sum / (ADC_SAMPLE_PNUM - 2));
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

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();
	HAL_ADC_Start_DMA(&hadc1, dmaDataBuffer ,BATCH_DATA_LEN);
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start(&htim8);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	INA238_init(&hi2c1, -100);
//	INA238_init(&hi2c2, 90);
	PID_Init();
	OLED_Clear();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//
//		LED_Show();
//		Scan_Key();
		HAL_Delay(10);
//		OLED_Clear();
		OLED_ShowNum(0, 0, u1_show*1000000, 12, 16);
		OLED_ShowNum(0, 2, (float) INA238_getVolt(&hi2c1)*1000000, 12, 16);
//		OLED_ShowNum(0, 0, u1_show, 12, 16);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		u1 = 0.02143/1.2391*ADC_Filter(0);
		u1_sum += u1;
		p_count++;
		p_count1++;
		if(p_count == 8000)
		{
			p_count = 0;
			u1_show = u1_sum * 0.000125;
			u1_sum = 0;
		}
	}
}
/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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
