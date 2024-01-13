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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bmp280.h"
#include "stdio.h"
#include "oled_spi.h"
#include "string.h"
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
BMP280 bmp280;
uint8_t oData[32];
uint8_t pData[64];
uint8_t circle = 0;
uint8_t t_Data[64];
uint8_t p_Data[64];
uint8_t h_Data[64];
uint8_t usart1_Rx_Temp[1];
uint8_t usart1_Rx_Data[512] = {0};
uint8_t usart1_Rx_cnt       = 0;
uint8_t usart1_Rx_flag      = 0;
uint8_t usart2_Rx_Temp[1];
uint8_t usart2_Rx_Data[512] = {0};
uint8_t usart2_Rx_cnt       = 0;
uint8_t usart2_Rx_flag      = 0;
uint8_t usart_flag          = 0;
float T, P, H;
void AT_init(void)
{
    OLED_ShowString(0, 0, (uint8_t *)"Loading...");
    HAL_Delay(2000);
    // 初始化
    HAL_UART_Transmit_IT(&huart6, (uint8_t *)"AT+RST\r\n", sizeof("AT+RST\r\n"));
    HAL_UART_Transmit_IT(&huart1, (uint8_t *)"初始化中...\r\n", sizeof("初始化中..."));
    HAL_Delay(5000);
    // 等待连接wifi
    HAL_UART_Transmit_IT(&huart1, (uint8_t *)"连接WIFI...\r\n", sizeof("连接WIFI..."));
    HAL_UART_Transmit_IT(&huart6, (uint8_t *)"AT+CIPV6=1\r\n", sizeof("AT+CIPV6=1\r\n")); // ipv6
    HAL_Delay(1000);
    HAL_UART_Transmit_IT(&huart6, (uint8_t *)"AT+CWMODE=3\r\n", sizeof("AT+CWMODE=3\r\n"));
    HAL_Delay(1000);
    HAL_UART_Transmit_IT(&huart6, (uint8_t *)"AT+CWJAP=\"abcdefg\",\"asdfghjkl\"\r\n", sizeof("AT+CWJAP=\"abcdefg\",\"asdfghjkl\"\r\n"));
    HAL_Delay(6000);
    // 设置Mqtt相关参数
    HAL_UART_Transmit_IT(&huart6, (uint8_t *)"AT+MQTTUSERCFG=0,1,\"esp32bmp280\",\"esp32bmp280\",\"123456789\",0,0,\"\"\r\n", sizeof("AT+MQTTUSERCFG=0,1,\"esp32bmp280\",\"esp32bmp280\",\"123456789\",0,0,\"\"\r\n"));
    HAL_Delay(1000);
    HAL_UART_Transmit_IT(&huart1, (uint8_t *)"域名设置完成\r\n", sizeof("域名设置完成"));
    // 连接Mqtt
    HAL_UART_Transmit_IT(&huart6, (uint8_t *)"AT+MQTTCONN=0,\"hollies.top\",25303,1\r\n", sizeof("AT+MQTTCONN=0,\"hollies.top\",25303,1\r\n"));
    HAL_Delay(5000);
    HAL_UART_Transmit_IT(&huart1, (uint8_t *)"连接完成\r\n", sizeof("连接完成"));
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
    MX_TIM8_Init();
    MX_USART1_UART_Init();
    MX_I2C1_Init();
    MX_SPI1_Init();
    MX_TIM2_Init();
    MX_USART6_UART_Init();
    /* USER CODE BEGIN 2 */
    // BMP280初始化
    if (bmp280_init(&bmp280, &hi2c1) == HAL_OK) {
        sprintf((char *)pData, "BMP280 initialization OK\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)pData, sizeof(pData), 100);
    } else {
        sprintf((char *)pData, "BMP280 initialization FAIL\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)pData, sizeof(pData), 100);
    }
    OLED_Init();
    AT_init();
    HAL_UART_Receive_IT(&huart1, usart1_Rx_Temp, 1);
    HAL_UART_Receive_IT(&huart6, usart2_Rx_Temp, 1);
    HAL_TIM_Base_Start_IT(&htim8);
    // HAL_TIM_Base_Start_IT(&htim8);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        if (usart1_Rx_flag == 1) {
            uint8_t i      = usart1_Rx_cnt;
            usart1_Rx_flag = 0;
            usart_flag     = 0;
            usart1_Rx_cnt  = 0;
            HAL_UART_Transmit_IT(&huart6, usart1_Rx_Data, i);
            HAL_TIM_Base_Start_IT(&htim8);
            memset(usart1_Rx_Temp, 0, sizeof(usart1_Rx_Temp));
        }
        if (usart2_Rx_flag == 1) {
            uint8_t i      = usart2_Rx_cnt;
            usart2_Rx_flag = 0;
            usart_flag     = 0;
            usart2_Rx_cnt  = 0;
            HAL_UART_Transmit_IT(&huart1, usart2_Rx_Data, i);
            HAL_TIM_Base_Start_IT(&htim8);
            memset(usart2_Rx_Temp, 0, sizeof(usart2_Rx_Temp));
        }
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
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 4;
    RCC_OscInitStruct.PLL.PLLN       = 168;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
// 接收中断
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        // 当串口接收数据超过5ms时，判断接受完成
        __HAL_TIM_SET_COUNTER(&htim2, 0); // 清除计数
        if (usart1_Rx_cnt == 0) {
            __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
            memset(usart1_Rx_Data, 0, sizeof(usart1_Rx_Data));
            usart_flag = 1;
            HAL_TIM_Base_Start_IT(&htim2);
            HAL_TIM_Base_Stop_IT(&htim8);
        }
        usart1_Rx_Data[usart1_Rx_cnt] = usart1_Rx_Temp[0];
        usart1_Rx_cnt++;
        HAL_UART_Receive_IT(&huart1, usart1_Rx_Temp, 1);
    }
    if (huart->Instance == USART6) {
        // 当串口接收数据超过5ms时，判断接受完成
        __HAL_TIM_SET_COUNTER(&htim2, 0); // 清除计数
        if (usart2_Rx_cnt == 0) {
            __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
            memset(usart2_Rx_Data, 0, sizeof(usart2_Rx_Data));
            usart_flag = 2;
            HAL_TIM_Base_Start_IT(&htim2);
            HAL_TIM_Base_Stop_IT(&htim8);
        }
        usart2_Rx_Data[usart2_Rx_cnt] = usart2_Rx_Temp[0];
        usart2_Rx_cnt++;
        HAL_UART_Receive_IT(&huart6, usart2_Rx_Temp, 1);
    }
}
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM1) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */
    if (htim->Instance == TIM8) {
        T = bmp280_getTemprature(&bmp280, &hi2c1);
        P = bmp280_getPressure(&bmp280, &hi2c1);
        H = bmp280_getAltitude(&bmp280, &hi2c1);
        // 屏幕显示
        sprintf((char *)pData, "T: %.2f C, P= %.2f hPa, H= %.2f m\r\n", T, P, H);
        HAL_UART_Transmit_IT(&huart1, (uint8_t *)pData, sizeof(pData));
        sprintf((char *)oData, "T: %.2f C", T);
        OLED_ShowString(0, 0, (uint8_t *)oData);
        sprintf((char *)oData, "P= %.2f hPa", P);
        OLED_ShowString(0, 2, (uint8_t *)oData);
        sprintf((char *)oData, "H= %.2f m", H);
        OLED_ShowString(0, 4, (uint8_t *)oData);
        // 循环提交温度，气压，海拔
        if (circle == 3)
            circle = 0;
        if (circle == 0) {
            circle++;
            sprintf((char *)t_Data, "AT+MQTTPUB=0,\"bmp280_t\",\"%.2f\",1,0\r\n", T);
            HAL_UART_Transmit_IT(&huart6, (uint8_t *)t_Data, 64);
        } else if (circle == 1) {
            circle++;
            sprintf((char *)p_Data, "AT+MQTTPUB=0,\"bmp280_p\",\"%.2f\",1,0\r\n", P);
            HAL_UART_Transmit_IT(&huart6, (uint8_t *)p_Data, 64);
        } else if (circle == 2) {
            circle++;
            sprintf((char *)h_Data, "AT+MQTTPUB=0,\"bmp280_h\",\"%.2f\",1,0\r\n", H);
            HAL_UART_Transmit_IT(&huart6, (uint8_t *)h_Data, 64);
        }
    }
    // 串口接收标志
    if (htim->Instance == TIM2) {
        if (usart_flag == 1) {
            usart1_Rx_flag = 1;
            usart_flag     = 0;
        } else if (usart_flag == 2) {
            usart2_Rx_flag = 1;
            usart_flag     = 0;
        } else {
            usart1_Rx_flag = 0;
            usart2_Rx_flag = 0;
        }
        HAL_TIM_Base_Stop_IT(&htim2);
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
    while (1) {
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
