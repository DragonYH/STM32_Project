#include "spwm.h"
const double Pi = 3.141592653589793238;
/**
 * @brief 用于存储单极性spwm波每个点对应值
 *
 */
int spwm_value_Unipolarity[COUNT] = {0};
/**
 * @brief 用于存储双极性spwm波每个点对应值
 *
 */
int spwm_value_Bipolar[COUNT] = {0};

/**
 * @brief 存储spwm相关数据
 * @param[in] f 载波比
 * @param[in] M 调制度
 * @param[in] HZ 当前基频
 * @param[in] pre_HZ 上一次循环基频
 * @param[in] i 循环计数
 * @param[in] turn 翻转标志
 */
spwm_data spwm = {
    1.0,
    1.0,
    50,
    50,
    0,
    0,
};

uint32_t HZ_saved[] = {0};

/**
 * @brief 生成spwm波每个点对应值
 */
void spwm_value_Init(void)
{
    for (int i = 0; i < COUNT; i++)
    {
        spwm_value_Unipolarity[i] = TIM_spwm_->ARR * sin(180.0 * (i + 1) / COUNT * Pi / 180);
    }
    for (int i = 0; i < COUNT; i++)
    {
        spwm_value_Bipolar[i] = TIM_spwm_->ARR * sin(360.0 * (i + 1) / COUNT * Pi / 180);
    }
}

/**
 * @brief spwm初始化
 */
void spwm_Init(void)
{
    //读取Flash数据
    uint32_t read[]= {0};
    FlashRead(ADDR_FLASH_SECTOR_7, read, sizeof(read)/sizeof(uint32_t));
    DWT_Delay_us(10000);
    spwm.HZ = read[0];
    spwm.pre_HZ = read[0];
    //修改定时器预分频
    TIM_spwm_->PSC = 0;
    TIM_pwm_->PSC = 100 - 1;
    //修改定时器周期
    TIM_spwm_->ARR = TIM_spwm_HZ / (COUNT * 2) / spwm.HZ - 1;
    TIM_pwm_->ARR = TIM_pwm_HZ / 100 / spwm.HZ - 1;
    //设置占空比
    TIM_spwm_->CCR1 = TIM_spwm_->ARR / 2;
    TIM_pwm_->CCR1 = TIM_pwm_->ARR / 2;
    //使能定时器
    HAL_TIM_PWM_Start_IT(&TIM_spwm, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_IT(&TIM_pwm, TIM_CHANNEL_1);

    //生成spwm波表
    spwm_value_Init();
}
/**
 * @brief 单极性spwm波：在中断中引用，填写为触发中断的定时器
 * @param[in] htim 用于计数的定时器
 */
void spwm_Unipolarity(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM_spwm.Instance)
    {
        if (++spwm.i == COUNT)    
        {
            spwm.i = 0;
            if (spwm.turn == 0)
            {
                spwm.turn = 1;
                TIM_spwm_->CCER = 0x00000001;
            }
            else
            {
                spwm.turn = 0;
                TIM_spwm_->CCER = 0x00000003;
            }
        }
        TIM_spwm_->CCR1 = spwm.M * spwm_value_Unipolarity[spwm.i]; //由向量表修改占空比
    }
    change_HZ();
}
/**
 * @brief 双极性spwm波：在中断中引用，填写为触发中断的定时器（未完善）
 * @param[in] htim 用于计数的定时器
 */
void spwm_Bipolar(TIM_HandleTypeDef *htim)
{
    static int i = 0;
    if (htim->Instance == TIM_spwm.Instance)
    {
        if (++i == COUNT)
        i = 0;
        TIM_spwm_->CCR1 = spwm.f * spwm.M * spwm_value_Unipolarity[i]; //由向量表修改占空比
    }
    change_HZ();
}
/**
 * @brief 修改频率
 *
 */
void change_HZ(void)
{
    if (spwm.pre_HZ != spwm.HZ)
    {
        //关闭中断
        HAL_TIM_PWM_Stop_IT(&TIM_spwm, TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop_IT(&TIM_pwm, TIM_CHANNEL_1);
        //重新给spwm波和pwm波赋值
        TIM_spwm_->ARR = TIM_spwm_HZ / (COUNT * 2) / spwm.HZ - 1;
        TIM_pwm_->ARR = TIM_pwm_HZ / 100 / spwm.HZ - 1;
        TIM_spwm_->CCR1 = TIM_spwm_->ARR / 2;
        TIM_pwm_->CCR1 = TIM_pwm_->ARR / 2;
        //重新生成sin表
        spwm_value_Init();
        //更新频率记录
        spwm.pre_HZ = spwm.HZ;
        //置零
        spwm.i = 0;
        spwm.turn = 0;
        //重置定时器计数值
        TIM_spwm_->CNT = 0;
        TIM_pwm_->CNT = 0;
        //重启中断
        HAL_TIM_PWM_Start_IT(&TIM_spwm, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start_IT(&TIM_pwm, TIM_CHANNEL_1);
        //写入改变的频率
        uint32_t write [] = {spwm.HZ};
        FlashErase(ADDR_FLASH_SECTOR_7);
        FlashWrite(ADDR_FLASH_SECTOR_7, write, sizeof(write)/sizeof(uint32_t));
        //重启
        HAL_NVIC_SystemReset();
    }
}

