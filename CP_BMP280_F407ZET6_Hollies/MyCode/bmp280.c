#include "bmp280.h"
#include "math.h"
// 读取校准系数子函数
HAL_StatusTypeDef bmp280_read_calib_(I2C_HandleTypeDef *hi2c, uint16_t MemAddress, uint16_t *value)
{
    uint8_t temp[2];
    if (HAL_I2C_Mem_Read(hi2c, (BMP280_ADDRESS_READ << 1), MemAddress, I2C_MEMADD_SIZE_8BIT, temp, 2, 5000) == HAL_OK) {
        *value = (uint16_t)(((uint16_t)temp[1] << 8) | temp[0]);
        return HAL_OK;
    } else
        return HAL_ERROR;
}
// 读取校准系数
HAL_StatusTypeDef bmp280_read_calib(BMP280 *bmp280, I2C_HandleTypeDef *hi2c)
{
    bmp280_read_calib_(hi2c, BMP280_DIG_T1_LSB, (uint16_t *)&bmp280->dig_T1);
    bmp280_read_calib_(hi2c, BMP280_DIG_T2_LSB, (uint16_t *)&bmp280->dig_T2);
    bmp280_read_calib_(hi2c, BMP280_DIG_T3_LSB, (uint16_t *)&bmp280->dig_T3);
    bmp280_read_calib_(hi2c, BMP280_DIG_P1_LSB, (uint16_t *)&bmp280->dig_P1);
    bmp280_read_calib_(hi2c, BMP280_DIG_P2_LSB, (uint16_t *)&bmp280->dig_P2);
    bmp280_read_calib_(hi2c, BMP280_DIG_P3_LSB, (uint16_t *)&bmp280->dig_P3);
    bmp280_read_calib_(hi2c, BMP280_DIG_P4_LSB, (uint16_t *)&bmp280->dig_P4);
    bmp280_read_calib_(hi2c, BMP280_DIG_P5_LSB, (uint16_t *)&bmp280->dig_P5);
    bmp280_read_calib_(hi2c, BMP280_DIG_P6_LSB, (uint16_t *)&bmp280->dig_P6);
    bmp280_read_calib_(hi2c, BMP280_DIG_P7_LSB, (uint16_t *)&bmp280->dig_P7);
    bmp280_read_calib_(hi2c, BMP280_DIG_P8_LSB, (uint16_t *)&bmp280->dig_P8);
    bmp280_read_calib_(hi2c, BMP280_DIG_P9_LSB, (uint16_t *)&bmp280->dig_P9);
    return HAL_OK;
}
// bmp280初始化
HAL_StatusTypeDef bmp280_init(BMP280 *bmp280, I2C_HandleTypeDef *hi2c)
{
    uint8_t temp = 0;
    HAL_I2C_Mem_Read(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_CHIP_ID, I2C_MEMADD_SIZE_8BIT, &temp, 1, 5000);
    if (temp != 0x58) // 未识别到芯片，报错
        return HAL_ERROR;
    // 识别到芯片，先重置
    temp = 0xB6;
    if (HAL_I2C_Mem_Write(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_RESET, I2C_MEMADD_SIZE_8BIT, &temp, 1, 10000) != HAL_OK)
        return HAL_ERROR;
    switch (BMP280_MODE) {
        case 1:
            temp = BMP280_MODE_1_CTRL;
            break;
        case 2:
            temp = BMP280_MODE_2_CTRL;
            break;
        case 3:
            temp = BMP280_MODE_3_CTRL;
            break;
        case 4:
            temp = BMP280_MODE_4_CTRL;
            break;
        case 5:
            temp = BMP280_MODE_5_CTRL;
            break;
        case 6:
            temp = BMP280_MODE_6_CTRL;
            break;
        default:
            return HAL_ERROR;
    }
    if (HAL_I2C_Mem_Write(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_CTRL_MEAS, I2C_MEMADD_SIZE_8BIT, &temp, 1, 10000) != HAL_OK)
        return HAL_ERROR; // 写入失败
    HAL_Delay(100);
    switch (BMP280_MODE) {
        case 1:
            temp = BMP280_MODE_1_CONFIG;
            break;
        case 2:
            temp = BMP280_MODE_2_CONFIG;
            break;
        case 3:
            temp = BMP280_MODE_3_CONFIG;
            break;
        case 4:
            temp = BMP280_MODE_4_CONFIG;
            break;
        case 5:
            temp = BMP280_MODE_5_CONFIG;
            break;
        case 6:
            temp = BMP280_MODE_6_CONFIG;
            break;
        default:
            return HAL_ERROR;
    }
    if (HAL_I2C_Mem_Write(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_CONFIG, I2C_MEMADD_SIZE_8BIT, &temp, 1, 10000) != HAL_OK)
        return HAL_ERROR; // 写入失败
    HAL_Delay(100);
    if (bmp280_read_calib(bmp280, hi2c) != HAL_OK)
        return HAL_ERROR;
    HAL_Delay(100);
    return HAL_OK;
}
// 转换温度采样值
float bmp280_compensate_T_int32(BMP280 *bmp280, int32_t t_raw)
{
    double var1, var2, T;
    var1           = (((double)t_raw) / 16384.0f - ((double)bmp280->dig_T1) / 1024.0f) * ((double)bmp280->dig_T2);
    var2           = ((((double)t_raw) / 131072.0f - ((double)bmp280->dig_T1) / 8192.0f) * (((double)t_raw) / 131072.0f - ((double)bmp280->dig_T1) / 8192.0f)) * ((double)bmp280->dig_T3);
    bmp280->t_fine = (int32_t)(var1 + var2);
    T              = (var1 + var2) / 5120.0f;
    return T;
}
// 转换气压原始值
float bmp280_compensate_P_int64(BMP280 *bmp280, int32_t p_raw)
{
    double var1, var2, p;
    var1 = ((double)bmp280->t_fine / 2.0f) - 64000.0f;
    var2 = var1 * var1 * ((double)bmp280->dig_P6) / 32768.0f;
    var2 = var2 + var1 * ((double)bmp280->dig_P5) * 2.0f;
    var2 = (var2 / 4.0f) + (((double)bmp280->dig_P4) * 65536.0f);
    var1 = (((double)bmp280->dig_P3) * var1 * var1 / 524288.0f + ((double)bmp280->dig_P2) * var1) / 524288.0f;
    var1 = (1.0f + var1 / 32768.0f) * ((double)bmp280->dig_P1);
    if (var1 == 0.0f) {
        return 0; // 避免除零引起的异常
    }
    p    = 1048576.0f - (double)p_raw;
    p    = (p - (var2 / 4096.0f)) * 6250.0f / var1;
    var1 = ((double)bmp280->dig_P9) * p * p / 2147483648.0f;
    var2 = p * ((double)bmp280->dig_P8) / 32768.0f;
    p    = p + (var1 + var2 + ((double)bmp280->dig_P7)) / 16.0f;
    p    = p / 100.0f;
    return p;
}
// 返回实际温度（℃）
float bmp280_getTemprature(BMP280 *bmp280, I2C_HandleTypeDef *hi2c)
{
    uint8_t temp[3];
    HAL_I2C_Mem_Read(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_TEMPERATURE_MSB, I2C_MEMADD_SIZE_8BIT, &temp[0], 1, 1000);
    HAL_I2C_Mem_Read(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_TEMPERATURE_LSB, I2C_MEMADD_SIZE_8BIT, &temp[1], 1, 1000);
    HAL_I2C_Mem_Read(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_TEMPERATURE_XLSB, I2C_MEMADD_SIZE_8BIT, &temp[2], 1, 1000);
    int32_t t_raw    = (((int32_t)temp[0]) << 12) | (((int32_t)temp[1]) << 4) | (((int32_t)temp[2]) >> 4);
    bmp280->t_result = bmp280_compensate_T_int32(bmp280, t_raw);
    return bmp280->t_result;
}
// 返回实际气压（hPa）
float bmp280_getPressure(BMP280 *bmp280, I2C_HandleTypeDef *hi2c)
{
    uint8_t temp[3];
    HAL_I2C_Mem_Read(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_PRESSURE_MSB, I2C_MEMADD_SIZE_8BIT, &temp[0], 1, 1000);
    HAL_I2C_Mem_Read(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_PRESSURE_LSB, I2C_MEMADD_SIZE_8BIT, &temp[1], 1, 1000);
    HAL_I2C_Mem_Read(hi2c, (BMP280_ADDRESS_READ << 1), BMP280_PRESSURE_XLSB, I2C_MEMADD_SIZE_8BIT, &temp[2], 1, 1000);
    int32_t p_raw    = (((int32_t)temp[0]) << 12) | (((int32_t)temp[1]) << 4) | (((int32_t)temp[2]) >> 4);
    bmp280->p_result = bmp280_compensate_P_int64(bmp280, p_raw);
    return bmp280->p_result;
}
// 返回海拔
float bmp280_getAltitude(BMP280 *bmp280, I2C_HandleTypeDef *hi2c)
{
    bmp280_getTemprature(bmp280, hi2c);
    bmp280_getPressure(bmp280, hi2c);
    return ((pow(1013.25f / bmp280->p_result, 1 / 5.257f) - 1.0) * (bmp280->t_result + 273.15f)) / 0.0065f;
}
