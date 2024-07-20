#include <stddef.h>
#include <stdint.h>

#include "ina228.h"

#define MSB(u16) (((u16) & 0xFF00U) >> 8)
#define LSB(u16) ((u16) & 0xFFU)

#define maxRegAddress 0x3F

// 寄存器大小，单位字节
const uint8_t INA228_regSize[maxRegAddress + 1] = {
	2, 2, 2, 2, 3, 3, 2, 3,
	3, 5, 5, 2, 2, 2, 2, 2,
	2, 2, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 2, 2};

/**
 * @brief 写入寄存器值
 * @param sensor INA228传感器的句柄
 * @param regAddr 要写入的寄存器地址
 * @param value 要写入的值
 */
void INA228_writeReg(INA228_Handle sensor, uint8_t regAddr, uint16_t value)
{
	uint8_t txBuf[2] = {0}; // 所有可写寄存器均为 2 字节

	txBuf[0] = MSB(value);
	txBuf[1] = LSB(value);
	// 写入寄存器
	HAL_I2C_Mem_Write(sensor->hi2c, sensor->devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, txBuf, 2, 100);

	// 检查 ADC量程 的变化
	if (regAddr == INA228_config_register)
	{
		sensor->adcrange = value & INA228_config_register_adcrange_4096mV;
	}
}

/**
 * @brief 使用当前设置配置设备
 * @param sensor INA228传感器的句柄
 */
void INA228_config(INA228_Handle sensor)
{
	// 写入传感器配置寄存器
	INA228_writeReg(sensor, INA228_config_register, sensor->configRegister);
	INA228_writeReg(sensor, INA228_adc_config_register, sensor->adcConfigRegister);
	INA228_writeReg(sensor, INA228_shunt_cal_register, sensor->shuntCalRegister);
	INA228_writeReg(sensor, INA228_shunt_tempco_register, sensor->shuntTempcoRegister);
	INA228_writeReg(sensor, INA228_diag_alrt_register, sensor->diagAlrtRegister);
	INA228_writeReg(sensor, INA228_sovl_register, sensor->sovlRegister);
	INA228_writeReg(sensor, INA228_suvl_register, sensor->suvlRegister);
	INA228_writeReg(sensor, INA228_bovl_register, sensor->bovlRegister);
	INA228_writeReg(sensor, INA228_buvl_register, sensor->buvlRegister);
	INA228_writeReg(sensor, INA228_temp_limit_register, sensor->tempLimitRegister);
	INA228_writeReg(sensor, INA228_pwr_limit_register, sensor->pwrLimitRegister);
}

/**
 * @brief 设置用于计算的电流最小可测量单位值
 * @param sensor INA228传感器的句柄
 * @param CURRENT_LSB 电流最小可测量单位值
 */
void INA228_setCURRENT_LSB(INA228_Handle sensor, float CURRENT_LSB)
{
	sensor->currentlsb = CURRENT_LSB;
}

/**
 * @brief 从寄存器读取值
 * @param sensor INA228传感器的句柄
 * @param regAddr 要读取的寄存器地址
 * @return 寄存器中的值
 */
uint64_t INA228_readReg(INA228_Handle sensor, uint8_t regAddr)
{
	uint64_t value;
	int i;

	uint8_t rxBuf[5] = {0}; // max buffer size
	// 读取寄存器值
	HAL_I2C_Mem_Read(sensor->hi2c, sensor->devAddr, regAddr, I2C_MEMADD_SIZE_8BIT, rxBuf, INA228_regSize[regAddr], 100);

	// 合并字节
	value = rxBuf[0];
	for (i = 1; i < INA228_regSize[regAddr]; i++)
	{
		value = (value << 8) | rxBuf[i];
	}

	return value;
}

/**
 * @brief 获取Shunt电压值（毫伏）
 * @param sensor INA228传感器的句柄
 * @return Shunt电压值
 */
float INA228_getVSHUNT_mV(INA228_Handle sensor)
{
	uint64_t value = INA228_readReg(sensor, INA228_vshunt_register);
	float data;

	// 删除保留位
	value = value >> 4;

	// 转换为 2 的补码和有符号值
	if (value > 0x7FFFF)
	{
		data = (float)value - 0x100000;
	}
	else
	{
		data = (float)value;
	}

	// 转换为毫伏
	if (sensor->adcrange == INA228_config_register_adcrange_4096mV)
	{
		data = (data * 78.125f) / 1000000;
	}
	else
	{
		data = (data * 312.5f) / 1000000;
	}

	return data;
}

/**
 * @brief 获取总线电压值（伏特）
 * @param sensor INA228传感器的句柄
 * @return 总线电压值（V）
 */
float INA228_getVBUS_V(INA228_Handle sensor)
{
	uint64_t value = INA228_readReg(sensor, INA228_vbus_register);
	float data;

	// 删除保留位
	value = value >> 4;

	// Convert for 2's compliment and signed value (though always positive)
	if (value > 0x7FFFF)
	{
		data = (float)value - 0x100000; // left for redundancy and error checking, should never get used
	}
	else
	{
		data = (float)value;
	}

	// Convert to V
	data = (data * 195.3125f) / 1000000;

	return data;
}

/**
 * @brief 获取芯片温度值（摄氏度）
 * @param sensor INA228传感器的句柄
 * @return 芯片温度值（°C）
 */
float INA228_getDIETEMP_C(INA228_Handle sensor)
{
	uint64_t value = INA228_readReg(sensor, INA228_dietemp_register);
	float data;

	// 转换为 2 的补码和有符号值
	if (value > 0x7FFF)
	{
		data = (float)value - 0x10000;
	}
	else
	{
		data = (float)value;
	}

	// 转换为摄氏度
	data = (data * 7.8125f) / 1000;

	return data;
}

/**
 * @brief 获取芯片温度值（华氏度）
 * @param sensor INA228传感器的句柄
 * @return 芯片温度值（°F）
 */
float INA228_getDIETEMP_F(INA228_Handle sensor)
{
	float data = INA228_getDIETEMP_C(sensor);

	// 转换为华氏度
	data = (data * (9.f / 5.f)) + 32.f;

	return data;
}

/**
 * @brief 获取电流值（最低有效位）
 * @param sensor INA228传感器的句柄
 * @return 电流值（signed LSBs）
 */
float INA228_getCURRENT_signedLSB(INA228_Handle sensor)
{
	uint64_t value = INA228_readReg(sensor, INA228_current_register);
	float data;

	// 删除保留位
	value = value >> 4;

	// 转换为 2 的补码和有符号值
	if (value > 0x7FFFF)
	{
		data = (float)value - 0x100000;
	}
	else
	{
		data = (float)value;
	}

	return data;
}

/**
 * @brief 获取电流值（安培）
 * @param sensor INA228传感器的句柄
 * @return 电流值（A）
 */
float INA228_getCURRENT_A(INA228_Handle sensor)
{
	float data = INA228_getCURRENT_signedLSB(sensor);

	data = data * sensor->currentlsb;

	return data;
}

/**
 * @brief 获取功率值（最低有效位）
 * @param sensor INA228传感器的句柄
 * @return 功率值（signed LSBs）
 */
float INA228_getPOWER_signedLSB(INA228_Handle sensor)
{
	uint64_t value = INA228_readReg(sensor, INA228_power_register);
	float data;

	data = (float)value;

	return data;
}

/**
 * @brief 获取功率值（瓦特）
 * @param sensor INA228传感器的句柄
 * @return 功率值（W）
 */
float INA228_getPOWER_W(INA228_Handle sensor)
{
	float data = INA228_getPOWER_signedLSB(sensor);

	data = data * sensor->currentlsb * 3.2f;

	return data;
}

/**
 * @brief 获取能量值（最低有效位）
 * @param sensor INA228传感器的句柄
 * @return 能量值（signed LSBs）
 */
double INA228_getENERGY_signedLSB(INA228_Handle sensor)
{
	uint64_t value = INA228_readReg(sensor, INA228_energy_register);
	double data;

	data = (double)value;

	return data;
}

/**
 * @brief 获取能量值（焦耳）
 * @param sensor INA228传感器的句柄
 * @return 能量值（J）
 */
double INA228_getENERGY_J(INA228_Handle sensor)
{
	double data = INA228_getENERGY_signedLSB(sensor);

	data = data * sensor->currentlsb * 51.2f;

	return data;
}

/**
 * @brief 获取电荷值（以LSB为单位的有符号值）
 * @param sensor INA228传感器的句柄
 * @return 电荷值（signed LSBs）
 */
double INA228_getCHARGE_signedLSB(INA228_Handle sensor)
{
	uint64_t value = INA228_readReg(sensor, INA228_charge_register);
	double data;

	// 转换为 2 的补码和有符号值
	if (value > 0x7FFFFFFFFF)
	{
		data = (double)value - 0x10000000000;
	}
	else
	{
		data = (double)value;
	}

	return data;
}

/**
 * @brief 获取电荷值（库伦）
 * @param sensor: INA228传感器的句柄
 * @return 电荷值（C）
 */
double INA228_getCHARGE_C(INA228_Handle sensor)
{
	double data = INA228_getCHARGE_signedLSB(sensor);

	data = data * sensor->currentlsb;

	return data;
}
