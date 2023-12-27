#include "tmp117.h"
#include "i2c.h"

#define TMP117_IIC_ADDR_R  0x91 ///TM117的地址
#define TMP117_IIC_ADDR_W  0x90  ///写

#define TMP117_REG_SIZE  2  //16BIT=2X8BYE

#define TMP117_IIC_PORT  hi2c1
#define TMP117_RESOLUTION       (float)0.0078125  ///TM117的分辨率

/* pointer registers ----- */
// name			 										address		description
#define TMP117_TEMP_RES_REG 		0x00			// temperature result register		    R	
#define TMP117_CONFIG_REG		    0x01			// configuration register				R/W		
#define TMP117_THIGH_LIM_REG		0x02			// temperature high limit register		R/W                
#define TMP117_TLOW_LIM_REG			0x03			// temperature low limit register		R/W    
#define TMP117_EEPROM_UL_REG		0x04			// EEPROM unlock register	    R/W    
#define TMP117_EEPROM1_REG			0x05			// EEPROM1 register				R/W            
#define TMP117_EEPROM2_REG			0x06			// EEPROM2 register				R/W    
#define TMP117_TEMP_OFFSET_REG	    0x07			// temperature offset register	R/W                 
#define TMP117_EEPROM3_REG			0x08			// EEPROM3 register	
#define TMP117_DEVICE_ID_REG		0x0F			// device ID register		R   

///每个寄存器都是2个字节
uint8_t Tm117_Write_2byte( uint8_t reg_addr, uint16_t cmd )
{
    HAL_StatusTypeDef ret;

    uint8_t temp[TMP117_REG_SIZE + 1] = {0X00, 0X00};

    temp[2] = ( uint8_t )cmd;
    temp[1] = ( uint8_t )( cmd >> 8 );
    temp[0] = reg_addr;

    ret = HAL_I2C_Master_Transmit( &TMP117_IIC_PORT, TMP117_IIC_ADDR_W, temp, 3, HAL_MAX_DELAY );
    if ( ret != HAL_OK )
    {
        // DBG_PRINTF( "Error Tx I2C\r\n" );
        return 0;
    }
    return 1;
}


uint8_t Tm117_Read_2byte( uint8_t reg_addr, uint16_t* reg_value )
{
    HAL_StatusTypeDef ret;																			// return status of i2c

    uint8_t buffer[2];

    buffer[0] = reg_addr;

    ///
    ret = HAL_I2C_Master_Transmit( &TMP117_IIC_PORT, TMP117_IIC_ADDR_W, buffer, 1, HAL_MAX_DELAY );
    if ( ret != HAL_OK )
    {
        // DBG_PRINTF( "Error Tx I2C\r\n" );
        return 0;
    }

    ///
    ret = HAL_I2C_Master_Receive( &TMP117_IIC_PORT, TMP117_IIC_ADDR_R, buffer, 2, HAL_MAX_DELAY );
    if ( ret != HAL_OK )
    {
        // DBG_PRINTF( "Error Rx I2C\r\n" );
        return 0;
    }

    ///转换成温度数据
    *reg_value = ( buffer[0] << 8 ) | buffer[1];
    return 1;

}

///TMP117返回直接是补码形式，直接定义一个有符号的整形数量int16_t
uint8_t Tm117_Read_Temp_Reg_Value( int16_t* reg_value )
{
    HAL_StatusTypeDef ret;																			// return status of i2c

    uint8_t buffer[2];

    buffer[0] = TMP117_TEMP_RES_REG;

    ///先写要读取的寄存器地址
    ret = HAL_I2C_Master_Transmit( &TMP117_IIC_PORT, TMP117_IIC_ADDR_W, buffer, 1, HAL_MAX_DELAY );
    if ( ret != HAL_OK )
    {
        // DBG_PRINTF( "Error Tx I2C\r\n" );
        return 0;
    }

    ///
    ret = HAL_I2C_Master_Receive( &TMP117_IIC_PORT, TMP117_IIC_ADDR_R, buffer, 2, HAL_MAX_DELAY );
    if ( ret != HAL_OK )
    {
        // DBG_PRINTF( "Error Rx I2C\r\n" );
        return 0;
    }

    ///转换成温度数据
    *reg_value = ( buffer[0] << 8 ) | buffer[1];
    return 1;

}




void Tmp117_Init()
{
    Tm117_Write_2byte( TMP117_CONFIG_REG, 0x0220 );     //设置
    Tm117_Write_2byte( TMP117_TEMP_OFFSET_REG, 0x0000 ); //设置温度偏移量
    ///Tm117_Write_2byte(0x00, 0x00);   //设置解锁EEPROM
}

///NJSG
float Code_Junction_Temperature_Measurement()
{
    ///uint16_t val;
    int16_t temp_reg_val=0;
    int16_t temp_int16;
    float tempe_float=0.0;

    Tm117_Read_Temp_Reg_Value(&temp_reg_val ); ///到得带符号的整形数量
    
    tempe_float=(float)temp_reg_val; ///强制转换成浮点数 
    tempe_float=10*tempe_float*TMP117_RESOLUTION; ///只要小数点1位 得到环境温度 X10倍
    temp_int16=(int16_t)(tempe_float);  ///只保留整数，不要小数
    
    tempe_float=(float)(temp_int16); ///重新转为浮点数
    tempe_float=(tempe_float/10);  ///转成真实浮点温度，只有一位小数点
    
    return tempe_float;
    
//    sprintf ( string_temp, "TMP117 temper:%f\r\n", WorkPamData.code_temperature ); //和软件保持一致，显示为0.01精度
//    DBG_PRINTF( string_temp );
}
