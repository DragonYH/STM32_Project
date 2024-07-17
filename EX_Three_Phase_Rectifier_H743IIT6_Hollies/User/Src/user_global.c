#include "user_global.h"

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
    // shuntCalRegister = 13107.2 * 1e6 * 预计最大电流 / 524288 * 采样电阻
    .shuntCalRegister = (uint16_t)(13107.2f * 1e6 * 5.f / 524288.f * 0.02f),
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
    // currentlsb = 预计最大电流 / 524288
    .currentlsb = 5.f / 524288.f,

    /* 传感器的I2C总线句柄和地址 */
    .hi2c = &hi2c1,
    .devAddr = 0x80U,
};
const INA228_Handle INA228_0 = &INA228_0_state;
float U = 0; // 直流电压
float I = 0; // 直流电流

float M = 1.f; // 调制比

pll_Signal_V *signal_V;
pll_Signal_I *signal_I;

volatile enum state runState = START;

arm_fir_instance_f32 *fir_Va, *fir_Vb, *fir_Vc;
arm_fir_instance_f32 *fir_Ia, *fir_Ib, *fir_Ic;
float firState_Va[21] = {0}, firState_Vb[21] = {0}, firState_Vc[21] = {0};
float firState_Ia[21] = {0}, firState_Ib[21] = {0}, firState_Ic[21] = {0};
