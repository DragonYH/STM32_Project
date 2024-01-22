#ifndef _ADS1256_H
#define _ADS1256_H
/****************************加载头文件区***********************/
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include "math.h"
#include "stdlib.h"
extern int32_t adcVaule;
extern double voltage;
/****************************加载宏定义，注意修改***********************/

#define CS_PIN      CS_Pin
#define CS_PORT     CS_GPIO_Port

#define DRDY_PIN    DRDY_Pin
#define DRDY_PORT   DRDY_GPIO_Port

/****************************定义ads1256寄存器地址**************/
#define STATUS  0x00                //状态寄存器
#define MUX     0x01                //通道切换寄存器
#define ADCON   0x02                //ADC控制寄存器
#define DRATE   0x03                //ADC速率设置寄存器
#define IO      0x04                //通用IO寄存器
#define OFC0    0x05
#define OFC1    0x06
#define OFC2    0x07
#define FSC0    0x08
#define FSC1    0x09
#define FSC2    0x0A

/****************************定义ads1256命令*******************/
#define WAKEUP      0x00
#define RDATA       0x01
#define TDATAC      0x03
#define SDATAC      0x0F
#define RREG        0x10
#define WREG        0x50
#define SELFCAL     0xF0
#define SELFOCAL    0xF1
#define SELFGCAL    0xF2
#define SYSOCAL     0xF3
#define SYSGCAL     0xF4
#define SYNC        0xFC
#define STANDBY     0xFD
#define RESET       0xFE

/*****************************定义增益代码*********************/
#define GAIN_1   0x00
#define GAIN_2   0x01
#define GAIN_4   0x02
#define GAIN_8   0x03
#define GAIN_16  0x04
#define GAIN_32  0x05
#define GAIN_64  0x06

/*****************************定义设置ads1256采集速率代码*******/
#define RATE_30000 0xF0
#define RATE_15000 0xE0
#define RATE_7500  0xD0                       
#define RATE_3750  0xC0
#define RATE_2000  0xB0
#define RATE_1000  0xA1
#define RATE_500   0x92
#define RATE_100   0x82
#define RATE_60    0x72
#define RATE_50    0x63
#define RATE_30    0x53
#define RATE_25    0x43
#define RATE_15    0x33
#define RATE_10    0x23
#define RATE_5     0x13
#define RATE_2_5   0x03

/*****************************定义ads1256通道选择**************/
//定义正输入通道
#define MUXP_AIN0  0x00
#define MUXP_AIN1  0x10
#define MUXP_AIN2  0x20
#define MUXP_AIN3  0x30
#define MUXP_AIN4  0x40
#define MUXP_AIN5  0x50
#define MUXP_AIN6  0x60
#define MUXP_AIN7  0x70
#define MUXP_AINCOM 0x80
//定义负输入通道
#define MUXN_AIN0   0x00
#define MUXN_AIN1   0x01
#define MUXN_AIN2   0x02
#define MUXN_AIN3   0x03
#define MUXN_AIN4   0x04
#define MUXN_AIN5   0x05
#define MUXN_AIN6   0x06
#define MUXN_AIN7   0x07
#define MUXN_AINCOM 0x08
/****************************申明函数区************************/
void delayXus(uint16_t us);
void spiWriteByte(uint8_t txData);
uint8_t spiReadByte(void);


void spiWriteRegData(uint8_t regAdd, uint8_t regData);
void ads1256Init(void);
int32_t ads1256ReadValue(uint8_t channel);
float ADS1256_Get_Average(uint8_t channel, uint32_t times);
void setmode(uint8_t mode);
void setGain(uint8_t gain);
void setRate(uint8_t rate);
void disableInterrupt(void);
void enableInterrupt(void);

typedef struct ADS1256_Sample {
    float (*readAverageValue)(uint8_t channel, uint32_t times);
    void (*setGain)(uint8_t mode);
    void (*setRate)(uint8_t rate);
    void (*Init)(void);
    void (*writeRegData)(uint8_t regAdd, uint8_t regData);
}ADS1256_Sample;


extern ADS1256_Sample ADS1256;

#endif
