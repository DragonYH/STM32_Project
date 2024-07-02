#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include "gpio.h"
#include "stdlib.h"
//-----------------OLED总函数----------------

#define OLED_RES_Clr() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET) // RES
#define OLED_RES_Set() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET) // DC
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)

#define OLED_CS_Clr() HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET) // CS
#define OLED_CS_Set() HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)

#define OLED_CMD 0
#define OLED_DATA 1

#define WHICH_SPI &hspi1 // 硬件SPI结构体选择

void OLED_ClearPoint(uint8_t x, uint8_t y);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void OLED_DrawSquare(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1);
void OLED_Showdecimal(uint8_t x, uint8_t y, float num, uint8_t z_len, uint8_t f_len, uint8_t size2);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1);
void OLED_ScrollDisplay(uint8_t num, uint8_t space);
void OLED_WR_BP(uint8_t x, uint8_t y);
void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
void OLED_Printf(uint8_t str[]);
void OLED_Init(void);
#endif
