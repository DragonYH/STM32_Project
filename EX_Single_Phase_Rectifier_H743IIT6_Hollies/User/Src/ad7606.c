/*
 * @Author       : DragonYH 1016633827@qq.com
 * @Date         : 2024-07-20 20:10:11
 * @LastEditors  : DragonYH 1016633827@qq.com
 * @LastEditTime : 2024-07-20 20:19:31
 * @FilePath     : \EX_Single_Phase_Rectifier_H743IIT6_Hollies\User\Src\ad7606.c
 * @Description  : AD7606 spi驱动实现
 *
 * Copyright (c) 2024 by DragonYH, All Rights Reserved.
 */

#include "ad7606.h"

/**
 * @brief AD7606初始化
 */
void ad7606_Init(void)
{
    AD7606_RST_RESET();
    HAL_Delay(1);
    AD7606_RST_SET();
    HAL_Delay(1);
    AD7606_RST_RESET();
}
/**
 * @brief 读取数据
 * @param hspi SPI句柄
 * @param data 数据指针
 * @param channel 读取几个通道
 */
void ad7606_Read(SPI_HandleTypeDef *hspi, int16_t *data, uint16_t channel)
{
    AD7606_CS_RESET();
    HAL_SPI_Receive(hspi, (uint8_t *)data, channel, 100);
    AD7606_CS_SET();
}
/**
 * @brief 启动AD7606
 * @param htim PWM产生的时钟
 * @param channel PWM产生的通道
 */
void ad7606_Start(TIM_HandleTypeDef *htim, uint32_t channel)
{
    HAL_TIM_PWM_Start(htim, channel);
}
/**
 * @brief 停止AD7606
 * @param htim PWM产生的时钟
 * @param channel PWM产生的通道
 */
void ad7606_Stop(TIM_HandleTypeDef *htim, uint32_t channel)
{
    HAL_TIM_PWM_Stop(htim, channel);
    AD7606_CS_SET();
}
/**
 * @brief 数据转换
 * @param data 数据指针
 * @param channel 通道数
 * @param adcValue 采样值
 */
void ad7606_Convert(int16_t *data, uint16_t channel, float *adcValue)
{
    for (int i = 0; i < channel; i++)
    {
        // 16位二进制补码转单精度浮点数
        data[i] = data[i] & 0x8000 ? (-((~data[i] + 1) & 0x7fff)) : data[i];
        adcValue[i] = 5.f * data[i] / 32768.f;
    }
}
/**
 * @brief 获取采样值
 * @param hspi SPI句柄
 * @param channel 读取几个通道 1-8
 * @param adcValue 采样值
 */
void ad7606_GetValue(SPI_HandleTypeDef *hspi, uint16_t channel, float *adcValue)
{
    int16_t ad7606_data[channel];
    ad7606_Read(hspi, ad7606_data, channel);
    ad7606_Convert(ad7606_data, channel, adcValue);
}
/**
 * @brief 外部中断触发示例
 * @param GPIO_Pin 中断引脚
 */
/**
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == AD7606_BUSY_Pin)
    {
        ad7606_GetValue(&hspi1, 8, adcValue);
    }
}
*/
