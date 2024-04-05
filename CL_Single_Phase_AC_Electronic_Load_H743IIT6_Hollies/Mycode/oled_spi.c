#include "oled_spi.h"
#include "oledfont.h"
#include "main.h"
#include "spi.h"

// OLED的显存
// 存放格式如下
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
// 向SSD1306写入一个字节
// dat:要写入的数据/命令
// cmd:数据/命令标志 0，表示命令；1，表示数据
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	if (cmd)
	{
		OLED_DC_Set(); // 命令/数据标志位置为1，则表示传送的是命令字节
	}
	else
		OLED_DC_Clr();						 // 命令/数据标志位置为0，则表示传送的是数据字节
	OLED_CS_Clr();							 // 片选信号为低，表示选中OLED
	HAL_SPI_Transmit(&hspi1, &dat, 1, 1000); // oled.c文件唯一修改的地方，这里是利用了hal库提供的SPI传送函数
	OLED_CS_Set();
	OLED_DC_Set();
}

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte((((x + 2) & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte(((x + 2) & 0x0f), OLED_CMD);
}
// 开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // 设置电荷泵命令字
	OLED_WR_Byte(0X14, OLED_CMD); // 开启电荷泵
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}
// 关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // 设置电荷泵命令字
	OLED_WR_Byte(0X10, OLED_CMD); // 关闭电荷泵
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}
// 清屏函数，清完后整个屏幕都是黑色的，没有一点光亮
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址
		OLED_WR_Byte(0x02, OLED_CMD);	  // 设置起始列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置起始列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(0, OLED_DATA);
	} // 更新显示
}

// 在指定位置显示一个字符，包括部分字符
// x:0~127
// y:0~6
// mode:0,反白显示；1，正常显示
// size:选择字体大小 16/12
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; // 得到偏移后的值
	if (x > Max_Column - 1)
	{
		x = 0;
		y = y + 2;
	}
	if (SIZE == 16)
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 8; i++)
			OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 8; i++)
			OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
	}
	else
	{
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 6; i++)
			OLED_WR_Byte(F6x8[c][i], OLED_DATA);
	}
}
// m^n函数
uint32_t oled_pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}
// 显示两个数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// mode:0:填充模式；1:叠加模式
// num:数值(0~4294967295);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (size / 2) * t, y, ' ');
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (size / 2) * t, y, temp + '0');
	}
}
// 显示一个字符串
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr)
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{
		OLED_ShowChar(x, y, chr[j]);
		x += 8;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		j++;
	}
}
// 显示汉字
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no)
{
	uint8_t t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
		adder += 1;
	}
}
/*显示BMP图片。x的范围为0~127，y的页得的范围0~7*/
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

// 初始化SSD1306
void OLED_Init(void)
{
	// 复位OLED
	OLED_RST_Clr(); // 拉低复位引脚
	HAL_Delay(200); // 延时200毫秒
	OLED_RST_Set(); // 拉高复位引脚

	// 关闭OLED面板
	OLED_WR_Byte(0xAE, OLED_CMD); // 关闭面板指令
	OLED_WR_Byte(0x02, OLED_CMD); // 设置低列地址
	OLED_WR_Byte(0x10, OLED_CMD); // 设置高列地址
	OLED_WR_Byte(0x40, OLED_CMD); // 设置起始行地址，映射RAM显示起始行（0x00~0x3F）
	OLED_WR_Byte(0x81, OLED_CMD); // 设置对比度控制寄存器
	OLED_WR_Byte(0xCF, OLED_CMD); // 设置SEG输出电流亮度
	OLED_WR_Byte(0xA1, OLED_CMD); // 设置SEG/列映射，0Xa0左右反置，0Xa1正常
	OLED_WR_Byte(0xC8, OLED_CMD); // 设置COM/行扫描方向，0Xc0上下反置，0Xc8正常
	OLED_WR_Byte(0xA6, OLED_CMD); // 设置正常显示
	OLED_WR_Byte(0xA8, OLED_CMD); // 设置多路复用比（1到64）
	OLED_WR_Byte(0x3f, OLED_CMD); // 1/64 duty
	OLED_WR_Byte(0xD3, OLED_CMD); // 设置显示偏移，移动映射RAM计数器（0x00~0x3F）
	OLED_WR_Byte(0x00, OLED_CMD); // 不偏移
	OLED_WR_Byte(0xd5, OLED_CMD); // 设置显示时钟分频比/振荡器频率
	OLED_WR_Byte(0x80, OLED_CMD); // 设置分频比，设置时钟为100帧/秒
	OLED_WR_Byte(0xD9, OLED_CMD); // 设置预充电周期
	OLED_WR_Byte(0xF1, OLED_CMD); // 设置预充电为15个时钟，放电为1个时钟
	OLED_WR_Byte(0xDA, OLED_CMD); // 设置COM引脚硬件配置
	OLED_WR_Byte(0x12, OLED_CMD);
	OLED_WR_Byte(0xDB, OLED_CMD); // 设置vcomh
	OLED_WR_Byte(0x40, OLED_CMD); // 设置VCOM取消电平
	OLED_WR_Byte(0x20, OLED_CMD); // 设置页面寻址模式（0x00/0x01/0x02）
	OLED_WR_Byte(0x02, OLED_CMD);
	OLED_WR_Byte(0x8D, OLED_CMD); // 设置充电泵使能/禁用
	OLED_WR_Byte(0x14, OLED_CMD); // 设置（0x10）禁用
	OLED_WR_Byte(0xA4, OLED_CMD); // 禁用整个显示（0xa4/0xa5）
	OLED_WR_Byte(0xA6, OLED_CMD); // 禁用反相显示（0xa6/a7）
	OLED_WR_Byte(0xAF, OLED_CMD); // 打开OLED面板

	OLED_WR_Byte(0xAF, OLED_CMD); /*显示开*/
	OLED_Clear();				  // 清除显示
}
