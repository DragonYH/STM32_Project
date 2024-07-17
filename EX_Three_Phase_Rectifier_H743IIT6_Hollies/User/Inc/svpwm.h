#ifndef __SVPWM_H
#define __SVPWM_H

#include "three_phrase_pll.h"
#include "user_global.h"

#define TIM_PERIOD 5999

#if Rectifier_Or_Inverter
void svpwm_Control(pll_Signal_I *signal);
#else
void svpwm_Control(pll_Signal_V *signal);
#endif

#endif
