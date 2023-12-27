#ifndef MODEL_H
#define MODEL_H
#include "stdio.h"
#include "gpio.h"
#include "main.h"


// 点灯宏定义
#define LED0_ON  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
#define LED0_OFF HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
#define LED1_ON  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);
#define LED1_OFF HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);

#define BUCK_TOPOLOGY  0
#define BOOST_TOPOLOGY 1


#define USING_TIM
#define USING_ADC
#define USING_USART
#define USING_PID

#ifdef USING_TIM
    #include "tim.h"
    #define TIM_CCR1 1
    #define TIM_CCR2 2
    #define TIM_CCR3 3
    #define TIM_CCR4 4
typedef struct timParameters
    {
        double pwmValue;
        TIM_TypeDef* driveTIM;      // 驱动PWM所在的定时器
        uint8_t driveTIM_CHINNAL;
    }timParameters;
#endif


#ifdef USING_ADC
#include "adc.h"
    #define PA0 0
    #define PA1 1
    #define PA2 2
    #define PA3 3
    typedef struct adcParameters
    {
        double outputVol;
        double outputCur;
    }adcParameters;
#endif


#ifdef USING_USART
    #include "usart.h"
#endif

//*-------------------------------------
#ifdef USING_PID
    typedef struct pidParameters
    {
            // pwmValue的上下限
            int pwmLowerLimit;
            int pwmUpperLimit;
            // 下面两个是指ADC采集到的电压值，pid会自动逼近
            int pwmVoltageSet;
            int pwmCurrentSet;
    }pidParameters;
#endif


typedef struct DCDC_MODEL
{
    int topology;
    #ifdef USING_TIM
        timParameters *myTIM;
        void (*setPwmValue)(struct DCDC_MODEL* Model);
    #endif

    #ifdef USING_ADC
        adcParameters *myADC;
        double (*dataSampling)(struct DCDC_MODEL* Model, uint8_t ADC_CHANNEL);
        double (*volLinearCal)(struct DCDC_MODEL* Model);     
        double (*curLinearCal)(struct DCDC_MODEL* Model);     
    #endif

    #ifdef USING_USART
        void (*printInformation)(double Data, char info[], char info2[]);
    #endif

    #ifdef USING_PID
        pidParameters *myPID;
        void (*pid_OutputVoltage)(struct DCDC_MODEL* Model, 
                                     double adcSampleSet  , 
                                     double coeErr        , 
                                     double coeInteg      , 
                                     double coeDeriva     ,
                                     uint8_t TOPOLOGY    );
        void (*pid_OutputCurrent)(struct DCDC_MODEL* Model,     
                                     double adcSampleSet  , 
                                     double coeErr        , 
                                     double coeInteg      , 
                                     double coeDeriva     ,
                                     uint8_t TOPOLOGY    ); 
    #endif
} DCDC_MODEL;


#ifdef USING_TIM
    void setPwmValue(DCDC_MODEL* Model);
#endif

#ifdef USING_ADC
    void quick_sort(uint32_t q[], int l, int r);
    uint32_t ADC_Get_MedianFilter(uint8_t ch);
    double dataSampling(DCDC_MODEL* Model, uint8_t ADC_CHANNEL);
    double volLinearCal(DCDC_MODEL* Model);     // 输出电压的线性计算
    double curLinearCal(DCDC_MODEL* Model);     // 输入电压的线性计算
#endif

#ifdef USING_USART
    void printInformation(double Data, char info[], char info2[]);
#endif

#ifdef USING_PID
    void pid_OutputVoltage( DCDC_MODEL* Model   , 
                            double adcSampleSet , 
                            double coeErr       , 
                            double coeInteg     , 
                            double coeDeriva    ,
                            uint8_t TOPOLOGY   );

    void pid_OutputCurrent( DCDC_MODEL* Model   , 
                            double adcSampleSet , 
                            double coeErr       , 
                            double coeInteg     , 
                            double coeDeriva    ,
                            uint8_t TOPOLOGY   );
#endif /* USING_PID */


DCDC_MODEL *initDCModel(uint8_t TOPOLOGY         ,
                        TIM_TypeDef* DRIVE_TIM   , 
                        uint8_t DRIVE_TIM_CHINNAL,
                        double   PWM_VALUE       ,
                        uint32_t pwmLowerLimit   ,
                        uint32_t pwmUpperLimit   ,
                        uint32_t pwmVoltageSet   ,
                        uint32_t pwmCurrentSet   );
// Method 定义：获取当前电压电流值


#endif /* MODEL_H */
