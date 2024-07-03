#include "user.h"
/* AD7606采样数值存储 */
float adcValue[8] = {0};
/* ina228配置 */
static INA228_State INA228_0_state = {

    /* 配置和设置 */
    .configRegister = (INA228_config_register_rst_NormalOperation |
                       INA228_config_register_rstacc_NormalOperation |
                       0x0000U | // 以 2ms 的步长设置初始 ADC 转换的延迟
                       INA228_config_register_tempcomp_Shunttemperaturecompensationdisabled |
                       INA228_config_register_adcrange_16384mV),
    .adcConfigRegister = (INA228_adc_config_register_mode_Continuousbusvoltageshuntvoltageandtemperature |
                          INA228_adc_config_register_vbusct_150us |
                          INA228_adc_config_register_vshct_150us |
                          INA228_adc_config_register_vtct_150us |
                          INA228_adc_config_register_avg_64),
    .shuntCalRegister = 0x1000U,
    .shuntTempcoRegister = 0x0000U, /* TEMPCO is 0 ppm/°C */
    .diagAlrtRegister = (INA228_diag_alrt_register_alatch_Transparent |
                         INA228_diag_alrt_register_cnvr_DisableconversionreadyflagonALERTpin |
                         INA228_diag_alrt_register_slowalert_ALERTcomparisononnonaveragedADCvalue |
                         INA228_diag_alrt_register_apol_Normalactivelowopendrain),
    .sovlRegister = 0x7FFFU,
    .suvlRegister = 0x8000U,
    .bovlRegister = 0x7FFFU,
    .buvlRegister = 0x0000U,
    .tempLimitRegister = 0x7FFFU,
    .pwrLimitRegister = 0xFFFFU,

    .adcrange = INA228_config_register_adcrange_16384mV,
    .currentlsb = 0,

    /* 传感器的I2C总线句柄和地址 */
    .hi2c = &hi2c1,
    .devAddr = 0x80U,

};
const INA228_Handle INA228_0 = &INA228_0_state;
float U = 0;
float C = 0;
