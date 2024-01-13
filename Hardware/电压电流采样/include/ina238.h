#ifndef INA238_H
#define INA238_H

#include "main.h"

#define ADDR_READ	0x80	//器件读地址
#define ADDR_WRITE	0x81	//器件写地址
#define CONFIG		0x00	//配置
#define ADC_CONFIG	0x01	//ADC配置
#define SHUNT_CAL	0x02	//分流校准
#define	VSHUNT		0x04	//分流电压测量
#define	VBUS		0x05	//总线电压测量
#define	DIETEMP		0x06	//温度测量
#define CURRENT		0x07	//电流结果
#define	POWER		0x08	//功率结果
#define	DIAG_ALRT	0x0b	//诊断标志和警报
#define	SOVL		0x0c	//分流过压阈值
#define	SUVL		0x0d	//分流欠压阈值
#define	BOVL		0x0e	//总线过压阈值
#define BUVL		0x0f	//总线欠压阈值
#define	TEMP_LIMIT	0x10	//温度高于上限阈值
#define	PWR_LIMIT	0x11	//功率高于上限阈值
#define	MANUFACTURER_ID	0x3e	//制造商ID
#define	DEVICE_ID	0x3f	//器件ID

//芯片初始化
void INA238_init(I2C_HandleTypeDef* hi2c, int8_t error);
//获取采样电压
double INA238_getVolt(I2C_HandleTypeDef* hi2c);
//获取采样电流
double INA238_getCurrent(I2C_HandleTypeDef* hi2c);
//获取功率
double INA238_getPower(I2C_HandleTypeDef* hi2c);
//获取温度
double INA238_getTemperature(I2C_HandleTypeDef* hi2c);
#endif //INA238_H
