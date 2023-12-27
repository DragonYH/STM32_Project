#include "main.h"
#include "i2c.h"

#ifndef _BMP280_H_
#define _BMP280_H_
// 定义寄存器地址
#define BMP280_ADDRESS_READ     0x76 // 读地址
#define BMP280_ADDRESS_WRITE    0x77 // 写地址
#define BMP280_CHIP_ID          0xD0 // 芯片id
#define BMP280_RESET            0xE0 // 输入0xB6复位
#define BMP280_STATUS           0xF3 // 状态
#define BMP280_CTRL_MEAS        0xF4 // 设置设备的数据采集选项
#define BMP280_CONFIG           0xF5 // 设置设备的速率、过滤器和接口选项。在正常模式下对“config”寄存器的写入可能会被忽略
#define BMP280_PRESSURE_MSB     0xF7 // 压力数据高位
#define BMP280_PRESSURE_LSB     0xF8 // 压力数据低位
#define BMP280_PRESSURE_XLSB    0xF9 // 压力数据低位
#define BMP280_TEMPERATURE_MSB  0xFA // 温度数据高位
#define BMP280_TEMPERATURE_LSB  0xFB // 温度数据低位
#define BMP280_TEMPERATURE_XLSB 0xFC // 温度数据低位
// 校准系数，只读
#define BMP280_DIG_T1_LSB   0x88 // 温度校正系数1低位
#define BMP280_DIG_T1_MSB   0x89 // 温度校正系数1高位
#define BMP280_DIG_T2_LSB   0x8A // 温度校正系数2低位
#define BMP280_DIG_T2_MSB   0x8B // 温度校正系数2高位
#define BMP280_DIG_T3_LSB   0x8C // 温度校正系数3低位
#define BMP280_DIG_T3_MSB   0x8D // 温度校正系数3高位
#define BMP280_DIG_P1_LSB   0x8E // 压力校正系数1低位
#define BMP280_DIG_P1_MSB   0x8F // 压力校正系数1高位
#define BMP280_DIG_P2_LSB   0x90 // 压力校正系数2低位
#define BMP280_DIG_P2_MSB   0x91 // 压力校正系数2高位
#define BMP280_DIG_P3_LSB   0x92 // 压力校正系数3低位
#define BMP280_DIG_P3_MSB   0x93 // 压力校正系数3高位
#define BMP280_DIG_P4_LSB   0x94 // 压力校正系数4低位
#define BMP280_DIG_P4_MSB   0x95 // 压力校正系数4高位
#define BMP280_DIG_P5_LSB   0x96 // 压力校正系数5低位
#define BMP280_DIG_P5_MSB   0x97 // 压力校正系数5高位
#define BMP280_DIG_P6_LSB   0x98 // 压力校正系数6低位
#define BMP280_DIG_P6_MSB   0x99 // 压力校正系数6高位
#define BMP280_DIG_P7_LSB   0x9A // 压力校正系数7低位
#define BMP280_DIG_P7_MSB   0x9B // 压力校正系数7高位
#define BMP280_DIG_P8_LSB   0x9C // 压力校正系数8低位
#define BMP280_DIG_P8_MSB   0x9D // 压力校正系数8高位
#define BMP280_DIG_P9_LSB   0x9E // 压力校正系数9低位
#define BMP280_DIG_P9_MSB   0x9F // 压力校正系数9高位
#define BMP280_RESERVED_LSB 0xA0 // 保留
#define BMP280_RESERVED_MSB 0xA1 // 保留
// 功能模式
#define BMP280_MODE          6
#define BMP280_MODE_1_CTRL   0x57 // 模式1:手持设备低功耗
#define BMP280_MODE_1_CONFIG 0x2C
#define BMP280_MODE_2_CTRL   0x2F // 模式2:手持设备动态
#define BMP280_MODE_2_CONFIG 0x14
#define BMP280_MODE_3_CTRL   0x27 // 模式3:天气监测
#define BMP280_MODE_3_CONFIG 0xE0
#define BMP280_MODE_4_CTRL   0x2F // 模式4:电梯/楼层更换
#define BMP280_MODE_4_CONFIG 0x4C
#define BMP280_MODE_5_CTRL   0x17 // 模式5:掉落检测
#define BMP280_MODE_5_CONFIG 0x00
#define BMP280_MODE_6_CTRL   0x57 // 模式6:室内导航
#define BMP280_MODE_6_CONFIG 0x14
// 定义BMP280结构体
typedef struct
{
    // 结果值
    float t_result; // 温度
    float p_result; // 压力
    // 中间量
    int32_t t_fine;
    // 校准系数
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
} BMP280;
// 函数声明
HAL_StatusTypeDef bmp280_read_calib(BMP280 *bmp280, I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef bmp280_init(BMP280 *bmp280, I2C_HandleTypeDef *hi2c);
float bmp280_compensate_T_int32(BMP280 *bmp280, int32_t t_raw);
float bmp280_compensate_P_int64(BMP280 *bmp280, int32_t p_raw);
float bmp280_getTemprature(BMP280 *bmp280, I2C_HandleTypeDef *hi2c);
float bmp280_getPressure(BMP280 *bmp280, I2C_HandleTypeDef *hi2c);
float bmp280_getAltitude(BMP280 *bmp280, I2C_HandleTypeDef *hi2c);
#endif //_BMP280_H_
