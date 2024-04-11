#include "bc26.h"
#include "usart.h"
/**
 * @brief 初始化BC26模块
 *
 */
void bc26_Init(void)
{
}

/**
 * @brief 连接MQTT服务器
 *
 */
void bc26_MQTT_Connect(void)
{
    HAL_UART_Transmit(&huart1, "AT+QMTOPEN=0,\"gifthub.cn\",1883", strlen("AT+QMTOPEN=0,\"gifthub.cn\",1883"), 1000);
}