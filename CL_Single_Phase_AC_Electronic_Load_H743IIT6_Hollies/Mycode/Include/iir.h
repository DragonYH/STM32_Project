#ifndef __IIR_H
#define __IIR_H

#define iirNumStages 2 // 2阶IIR滤波器个数
#define iirBlockSize 1 // 每次处理一个数据

extern const float iirCoeffs_5Hz[5 * iirNumStages];
extern const float iirScale_5Hz;
extern const float iirCoeffs_20Hz[5 * iirNumStages];
extern const float iirScale_20Hz;
#endif
