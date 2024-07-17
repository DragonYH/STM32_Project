#ifndef __FIR_H
#define __FIR_H

#include "arm_math.h"

extern const float firCoeffs_100Hz[21];

void firFilterInit(arm_fir_instance_f32 **S, const float *firCoeffs, float *firState);

#endif
