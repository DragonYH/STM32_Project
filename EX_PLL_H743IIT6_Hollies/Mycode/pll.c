#include "pll.h"
#include "arm_math.h"
#include "sogi.h"
#include "malloc.h"

pll_Signal *signal_1;
pll_Config *signal_cofig_1;

// 时钟，pwm通道，
void pll_Control(pll_Signal *signal)
{
    sogi(signal);
    // arm_park_f32(Ialpha, Ibeta, pId, pIq, sinVal, cosVal);
}
/**
 * @brief 信号参数初始化
 * @param signal: 信号指针
 * @param config: 配置指针
 * @param f: 信号频率(典型值:50)
 * @param F: 采样频率(典型值:20000)
 * @return
 */
void pll_Init(pll_Signal *signal, pll_Config *config, float f, uint16_t F)
{
	// 分配内存空间
	signal = (pll_Signal*)malloc(sizeof(pll_Signal));
	config = (pll_Config*)malloc(sizeof(pll_Config));
	// 初始化赋值
    signal->d_1 = 0.f;
    signal->d_2 = 0.f;
    signal->q_1 = 0.f;
    signal->q_2 = 0.f;

    config->omiga = 2 * 3.14f * f; // f典型值50
    config->Ts = 1.f / F;          // F典型值20000
    config->phase = 0;
    // 计算中间量
    signal->lamda = 0.5f * config->omiga * config->Ts;
    signal->x = 2 * config->k * config->omiga * config->Ts;
    signal->y = config->omiga * config->Ts * config->omiga * config->Ts;

    signal->b0 = signal->x / (signal->x + signal->y + 4);
    signal->a1 = (8 - 2 * signal->y) / (signal->x + signal->y + 4);
    signal->a2 = (signal->x - signal->y - 4) / (signal->x + signal->y + 4);
}
/**
 * @brief 回收空间
 * @param signal: 信号指针
 * @param config: 配置指针
 * @return
 */
void pll_Clear(pll_Signal *signal, pll_Config *config)
{
	free(signal);
	free(config);
}
