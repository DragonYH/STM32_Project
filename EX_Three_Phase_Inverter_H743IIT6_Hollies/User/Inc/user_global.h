#ifndef __USER_GLOBAL_H
#define __USER_GLOBAL_H

#include "ina228.h"
#include "three_phrase_pll.h"

extern const INA228_Handle INA228_0;
extern float U;
extern float I;

extern pll_Signal_V *signal_V;
extern pll_Signal_I *signal_I;

#endif
