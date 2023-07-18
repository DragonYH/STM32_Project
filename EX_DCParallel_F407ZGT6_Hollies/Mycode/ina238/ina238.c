#include "ina238.h"

void INA238_init(I2C_HandleTypeDef* hi2c, int16_t error)
{
	uint8_t pData[2];
	pData[0]=0x00;
	pData[1]=0x00;
	HAL_I2C_Mem_Write(hi2c, ADDR_WRITE, CONFIG, 1, pData, 2, 100);	//量程选择为163.84mv
	HAL_Delay(100);
	pData[0]=0xf4;
	pData[1]=0x93;
	HAL_I2C_Mem_Write(hi2c, ADDR_WRITE, ADC_CONFIG, 1, pData, 2, 100);	//转换时间设置为9.6ms
	HAL_Delay(100);
	uint16_t temp=0x0fa0 + error;
	pData[0]=(temp >> 8) & 0xff;
	pData[1]=temp & 0xff;
	HAL_I2C_Mem_Write(hi2c, ADDR_WRITE, SHUNT_CAL, 1, pData, 2, 100);	//设置最大电流为8A，采样电阻为20mΩ
	HAL_Delay(100);
}

double INA238_getVolt(I2C_HandleTypeDef* hi2c)
{
	uint8_t pData[2];
	double volt = 0.0;
	if(HAL_I2C_Mem_Read(hi2c, ADDR_READ, VBUS, 1, pData, 2, 100)==HAL_OK)
	{
		volt = pData[0] * 256 + pData[1];
		if(volt > 32767)	//为负数的情况
			volt = (volt - 65536) * 3.125 / 1000;
		else
			volt = volt * 3.125 / 1000;
	}
	return volt;
}

double INA238_getCurrent(I2C_HandleTypeDef* hi2c)
{
	uint8_t pData[2];
	double current = 0.0;
	if(HAL_I2C_Mem_Read(hi2c, ADDR_READ, CURRENT, 1, pData, 2, 100)==HAL_OK)
	{
		current = pData[0] * 256 + pData[1];
		if(current > 32767)
			current = (current - 65536) * (8 / 32768.0);
		else
			current = current * (8 / 32768.0);
	}
	return current;
}

double INA238_getPower(I2C_HandleTypeDef* hi2c)
{
	uint8_t pData[3];
	double power = 0.0;
	if(HAL_I2C_Mem_Read(hi2c, ADDR_READ, POWER, 1, pData, 3, 100)==HAL_OK)
	{
		power = 0.2 * (pData[0] * 65536 + pData[1] * 256 + pData[2]) * (8 / 32768.0);
	}
	return power;
}

double INA238_getTemperature(I2C_HandleTypeDef* hi2c)
{
	uint8_t pData[2];
	double Temperature = 0.0;
	if(HAL_I2C_Mem_Read(hi2c, ADDR_READ, DIETEMP, 1, pData, 2, 100)==HAL_OK)
	{
		Temperature = (pData[0] >> 4) * 256 + (pData[1] >> 4);
		if(Temperature > 2047)
			Temperature = (Temperature - 2048) * 0.125;
		else
			Temperature = Temperature * 0.125;
	}
	return Temperature;
}
