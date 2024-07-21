/*
 * @Author       : DragonYH 1016633827@qq.com
 * @Date         : 2024-07-20 20:09:54
 * @LastEditors  : DragonYH 1016633827@qq.com
 * @LastEditTime : 2024-07-20 20:20:33
 * @FilePath     : \EX_Single_Phase_Rectifier_H743IIT6_Hollies\User\Inc\ad7606.h
 * @Description  : AD7606 spi驱动声明
 *
 * Copyright (c) 2024 by DragonYH, All Rights Reserved.
 */

#ifndef __AD7606_H
#define __AD7606_H

#include "main.h"

#define AD7606_CS_SET() HAL_GPIO_WritePin(AD7606_CS_GPIO_Port, AD7606_CS_Pin, GPIO_PIN_SET)
#define AD7606_CS_RESET() HAL_GPIO_WritePin(AD7606_CS_GPIO_Port, AD7606_CS_Pin, GPIO_PIN_RESET)
#define AD7606_RST_SET() HAL_GPIO_WritePin(AD7606_RST_GPIO_Port, AD7606_RST_Pin, GPIO_PIN_SET)
#define AD7606_RST_RESET() HAL_GPIO_WritePin(AD7606_RST_GPIO_Port, AD7606_RST_Pin, GPIO_PIN_RESET)

void ad7606_Init(void);
void ad7606_Read(SPI_HandleTypeDef *hspi, int16_t *data, uint16_t channel);
void ad7606_Start(TIM_HandleTypeDef *htim, uint32_t channel);
void ad7606_Stop(TIM_HandleTypeDef *htim, uint32_t channel);
void ad7606_Convert(int16_t *data, uint16_t channel, float *adcValue);
void ad7606_GetValue(SPI_HandleTypeDef *hspi, uint16_t channel, float *adcValue);

#endif
