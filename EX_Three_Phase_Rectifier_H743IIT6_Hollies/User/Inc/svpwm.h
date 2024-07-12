#ifndef __SVPWM_H
#define __SVPWM_H

#include "three_phrase_pll.h"

#define TIM_PERIOD 5999

void svpwm_Control(pll_Signal_V *signal, float Udc);

#endif
