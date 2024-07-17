#ifndef __INA228_H
#define __INA228_H

#include <stdint.h>
#include "i2c.h"

/* 配置寄存器 */
#define INA228_config_register 0x00U // 配置寄存器地址
/*
 * 复位位。将该位设置为“1”会生成一个与上电复位相同的系统复位
 */
#define INA228_config_register_rst_NormalOperation 0x0000U // 正常运行
#define INA228_config_register_rst_SystemReset 0x8000U     // 系统复位将寄存器设置为默认值
/*
 * 将累积寄存器 ENERGY 和 CHARGE 的内容复位为 0
 */
#define INA228_config_register_rstacc_NormalOperation 0x0000U               // 正常运行
#define INA228_config_register_rstacc_ClearENERGYandCHARGEregisters 0x4000U // 将 Energy 和 Charge 寄存器清除为默认值
/*
 * 外部分流器的温度补偿
 */
#define INA228_config_register_tempcomp_Shunttemperaturecompensationdisabled 0x0000U // 禁用分流温度补偿
#define INA228_config_register_tempcomp_Shunttemperaturecompensationenabled 0x0020U  // 启用分流温度补偿
/*
 * IN+ 和 IN– 之间的分流满量程范围选择
 */
#define INA228_config_register_adcrange_16384mV 0x0000U //  ±163.84mV
#define INA228_config_register_adcrange_4096mV 0x0010U  // ±40.96mV
/* ADC配置寄存器 */
#define INA228_adc_config_register 0x01U // ADC配置寄存器地址
/*
 * 用户可以针对总线电压、分流电压或温度测量，设置 MODE 位以启用连续模式或触发模式。
 */
#define INA228_adc_config_register_mode_Shutdown0 0x0000U                                               // 关断
#define INA228_adc_config_register_mode_Triggeredbusvoltagesingleshot 0x1000U                           // 触发总线电压，单冲
#define INA228_adc_config_register_mode_Triggeredshuntvoltagesingleshot 0x2000U                         // 触发分流电压，单冲
#define INA228_adc_config_register_mode_Triggeredshuntvoltageandbusvoltagesingleshot 0x3000U            // 触发分流电压和总线电压，单冲
#define INA228_adc_config_register_mode_Triggeredtemperaturesingleshot 0x4000U                          // 触发温度，单冲
#define INA228_adc_config_register_mode_Triggeredtemperatureandbusvoltagesingleshot 0x5000U             // 触发温度和总线电压，单冲
#define INA228_adc_config_register_mode_Triggeredtemperatureandshuntvoltagesingleshot 0x6000U           // 触发温度和分流电压，单冲
#define INA228_adc_config_register_mode_Triggeredbusvoltageshuntvoltageandtemperaturesingleshot 0x7000U // 触发总线电压、分流电压和温度，单冲
#define INA228_adc_config_register_mode_Shutdown1 0x8000U                                               // 关断
#define INA228_adc_config_register_mode_Continuousbusvoltageonly 0x9000U                                // 仅连续总线电压
#define INA228_adc_config_register_mode_Continuousshuntvoltageonly 0xA000U                              // 仅连续分流电压
#define INA228_adc_config_register_mode_Continuousshuntandbusvoltage 0xB000U                            // 连续分流电压和总线电压
#define INA228_adc_config_register_mode_Continuoustemperatureonly 0xC000U                               // 仅连续温度
#define INA228_adc_config_register_mode_Continuousbusvoltageandtemperature 0xD000U                      // 连续总线电压和温度
#define INA228_adc_config_register_mode_Continuoustemperatureandshuntvoltage 0xE000U                    // 连续温度和分流电压
#define INA228_adc_config_register_mode_Continuousbusvoltageshuntvoltageandtemperature 0xF000U          // 连续总线电压、分流电压和温度
/*
 * 设置总线电压测量的转换时间
 */
#define INA228_adc_config_register_vbusct_50us 0x0000U
#define INA228_adc_config_register_vbusct_84us 0x0200U
#define INA228_adc_config_register_vbusct_150us 0x0400U
#define INA228_adc_config_register_vbusct_280us 0x0600U
#define INA228_adc_config_register_vbusct_540us 0x0800U
#define INA228_adc_config_register_vbusct_1052us 0x0A00U
#define INA228_adc_config_register_vbusct_2074us 0x0C00U
#define INA228_adc_config_register_vbusct_4120us 0x0E00U
/*
 * 设置分流电压测量的转换时间
 */
#define INA228_adc_config_register_vshct_50us 0x0000U
#define INA228_adc_config_register_vshct_84us 0x0040U
#define INA228_adc_config_register_vshct_150us 0x0080U
#define INA228_adc_config_register_vshct_280us 0x00C0U
#define INA228_adc_config_register_vshct_540us 0x0100U
#define INA228_adc_config_register_vshct_1052us 0x0140U
#define INA228_adc_config_register_vshct_2074us 0x0180U
#define INA228_adc_config_register_vshct_4120us 0x01C0U
/*
 * 设置温度测量的转换时间
 */
#define INA228_adc_config_register_vtct_50us 0x0000U
#define INA228_adc_config_register_vtct_84us 0x0008U
#define INA228_adc_config_register_vtct_150us 0x0010U
#define INA228_adc_config_register_vtct_280us 0x0018U
#define INA228_adc_config_register_vtct_540us 0x0020U
#define INA228_adc_config_register_vtct_1052us 0x0028U
#define INA228_adc_config_register_vtct_2074us 0x0030U
#define INA228_adc_config_register_vtct_4120us 0x0038U
/*
 * 选择 ADC 样片平均计数。平均值计算设置适用于所有有效输入。当 >0h 时，将在平均值计算完成后更新输出寄存器。
 */
#define INA228_adc_config_register_avg_1 0x0000U
#define INA228_adc_config_register_avg_4 0x0001U
#define INA228_adc_config_register_avg_16 0x0002U
#define INA228_adc_config_register_avg_64 0x0003U
#define INA228_adc_config_register_avg_128 0x0004U
#define INA228_adc_config_register_avg_256 0x0005U
#define INA228_adc_config_register_avg_512 0x0006U
#define INA228_adc_config_register_avg_1024 0x0007U
/* 分流校准寄存器 */
#define INA228_shunt_cal_register 0x02U // 分流校准寄存器地址
#define INA228_shunt_cal_register_reserved0_ENABLE 0x8000U
#define INA228_shunt_cal_register_reserved0_DISABLE 0x0000U
/* 分流温度系数寄存器 */
#define INA228_shunt_tempco_register 0x03U
/* 分流电压测量寄存器 */
#define INA228_vshunt_register 0x04U // 分流输出上测得的差分电压。二进制补码值。
/* 总线电压测量寄存器 */
#define INA228_vbus_register 0x05U // 总线电压输出。二进制补码，但始终为正。
/* 内部芯片温度测量寄存器 */
#define INA228_dietemp_register 0x06U // 内部芯片温度测量。二进制补码值。
/* 电流结果寄存器 */
#define INA228_current_register 0x07U // 计算得出的电流输出（单位为安培）。二进制补码值。
/* 功率结果寄存器 */
#define INA228_power_register 0x08U // 计算得出的功率输出。输出值（单位为瓦特）。无符号表示。正值。
/* 电能结果寄存器 */
#define INA228_energy_register 0x09U // 计算得出的电能输出。输出值以焦耳为单位。无符号表示。正值。
/* 电荷结果寄存器 */
#define INA228_charge_register 0x0AU // 计算得出的电荷输出。输出值以库仑为单位。二进制补码值。
/* 诊断和警报寄存器 */
#define INA228_diag_alrt_register 0x0BU // 诊断和警报寄存器地址
/*
 * 当警报锁存使能位设置为透明模式时，如果故障已被清除，则 ALERT引脚和标志位复位为空闲状态。
 * 当警报锁存使能位设置为锁存模式时，ALERT 引脚和 ALERT 标志位在故障后保持有效，直到已读取 DIAG_ALRT 寄存器。
 */
#define INA228_diag_alrt_register_alatch_Transparent 0x0000U     // 透明模式
#define INA228_diag_alrt_register_alatch_LatchedAlertpin 0x8000U // 锁存模式
/*
 * 将该位设置为高电平可配置在置位转换就绪标志（位 1）时要置位的ALERT 引脚，指示转换周期已完成。
 */
#define INA228_diag_alrt_register_cnvr_DisableconversionreadyflagonALERTpin 0x0000U // 禁用 ALERT 引脚上的转换就绪标志
#define INA228_diag_alrt_register_cnvr_EnablesconversionreadyflagonALERTpin 0x4000U // 启用 ALERT 引脚上的转换就绪标志
/*
 * 启用时，ALERT 功能置位为完成的平均值。
 * 这样可以灵活地将 ALERT 延迟到平均值之后。
 */
#define INA228_diag_alrt_register_slowalert_ALERTcomparisononnonaveragedADCvalue 0x0000U // 非平均 (ADC) 值上的 ALERT 比较
#define INA228_diag_alrt_register_slowalert_ALERTcomparisononaveragedvalue 0x2000U       // 平均值上的 ALERT 比较
/*
 * 警报极性位设定 ALERT 引脚极性。
 */
#define INA228_diag_alrt_register_apol_Normalactivelowopendrain 0x0000U    // 正常（低电平有效，开漏）
#define INA228_diag_alrt_register_apol_Invertedactivehighopendrain 0x1000U // 正常（低电平有效，开漏）
/*
 * 该位表示 ENERGY 寄存器的运行状况。
 * 如果 40 位 ENERGY 寄存器已溢出，则该位设置为 1。
 */
#define INA228_diag_alrt_register_energyof_ENABLE 0x0800U
#define INA228_diag_alrt_register_energyof_DISABLE 0x0000U
#define INA228_diag_alrt_register_chargeof_ENABLE 0x0400U
#define INA228_diag_alrt_register_chargeof_DISABLE 0x0000U
#define INA228_diag_alrt_register_mathof_ENABLE 0x0200U
#define INA228_diag_alrt_register_mathof_DISABLE 0x0000U
#define INA228_diag_alrt_register_reserved0_ENABLE 0x0100U
#define INA228_diag_alrt_register_reserved0_DISABLE 0x0000U
#define INA228_diag_alrt_register_tmpol_ENABLE 0x0080U
#define INA228_diag_alrt_register_tmpol_DISABLE 0x0000U
#define INA228_diag_alrt_register_shntol_ENABLE 0x0040U
#define INA228_diag_alrt_register_shntol_DISABLE 0x0000U
#define INA228_diag_alrt_register_shntul_ENABLE 0x0020U
#define INA228_diag_alrt_register_shntul_DISABLE 0x0000U
#define INA228_diag_alrt_register_busol_ENABLE 0x0010U
#define INA228_diag_alrt_register_busol_DISABLE 0x0000U
#define INA228_diag_alrt_register_busul_ENABLE 0x0008U
#define INA228_diag_alrt_register_busul_DISABLE 0x0000U
#define INA228_diag_alrt_register_pol_ENABLE 0x0004U
#define INA228_diag_alrt_register_pol_DISABLE 0x0000U
#define INA228_diag_alrt_register_cnvrf_ENABLE 0x0002U
#define INA228_diag_alrt_register_cnvrf_DISABLE 0x0000U
#define INA228_diag_alrt_register_memstat_ENABLE 0x0001U
#define INA228_diag_alrt_register_memstat_DISABLE 0x0000U
/* 分流过压阈值寄存器 */
#define INA228_sovl_register 0x0CU
/* 分流欠压阈值寄存器 */
#define INA228_suvl_register 0x0DU
/* 总线过压阈值寄存器 */
#define INA228_bovl_register 0x0EU
#define INA228_bovl_register_reserved0_ENABLE 0x8000U
#define INA228_bovl_register_reserved0_DISABLE 0x0000U
/* 总线欠压阈值寄存器 */
#define INA228_buvl_register 0x0FU
#define INA228_buvl_register_reserved0_ENABLE 0x8000U
#define INA228_buvl_register_reserved0_DISABLE 0x0000U
/* 温度高于上限阈值寄存器 */
#define INA228_temp_limit_register 0x10U
/* 功率高于上限阈值寄存器 */
#define INA228_pwr_limit_register 0x11U
/* 制造商 ID 寄存器 */
#define INA228_manufacturer_id_register 0x3EU
/* 设备 ID 寄存器 */
#define INA228_device_id_register 0x3FU

/*
 *  INA228_State 结构体
 *  INA228传感器的初始配置状态
 */
typedef struct INA228_State
{
  uint16_t configRegister;      // 配置寄存器
  uint16_t adcConfigRegister;   // ADC配置寄存器
  uint16_t shuntCalRegister;    // Shunt校准寄存器
  uint16_t shuntTempcoRegister; // Shunt温度系数寄存器
  uint16_t diagAlrtRegister;    // 诊断警报寄存器
  uint16_t sovlRegister;        // Shunt电压超量程下限寄存器
  uint16_t suvlRegister;        // Shunt电压超量程上限寄存器
  uint16_t bovlRegister;        // 总线电压超量程下限寄存器
  uint16_t buvlRegister;        // 总线电压超量程上限寄存器
  uint16_t tempLimitRegister;   // 温度限制寄存器
  uint16_t pwrLimitRegister;    // 功率限制寄存器

  uint16_t adcrange; // ADC量程，定义在配置寄存器中
  float currentlsb;  // 当前最小可测量单位值，用于计算

  I2C_HandleTypeDef *hi2c; // I2C总线句柄
  uint8_t devAddr;         // 传感器在I2C总线上的地址

} INA228_State;

/*
 *  INA228方法的第一个参数，指向INA228_State结构体的指针
 */
typedef INA228_State *INA228_Handle;

extern void INA228_writeReg(INA228_Handle sensor, uint8_t regAddr, uint16_t value);
extern void INA228_config(INA228_Handle sensor);
extern void INA228_setCURRENT_LSB(INA228_Handle sensor, float CURRENT_LSB);
extern uint64_t INA228_readReg(INA228_Handle sensor, uint8_t regAddr);
extern float INA228_getVSHUNT_mV(INA228_Handle sensor);
extern float INA228_getVBUS_V(INA228_Handle sensor);
extern float INA228_getDIETEMP_C(INA228_Handle sensor);
extern float INA228_getDIETEMP_F(INA228_Handle sensor);
extern float INA228_getCURRENT_signedLSB(INA228_Handle sensor);
extern float INA228_getCURRENT_A(INA228_Handle sensor);
extern float INA228_getPOWER_signedLSB(INA228_Handle sensor);
extern float INA228_getPOWER_W(INA228_Handle sensor);
extern double INA228_getENERGY_signedLSB(INA228_Handle sensor);
extern double INA228_getENERGY_J(INA228_Handle sensor);
extern double INA228_getCHARGE_signedLSB(INA228_Handle sensor);
extern double INA228_getCHARGE_C(INA228_Handle sensor);

#endif
