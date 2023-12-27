#ifndef _TMP117_H
#define _TMP117_H

#include "main.h"

uint8_t Tm117_Write_2byte( uint8_t reg_addr, uint16_t cmd );
uint8_t Tm117_Read_2byte( uint8_t reg_addr, uint16_t* reg_value );
uint8_t Tm117_Read_Temp_Reg_Value( int16_t* reg_value );
void Tmp117_Init();
float Code_Junction_Temperature_Measurement();


#endif