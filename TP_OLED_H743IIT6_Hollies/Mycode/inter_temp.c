#include "inter_temp.h"

float getInterTemprature()
{
	uint16_t TS_CAL1 = *(__IO uint16_t *)(0x1FF1E820);
	uint16_t TS_CAL2 = *(__IO uint16_t *)(0x1FF1E840);
	HAL_ADC_Start(&hadc3);
	if(HAL_ADC_PollForConversion(&hadc3, 10)==HAL_OK)
		return ((110.0f - 30.0f) / (TS_CAL2 - TS_CAL1)) * (HAL_ADC_GetValue(&hadc3) - TS_CAL1) + 30.0f;
	return HAL_TIMEOUT;
}
