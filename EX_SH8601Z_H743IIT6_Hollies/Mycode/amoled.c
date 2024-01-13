/**
 * @file amoled.c
 * @brief 主功能实现
 * @author Hollies https://github.com/DragonYH
 * @date 2023年8月22日
 */
#include "amoled.h"
#include "quadspi.h"

#define DISP_ROW 368	//行
#define DISP_COL 448	//列

QSPI_CommandTypeDef* AMOLED;

/**
 * @brief 输入命令
 * @param
 * @param
 * @return
 */
HAL_StatusTypeDef amoled_Init()
{
	AMOLED->
}

HAL_StatusTypeDef amoled_Cmd(uint8_t cmd)
{

	HAL_QSPI_Command(&hqspi, AMOLED, 100);
	HAL_QSPI_Transmit(hqspi, pData, Timeout);
    return HAL_OK;
}
