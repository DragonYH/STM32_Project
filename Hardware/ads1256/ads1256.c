#include "ads1256.h"
int32_t adcVaule = 0x00;
double voltage = 0x00;

ADS1256_Sample ADS1256 = {
    ADS1256_Get_Average,
    setGain,
    setRate,
    ads1256Init,
    spiWriteRegData,
};


void delayXus(uint16_t us) {
    uint16_t diff = 0xffff - 5 - us;
    //设置定时器的计数值
    __HAL_TIM_SET_COUNTER(&htim6, diff);
    //启动定时器计数
    HAL_TIM_Base_Start(&htim6);
    //判定计数结束
    while(diff < 0xffff - 5) {
        diff = __HAL_TIM_GET_COUNTER(&htim6);
    }
    //延时完成关闭定时器计数
    HAL_TIM_Base_Stop(&htim6);
}

/**
 * @brief 实现SPI协议总线发送一个字节的数据信息
 * @param txData 待发送的数据信息
 * @return 无
 */
void spiWriteByte(uint8_t txData) {
    uint8_t tempData = 0x00;
    HAL_SPI_TransmitReceive(&hspi1, &txData, &tempData, 1, 100);
}

/**
* @brief 实现SPI协议总监接受一个字节的数据信息
* @return 接受到的数据信息
*/
uint8_t spiReadByte(void) {
    uint8_t tempDataT = 0xff;
    uint8_t tempData = 0x00;
    HAL_SPI_TransmitReceive(&hspi1, &tempDataT, &tempData, 1, 100);
    return tempData;
}

/**
* @brief 向ads1256寄存器中写入一个字节的数据
* @param regAdd 寄存器地址
* @param regData 待写入的数据信息
*/
void spiWriteRegData(uint8_t regAdd, uint8_t regData) {
    //拉低SPI协议的CS引脚
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    //等待RDY的引脚变低
    while(HAL_GPIO_ReadPin(DRDY_PORT, DRDY_PIN));
    //写入寄存地地址
    spiWriteByte(WREG | (regAdd & 0x0F));
    //写入即将写入数据的个数
    spiWriteByte(0x00);
    //写入数据信息
    spiWriteByte(regData);
    //拉高SPI协议的CS引脚
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

/**
* @brief 初始化ads1256
*/
void ads1256Init(void) {
    disableInterrupt();
    while(HAL_GPIO_ReadPin(DRDY_PORT, DRDY_PIN));
    //开启芯片的自校准
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    spiWriteByte(SELFCAL);
    while(HAL_GPIO_ReadPin(DRDY_PORT, DRDY_PIN));
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
    //设置ads1256的状态寄存器
    spiWriteRegData(STATUS, 0x06);      //数据发送高位在前 自动校准 启用buf
    spiWriteRegData(MUX, MUXP_AIN0 | MUXN_AIN1); //单端模式
    //设置ads1256的增益
    spiWriteRegData(ADCON, GAIN_1);
    //设置ads采样速率
    spiWriteRegData(DRATE, RATE_2000);
    //设置IO状态
    spiWriteRegData(IO, 0x00);
    //再次进行校准
    while(HAL_GPIO_ReadPin(DRDY_PORT, DRDY_PIN));
    //开启芯片的自校准
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    spiWriteByte(SELFCAL);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
    enableInterrupt();
}

/**
* @brief 从ads1256中读取出相关数据信息
* @param channel 下一次转换通道
* @return 读取到的数据信息
*/
int32_t ads1256ReadValue(uint8_t channel) {
    int32_t sum = 0;
    //等待准备好信号变低
    while(HAL_GPIO_ReadPin(DRDY_PORT, DRDY_PIN));
    //设置下次转换的通道
    spiWriteRegData(MUX, channel);

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
    spiWriteByte(SYNC);     //发送同步命令
    delayXus(5);
    spiWriteByte(WAKEUP);   //发送唤醒命令
    delayXus(5);            //延时一下
    spiWriteByte(RDATA);    //发送读数据命令
    delayXus(25);
    sum |= (spiReadByte() << 16);
    sum |= (spiReadByte() << 8);
    sum |= (spiReadByte());
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
    if(sum > 0x7fffff)
        sum -= 0x1000000;
    adcVaule = sum;
    voltage = (float)(adcVaule * 5.0 / 8388607); //计算电压
    return sum;
}
/**
* @brief 实现ads的增益设置
*/
void setGain(uint8_t gain) {
    disableInterrupt();
    while(HAL_GPIO_ReadPin(DRDY_PORT, DRDY_PIN));
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);

    spiWriteRegData(ADCON, gain);

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
    enableInterrupt();
}
/**
* @brief 设置ads1256的采集速率
*/
void setRate(uint8_t rate) {
    disableInterrupt();
    while(HAL_GPIO_ReadPin(DRDY_PORT, DRDY_PIN));
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);

    spiWriteRegData(DRATE, rate);

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
    enableInterrupt();
}

/**
 * @brief 实现ADS1256均值滤波
 */
float ADS1256_Get_Average(uint8_t channel, uint32_t times)
{
    int i = 0;
    float Voltage_Average = 0;
    for(i = 0; i < times; i ++)
    {
        ads1256ReadValue(channel);
        Voltage_Average += voltage;
    }
    return Voltage_Average / (float)times;
}


/**
* @brief 实现屏蔽所有中断函数
*/
void disableInterrupt(void) {
    __set_PRIMASK(1);
}

/**
* @brief 开启全局中断
*/
void enableInterrupt(void) {
    __set_PRIMASK(0);
}
#if 0

#endif
