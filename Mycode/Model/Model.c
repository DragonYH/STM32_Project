#include "Model.h"
#include "stdlib.h"

/*
    初始化函数
调用格式：
DCDC_MODEL *Buck = initDCModel(   BUCK_TOPOLOGY,   // 电路拓扑结构    
                                  TIM1,            // 驱动PWM所在定时器
                                  TIM_CCR1,        // 驱动PWM所在定时器通道
                                  4200,            // 驱动PWM初始比较值
                                  2100,            // 驱动PWM比较值下限
                                  7200,            // 驱动PWM比较值上限
                                  1000,            // PID 采集电压逼近值
                                  1000);           // PID 采集电流逼近值 */

/**
  * @brief  初始化电路模型结构体
  * @param  DRIVE_TIM : 驱动PWM所在的定时器
  * @param  DRIVE_TIM_CHINNAL : 驱动PWM所在定时器的通道（见Model.h头文件中的宏定义）
  * @param  PWM_VALUE : PWM初始比较值
  * @param  PWMLOWERLIMIT :  PWM比较值的下限
  * @param  PWMUPPERLIMIT :  PWM比较值的上限
  * @param  PidVoltageSET : 目标输出电压所对应的ADC采样值
  * @param  pidCurrentSET : 目标输出电流所对应的ADC采样值
  * @retval DCDC_MODEL*
 */
DCDC_MODEL *initDCModel(uint8_t TOPOLOGY         ,
                        TIM_TypeDef* DRIVE_TIM   , 
                        uint8_t DRIVE_TIM_CHINNAL,
                        double   PWM_VALUE       ,
                        uint32_t PWMLOWERLIMIT   ,
                        uint32_t PWMUPPERLIMIT   ,
                        uint32_t PidVoltageSET   ,
                        uint32_t pidCurrentSET   )
{
    // 0. 为结构体指针分配内存空间
    DCDC_MODEL* Model = (DCDC_MODEL*)malloc(sizeof(DCDC_MODEL));

    Model->topology = BUCK_TOPOLOGY;
    // 1. myTIM
    #ifdef USING_TIM
        Model->myTIM = (timParameters*)malloc(sizeof(timParameters));
        Model->myTIM->pwmValue = PWM_VALUE;
        Model->setPwmValue = setPwmValue;
        Model->myTIM->driveTIM = DRIVE_TIM;
        Model->myTIM->driveTIM_CHINNAL = DRIVE_TIM_CHINNAL;
    #endif

    // 2. myADC
    #ifdef USING_ADC
        Model->myADC = (adcParameters*)malloc(sizeof(adcParameters));
        Model->myADC->outputCur = 0;
        Model->myADC->outputVol = 0;
        Model->dataSampling = dataSampling;
        Model->volLinearCal = volLinearCal;
        Model->curLinearCal = curLinearCal;
    #endif
    //3. myPID
    #ifdef USING_PID
        Model->myPID = (pidParameters*)malloc(sizeof(pidParameters));
        Model->myPID->pwmLowerLimit = PWMLOWERLIMIT;
        Model->myPID->pwmUpperLimit = PWMUPPERLIMIT;
        Model->myPID->pwmVoltageSet = PidVoltageSET;   // 根据 运放or传感器 的线性关系调整
        Model->myPID->pwmCurrentSet = pidCurrentSET;   // 根据 运放or传感器 的线性关系调整
        Model->pid_OutputVoltage = pid_OutputVoltage;
        Model->pid_OutputCurrent = pid_OutputCurrent;
    #endif

    //4. usart
    #ifdef USING_USART
        Model->printInformation = printInformation;
    #endif

    return Model;
}



// 初始化模块
#ifdef USING_TIM
    /**
      *  @brief  设置驱动PWM的比较值
      */
    void setPwmValue(DCDC_MODEL* Model)
    {
        switch (Model->myTIM->driveTIM_CHINNAL)
        {
            case TIM_CCR1:
                Model->myTIM->driveTIM->CCR1 = Model->myTIM->pwmValue;
              break;
            case TIM_CCR2:
                Model->myTIM->driveTIM->CCR2 = Model->myTIM->pwmValue;
              break;
            case TIM_CCR3:
                Model->myTIM->driveTIM->CCR3 = Model->myTIM->pwmValue;
              break;
            case TIM_CCR4:
                Model->myTIM->driveTIM->CCR4 = Model->myTIM->pwmValue;
              break;
            default:
              break;;
        }
    }
#endif

#ifdef USING_ADC
    /*
        快排函数，找出采样数组的中位值
    */
    void quick_sort(uint32_t q[], int l, int r)   
    {
      if (l >= r) return;
      int i = l - 1, j = r + 1, x = q[l];
      while (i < j)
      {
        do i ++ ; while (q[i] < x);
        do j -- ; while (q[j] > x);
        if (i < j) 
        {
          q[i] = q[i] ^ q[j];
          q[j] = q[i] ^ q[j];
          q[i] = q[i] ^ q[j];
        }
        else break;
      }
      quick_sort(q, l, j), quick_sort(q, j + 1, r);
    }
    /**
       @brief  中位值滤波采样函数（轮询）
       @param  ADC_CHANNEL ADC采样通道（参考Model.h头文件定义）
               通道可以自行根据 CubeMX 的配置修改
    */
    uint32_t ADC_Get_MedianFilter(uint8_t ADC_CHANNEL)
    {
      ADC_ChannelConfTypeDef sConfig;
      uint32_t value_sample[50] = {0};
      uint8_t i;
      switch(ADC_CHANNEL)
      {
        case PA0 : sConfig.Channel = ADC_CHANNEL_0; break;
        case PA1 : sConfig.Channel = ADC_CHANNEL_1; break;
        case PA2 : sConfig.Channel = ADC_CHANNEL_2; break;
        case PA3 : sConfig.Channel = ADC_CHANNEL_3; break;
      }
      sConfig.Rank = 1;
      HAL_ADC_ConfigChannel(&hadc1,&sConfig); 
      for(i = 0; i < 50; i ++)
      {
        HAL_ADC_Start(&hadc1);                      //* 启动转换
        HAL_ADC_PollForConversion(&hadc1,30);       //* 等待转化结束
        value_sample[i] = HAL_ADC_GetValue(&hadc1); //* 获取采样
        HAL_ADC_Stop(&hadc1);                       //* 停止转换
      }
      quick_sort(value_sample, 0, 49);
      return value_sample[25];
    }
    /*
        ADC采样函数
    */
    double dataSampling(DCDC_MODEL* Model, uint8_t ADC_CHANNEL)
    {
        double Data = (double)ADC_Get_MedianFilter(ADC_CHANNEL) * 3.3 / 4096.0;
        return Data;
    }
    /**
     *  @brief  线性计算，返回实际输出电压值
     *  @note   因为单片机的无法采样超过 3.3V 的电压，需要进行分压电路等比缩小
     *          该函数就是根据传感器的线性关系计算模块实际输出电压
     *  @retval 返回实际电压值
    */
    double volLinearCal(DCDC_MODEL* Model)     // 输出电压的线性计算
    {
        double x = Model->myADC->outputVol;
        return x * 10;
    }
    /**
     *  @brief  线性计算，返回实际输出电流值
     *  @note   因为单片机的无法采样直接采集电流，需要传感器将电流转换为电压
     *          该函数就是根据传感器的线性关系计算模块的实际输出电流
     *  @retval 返回实际电流值
    */
    double curLinearCal(DCDC_MODEL* Model)     // 输入电压的线性计算
    {
        double x = Model->myADC->outputCur;
        return x;
    }
#endif

#ifdef USING_USART
    /*
        重定向输出函数
    */
    int fputc(int ch, FILE *f)
    {
       uint8_t temp[1] = {ch};
       HAL_UART_Transmit(&huart1, temp, 1, 2);
       return ch;
    }
    /*
        重定向输入函数
    */
    int fgetc(FILE *f)
    {
      uint8_t ch = 0;
      HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
      return ch;
    }
    /*
        输出参数
    */
    void printInformation(double Data, char info[], char info2[])
    {
        printf("%s -- %.4f%s\n", info, Data, info2);
    }
#endif

#ifdef USING_PID
    void pid_OutputVoltage( DCDC_MODEL* Model   , 
                            double adcSampleSet , 
                            double coeErr       , 
                            double coeInteg     , 
                            double coeDeriva    ,
                            uint8_t TOPOLOGY   )
    {
        static double Desired, Actu;  
        static double Pre_Err = 0, Err = 0, Integ = 0, Deriva = 0, Duty = 0;
        Actu = Model->myADC->outputVol * 1000.0;
        Desired =  adcSampleSet; 
        Err = (TOPOLOGY == BUCK_TOPOLOGY) ? (Desired - Actu) : (Actu - Desired);
        Integ = Integ + Err;
        Deriva = Err - Pre_Err; 
        Duty = (Err*coeErr  + Integ*coeInteg + Deriva*coeDeriva);  
        Pre_Err = Err;
        Model->myTIM->pwmValue = Model->myTIM->pwmValue + Duty;
        if ( Model->myTIM->pwmValue < Model->myPID->pwmLowerLimit)
        {
            Model->myTIM->pwmValue = Model->myPID->pwmLowerLimit;
            Integ = 0;
            Deriva = 0;
        }
        if ( Model->myTIM->pwmValue > Model->myPID->pwmUpperLimit)
        {
            Model->myTIM->pwmValue = Model->myPID->pwmUpperLimit;
            Integ = 0;
            Deriva = 0;
        }
        Model->setPwmValue(Model);
    }

    void pid_OutputCurrent( DCDC_MODEL* Model   , 
                            double adcSampleSet , 
                            double coeErr       , 
                            double coeInteg     , 
                            double coeDeriva    ,   
                            uint8_t TOPOLOGY   )
    {
        static double Desired, Actu;  
        static double Pre_Err = 0, Err = 0, Integ = 0, Deriva = 0, Duty = 0;
        Actu = Model->myADC->outputCur * 1000.0;
        Desired =  adcSampleSet; 
        Err = (TOPOLOGY == BUCK_TOPOLOGY) ? (Desired - Actu) : (Actu - Desired);
        Integ = Integ + Err;
        Deriva = Err - Pre_Err; 
        Duty = (Err*coeErr  + Integ*coeInteg + Deriva*coeDeriva);  
        Pre_Err = Err;
        Model->myTIM->pwmValue = Model->myTIM->pwmValue + Duty;
        if ( Model->myTIM->pwmValue < Model->myPID->pwmLowerLimit)
        {
            Model->myTIM->pwmValue = Model->myPID->pwmLowerLimit;
            Integ = 0;
            Deriva = 0;
        }
        if ( Model->myTIM->pwmValue > Model->myPID->pwmUpperLimit)
        {
            Model->myTIM->pwmValue = Model->myPID->pwmUpperLimit;
            Integ = 0;
            Deriva = 0;
        }
        Model->setPwmValue(Model);
    }
#endif
