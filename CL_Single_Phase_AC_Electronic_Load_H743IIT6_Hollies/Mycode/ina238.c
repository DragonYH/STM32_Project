#include "ina238.h"

/**
 * @brief  INA238初始化
 * @param  hi2c: I2C句柄
 * @param  error: 误差纠正
 */
void ina238_Init(I2C_HandleTypeDef *hi2c, int8_t error)
{
	uint8_t pData[2];
	pData[0] = 0x00;
	pData[1] = 0x00;
	HAL_I2C_Mem_Write(hi2c, ADDR_WRITE, CONFIG, 1, pData, 2, 100); // 量程选择为163.84mv
	pData[0] = 0xf4;
	pData[1] = 0x93;
	HAL_I2C_Mem_Write(hi2c, ADDR_WRITE, ADC_CONFIG, 1, pData, 2, 100); // 转换时间设置为9.6ms
	pData[0] = 0x0f;
	pData[1] = 0xa0 + error;
	HAL_I2C_Mem_Write(hi2c, ADDR_WRITE, SHUNT_CAL, 1, pData, 2, 100); // 设置最大电流为8A，采样电阻为20mΩ
}
/**
 * @brief  INA238读取电压
 * @param  hi2c: I2C句柄
 * @retval 电压值
 */
float ina238_GetVolt(I2C_HandleTypeDef *hi2c)
{
	uint8_t pData[2];
	float volt;
	if (HAL_I2C_Mem_Read(hi2c, ADDR_READ, VBUS, 1, pData, 2, 100) == HAL_OK)
	{
		volt = pData[0] * 256 + pData[1];
		if (volt > 32767) // 为负数的情况
			volt = (volt - 65536) * 3.125 / 1000;
		else
			volt = volt * 3.125 / 1000;
	}
	return volt;
}
/**
 * @brief  INA238读取电流
 * @param  hi2c: I2C句柄
 * @retval 电流值
 */
float ina238_GetCurrent(I2C_HandleTypeDef *hi2c)
{
	uint8_t pData[2];
	float current;
	if (HAL_I2C_Mem_Read(hi2c, ADDR_READ, CURRENT, 1, pData, 2, 100) == HAL_OK)
	{
		current = pData[0] * 256 + pData[1];
		if (current > 32767)
			current = (current - 65536) * (8 / 32768.0);
		else
			current = current * (8 / 32768.0);
	}
	return current;
}
/**
 * @brief  INA238读取功率
 * @param  hi2c: I2C句柄
 * @retval 功率值
 */
float ina238_GetPower(I2C_HandleTypeDef *hi2c)
{
	uint8_t pData[3];
	float power;
	if (HAL_I2C_Mem_Read(hi2c, ADDR_READ, POWER, 1, pData, 3, 100) == HAL_OK)
	{
		power = 0.2 * (pData[0] * 65536 + pData[1] * 256 + pData[2]) * (8 / 32768.0);
	}
	return power;
}
/**
 * @brief  INA238读取温度
 * @param  hi2c: I2C句柄
 * @retval 温度值
 */
float ina238_GetTemperature(I2C_HandleTypeDef *hi2c)
{
	uint8_t pData[2];
	float Temperature;
	if (HAL_I2C_Mem_Read(hi2c, ADDR_READ, DIETEMP, 1, pData, 2, 100) == HAL_OK)
	{
		Temperature = (pData[0] >> 4) * 256 + (pData[1] >> 4);
		if (Temperature > 2047)
			Temperature = (Temperature - 2048) * 0.125;
		else
			Temperature = Temperature * 0.125;
	}
	return Temperature;
}
