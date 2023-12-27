/**
 * @brief:  DS18B20.c
 * @author：点灯大师
 * @mail：mail_leiaho@126.com
 * @gitee: https://gitee.com/zleihao/
 * @note 以下驱动结合网上资料与 DS18B20 数据手册编写
 */

#include "bsp_ds18b20.h"

/**
 * @brief  初始化DQ端口
 * @param  无
 * @return 无
 * @note   配置ds18b20 DQ引脚连接的IO口
 */
void DQ_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DQ_GPIO_Port, DQ_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE2 */
  GPIO_InitStruct.Pin = DQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DQ_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief  将 DQ 设置为推挽输出模式 
 * @param  无
 * @return 无
 */
void DQ_GPIO_OUT(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		//开启时钟
		__HAL_RCC_GPIOE_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = DQ_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  //模式设置为推挽输出
		GPIO_InitStruct.Pull = GPIO_PULLUP;  				//带上拉
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		
		HAL_GPIO_Init(DQ_GPIO_Port,&GPIO_InitStruct);
}

/**
 * @brief  将 DQ 设置为输入模式 
 * @param  无
 * @return 无
 */
void DQ_GPIO_IN(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
		
		//开启时钟
		__HAL_RCC_GPIOE_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = DQ_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  //模式设置为浮空输入
		GPIO_InitStruct.Pull = GPIO_PULLUP;  			//带上拉
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		
		HAL_GPIO_Init(DQ_GPIO_Port,&GPIO_InitStruct);
}

 
/**
 * @brief  DS18B20 复位
 * @param  无
 * @return 无
 * @note 将总线拉低 480us - 960us 启动复位，然后等待 15us 检测存在脉冲
 */
void Ds18b20_Reset(void)
{
	//输出模式下
	DQ_GPIO_OUT();
	
	//拉低总线 750us
	DQ_Write(DQ_LEVEL_LOW);
	HAL_Delay_Us(750);
	
	//释放总线，等待存在脉冲
	DQ_Write(DQ_LEVEL_HIGHT);
	HAL_Delay_Us(15);
}

/**
 * @brief  检测存在脉冲	
 * @param  无
 * @return   存在脉冲: 0 
 *         不存在脉冲: 1
 */
uint8_t Ds18B20_CheckPulse(void)
{
	/* 超时计数，若设备不存在，需要退出，不能一直等待 */
	uint8_t Time_Count = 0;
	
	//将 GPIO 改成输入模式下
	DQ_GPIO_IN();
	
	/* 对应时序中的 DS18B20 等待 (15 - 60)us */
	while (DQ_Read && Time_Count < 100)
	{
		Time_Count++;
		HAL_Delay_Us(1);
	}
	
	/* 已经过去了 100us 存在脉冲还没到来，代表设备不存在 */
	if (Time_Count >= 100)
		return 1;   
	/* 脉冲到来，复位超时计数 */
	else
		Time_Count = 0;
	
	/* 对应时序中的 DS18B20 存在脉冲 (60 - 240)us */
	while (!DQ_Read && Time_Count < 240)
	{
		Time_Count++;
		HAL_Delay_Us(1);
	}
	
	/* 由时序图可知，存在脉冲最长不得超过 240us */
	if (Time_Count >= 240)
		return 1;
	else
		return 0;    //检测到存在脉冲
}

/**
 * @brief  读取1bit数据 
 * @param  无
 * @return 读取到1Bit的数据
 * @note   先将总线拉低 15us 后，读取总线状态
 */ 
uint8_t Ds18b20_Read_Bit(void)
{
	uint8_t dat;
	//输出模式
	DQ_GPIO_OUT();
	
	//先拉低总线 15us 后读取总线状态
	DQ_Write(DQ_LEVEL_LOW);
	HAL_Delay_Us(15);
	
	//下面要读取总线值，将引脚设置成输入模式
	DQ_GPIO_IN();
	
	if (DQ_Read == SET)
		dat = 1;
	else
		dat = 0;
	
	//读取周期至少 60us
	HAL_Delay_Us(50);
	
	return dat;
}

/** 
 * @brief  从 DS18B20 上读取 1Byte
 * @param  无
 * @return 读到的8位数据
 * @note   低位到高位
 */
uint8_t Ds18B20_Read_Byte(void)
{
	uint8_t data = 0x00,mask;
	
	for (mask = 0x01;mask != 0;mask <<= 1)
	{
		if (Ds18b20_Read_Bit() == SET)
			data |= mask;
		else
			data &= ~mask;
	}
	
	return data;
}

/**
 * @brief  写入一字节数据，低位先行
 * @param  data 写入的数据 
 * @return 无
 * @note  写0: 由时序图可知，拉低总线至少 60us 表示写 0
          写1: 由时序图可知，拉低总线大于 1us 并且小于 15us 后，紧接着拉高总线，总时间超过 60us
          写周期必须有 1us 的恢复时间
 */
void Ds18B20_Write_Byte(uint8_t data)
{
	uint8_t mask;
	
	for (mask = 0x01;mask != 0;mask <<= 1)
	{
		DQ_GPIO_OUT();
		
		//写0
		if ((data & mask) == RESET)
		{
			/* 拉低总线至少 60us */
			DQ_Write(DQ_LEVEL_LOW);
			HAL_Delay_Us(70);
			
			//2us 的恢复时间
			DQ_Write(DQ_LEVEL_HIGHT);
			HAL_Delay_Us(2);
		}
		else  //写1
		{
			/* 拉低总线大于 1us 并且小于 15us */
			DQ_Write(DQ_LEVEL_LOW);
			HAL_Delay_Us(9);
			
			/* 拉高总线，总时间超过 60us */
			DQ_Write(DQ_LEVEL_HIGHT);
			HAL_Delay_Us(55);
		}
	}
}

/** 
 * @brief  DS18B20 初始化
 * @param  无
 * @return 存在脉冲
 * @note   初始化包含: 复位、检测存在脉冲
 */
uint8_t Ds18b20_Init()
{
	Ds18b20_Reset();
	
	return Ds18B20_CheckPulse();
}

/**
 * @brief  获取温度
 * @param  无
 * @return 从DS18B20上读取到的温度值
 * @note   此获取方法为跳过 ROM 读取,适合于总线上只有一个设备
 */
float Ds18b20_Get_Temp(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	Ds18b20_Reset();	   
	Ds18B20_CheckPulse();	 
	Ds18B20_Write_Byte(SKIP_ROM);				         /* 跳过 ROM */
	Ds18B20_Write_Byte(CONVERT_TEMP);				     /* 开始转换 */
	
	Ds18b20_Reset();	   
	Ds18B20_CheckPulse();	 
	Ds18B20_Write_Byte(SKIP_ROM);				        /* 跳过 ROM */
	Ds18B20_Write_Byte(READ_SCRATCHPAD);				/* 读温度值 */
	
	tplsb = Ds18B20_Read_Byte();		 
	tpmsb = Ds18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* 负温度 */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}
