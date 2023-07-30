#ifndef __BMP280_H__
#define __BMP280_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * 如果 SDO 引脚为高电平，则 BMP280 或 BME280 地址0x77 0x76，如果
 * SDO引脚为低电平。
 */

#define BMP280_I2C_ADDRESS_0 0x76
#define BMP280_I2C_ADDRESS_1 0x77

#define BMP280_CHIP_ID       0x58 /* BMP280 has chip-id 0x58 */
#define BME280_CHIP_ID       0x60 /* BME280 has chip-id 0x60 */

/**
 * BMP280 模块操作模式。
 * Force - 测量由用户启动。
 * Normal - 继续测量。
 */
typedef enum {
    BMP280_MODE_SLEEP  = 0,
    BMP280_MODE_FORCED = 1,
    BMP280_MODE_NORMAL = 3
} BMP280_Mode;

typedef enum {
    BMP280_FILTER_OFF = 0,
    BMP280_FILTER_2   = 1,
    BMP280_FILTER_4   = 2,
    BMP280_FILTER_8   = 3,
    BMP280_FILTER_16  = 4
} BMP280_Filter;

/**
 * 压力过采样设置
 */
typedef enum {
    BMP280_SKIPPED         = 0, /* no measurement  */
    BMP280_ULTRA_LOW_POWER = 1, /* oversampling x1 */
    BMP280_LOW_POWER       = 2, /* oversampling x2 */
    BMP280_STANDARD        = 3, /* oversampling x4 */
    BMP280_HIGH_RES        = 4, /* oversampling x8 */
    BMP280_ULTRA_HIGH_RES  = 5  /* oversampling x16 */
} BMP280_Oversampling;

/**
 * 在正常模式下测量间隔时间待机
 */
typedef enum {
    BMP280_STANDBY_05   = 0, /* stand by time 0.5ms */
    BMP280_STANDBY_62   = 1, /* stand by time 62.5ms */
    BMP280_STANDBY_125  = 2, /* stand by time 125ms */
    BMP280_STANDBY_250  = 3, /* stand by time 250ms */
    BMP280_STANDBY_500  = 4, /* stand by time 500ms */
    BMP280_STANDBY_1000 = 5, /* stand by time 1s */
    BMP280_STANDBY_2000 = 6, /* stand by time 2s BMP280, 10ms BME280 */
    BMP280_STANDBY_4000 = 7, /* stand by time 4s BMP280, 20ms BME280 */
} BMP280_StandbyTime;

/**
 * BMP280模块的配置参数。
 * 使用功能bmp280_init_default_params使用默认配置。
 */
typedef struct {
    BMP280_Mode mode;
    BMP280_Filter filter;
    BMP280_Oversampling oversampling_pressure;
    BMP280_Oversampling oversampling_temperature;
    BMP280_Oversampling oversampling_humidity;
    BMP280_StandbyTime standby;
} bmp280_params_t;

typedef struct {
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

    /* BME280 的湿度补偿 */
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;

    uint16_t addr;

    I2C_HandleTypeDef *i2c;

    bmp280_params_t params;

    uint8_t id; /* Chip ID */

} BMP280_HandleTypedef;

/**
 * 初始化默认参数。
 * 默认配置：
 * 模式：Normal
 * 过滤器：OFF
 * 过采样：x4
 * 待机时间：250ms
 */
void bmp280_init_default_params(bmp280_params_t *params);

/**
 * 初始化BMP280模块，探头为设备，软复位设备，
 * 读取校准常数，并使用提供的
 * 参数。成功时返回 true，否则返回 false。
 *
 * 假定 I2C 地址已在开发中初始化，并且
 * 可以是BMP280_I2C_ADDRESS_0，也可以是BMP280_I2C_ADDRESS_1。如果 I2C
 * 地址未知，然后依次尝试初始化每个。
 *
 * 可能会再次调用此函数以软重置设备并再次初始化。
 */
bool bmp280_init(BMP280_HandleTypedef *dev, bmp280_params_t *params);

/**
 * 以强制模式开始测量。
 * 此调用后，模块仍处于强制模式。
 * 不要在正常模式下调用此方法。
 */
bool bmp280_force_measurement(BMP280_HandleTypedef *dev);

/**
 * 检查 BMP280 是否忙于测量温度/压力。
 * 如果 BMP280 繁忙，则返回 true。
 */
bool bmp280_is_measuring(BMP280_HandleTypedef *dev);

/**
 * 读取补偿温度和压力数据：
 *
 * 温度以摄氏度乘以 100。
 *
 * 帕斯卡压力，定点 24 位整数 8 位分数格式。
 *
 * 湿度是可选的，仅针对 BME280 读取，以相对百分比表示
 * 湿度为定点22位中间和10位分数格式。
 */
bool bmp280_read_fixed(BMP280_HandleTypedef *dev, int32_t *temperature,
                       uint32_t *pressure, uint32_t *humidity);

/**
 * 读取补偿温度和压力数据：
 * 温度以摄氏度为单位。
 * 帕斯卡压力。
 * 湿度是可选的，仅针对 BME280 读取，以相对百分比表示
 * 湿度。
 */
bool bmp280_read_float(BMP280_HandleTypedef *dev, float *temperature,
                       float *pressure, float *humidity);

#endif // __BMP280_H__
