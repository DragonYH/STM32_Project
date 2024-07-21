/*
 * @Author       : DragonYH 1016633827@qq.com
 * @Date         : 2024-07-20 18:37:44
 * @LastEditors  : DragonYH 1016633827@qq.com
 * @LastEditTime : 2024-07-21 19:35:48
 * @FilePath     : \EX_Single_Phase_Rectifier_H743IIT6_Hollies\User\Src\user_task.c
 * @Description  : 用于FreeRTOS任务的实现
 *
 * Copyright (c) 2024 by DragonYH, All Rights Reserved.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "user_exit.h"
#include "user_task.h"
#include "ina228.h"
#include "oled.h"
#include "ad7606.h"
#include "adc.h"
#include "tim.h"
#include "dac.h"
#include <stdio.h>

enum state deviceState = START;     /* 设备状态 */
static float mcuTemperature = 0.0f; /* MCU温度 */
static float Udc = 0.0f;            /* 直流电压 */
static float Idc = 0.0f;            /* 直流电流 */
static float Utarget = 40.0f;       /* 目标直流电压 */
float Itarget = 0.0f;               /* 电流参考值 */

void UserInit(void)
{
    ad7606_Init();
    OLED_Init();
    pll_Init_V(&signal_V, 50, 20000);
    pll_Init_I(&signal_I, 50, 20000);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    ad7606_Start(&htim2, TIM_CHANNEL_1);
}

/**
 * @brief    状态指示灯控制
 * @param    *argument
 * @return
 */
void StartStateLED(void *argument)
{
    /* USER CODE BEGIN stateLED */
    /* Infinite loop */
    for (;;)
    {
        switch (deviceState)
        {
        case START: /* 启动状态 */
            HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
            break;
        case RUN: /* 正常运行 */
            HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
            osDelay(100);
            HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
            osDelay(200);
            HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
            osDelay(100);
            HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
            osDelay(200);
            HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
            osDelay(1000);
            break;
        case FAULT: /* 保护状态 */
            HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_0);
            osDelay(100);
            break;
        default:
            break;
        }
    }
    /* USER CODE END stateLED */
}

/**
 * @brief    芯片温度读取
 * @param    *argument
 * @return
 */
void StartMcuTemperature(void *argument)
{
    /* USER CODE BEGIN mcuTemperature */
    HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED, ADC_CALIB_OFFSET);
    HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED, ADC_CALIB_OFFSET_LINEARITY);
    // /* Infinite loop */
    for (;;)
    {
        HAL_ADC_Start(&hadc3);
        if (HAL_ADC_PollForConversion(&hadc3, 1000) == HAL_OK) /* 判断是否转换完成 */
        {
            uint16_t temprature = HAL_ADC_GetValue(&hadc3); /* 读出转换结果 */
            mcuTemperature = ((110.0 - 30.0) / (*(unsigned short *)(0x1FF1E840) - *(unsigned short *)(0x1FF1E820))) * (temprature - *(unsigned short *)(0x1FF1E820)) + 30;
        }
        osDelay(500);
    }
    /* USER CODE END mcuTemperature */
}

/**
 * @brief    OLED显示控制
 * @param    *argument
 * @return
 */
void StartOledDisplay(void *argument)
{
    /* USER CODE BEGIN oledDisplay */
    /* Infinite loop */
    for (;;)
    {
        char oledBuffer[22] = {0};
        float Uac = signal_V->basic->rms;
        float Iac = signal_I->basic->rms;

        /* 显示直流电压电流 */
        snprintf(oledBuffer, sizeof(oledBuffer), "Udc:%5.2fV Idc:%4.2fA", Udc, Idc);
        OLED_ShowString(0, 0, (uint8_t *)oledBuffer, 12);

        /* 显示交流电压电流 */
        snprintf(oledBuffer, sizeof(oledBuffer), "Uac:%5.2fV Iac:%4.2fA", Uac, Iac);
        OLED_ShowString(0, 12, (uint8_t *)oledBuffer, 12);

        /* 显示设备状态和温度 */
        const char *stateText;
        switch (deviceState)
        {
        case START:
            stateText = "START";
            break;
        case RUN:
            stateText = "RUN";
            break;
        case FAULT:
            stateText = "FAULT";
            break;
        default:
            stateText = "UNKNOWN";
            break;
        }
        snprintf(oledBuffer, sizeof(oledBuffer), "%s     T:%5.2fC", stateText, mcuTemperature);
        OLED_ShowString(0, 48, (uint8_t *)oledBuffer, 12);

        /* 刷新显示 */
        OLED_Refresh();
        osDelay(100);
    }
    /* USER CODE END oledDisplay */
}

/**
 * @brief    直流采样
 * @param    *argument
 * @return
 */
void StartDCSampling(void *argument)
{
    /* USER CODE BEGIN dcSampling */
    /* INA228 配置 */
    static INA228_State INA228_0_state = {
        /* 配置和设置 */
        .configRegister = (INA228_config_register_rst_NormalOperation |
                           INA228_config_register_rstacc_NormalOperation |
                           0x0000U | /* 以 2ms 的步长设置初始 ADC 转换的延迟 */
                           INA228_config_register_tempcomp_Shunttemperaturecompensationdisabled |
                           INA228_config_register_adcrange_16384mV),
        .adcConfigRegister = (INA228_adc_config_register_mode_Continuousbusvoltageshuntvoltageandtemperature |
                              INA228_adc_config_register_vbusct_150us |
                              INA228_adc_config_register_vshct_150us |
                              INA228_adc_config_register_vtct_150us |
                              INA228_adc_config_register_avg_64),
        /* shuntCalRegister = 13107.2 * 1e6 * 预计最大电流 / 524288 * 采样电阻 */
        .shuntCalRegister = (uint16_t)(13107.2f * 1e6 * 5.f / 524288.f * 0.02f),
        .shuntTempcoRegister = 0x0000U, /* TEMPCO is 0 ppm/°C */
        .diagAlrtRegister = (INA228_diag_alrt_register_alatch_Transparent |
                             INA228_diag_alrt_register_cnvr_DisableconversionreadyflagonALERTpin |
                             INA228_diag_alrt_register_slowalert_ALERTcomparisononnonaveragedADCvalue |
                             INA228_diag_alrt_register_apol_Normalactivelowopendrain),
        .sovlRegister = 0x7FFFU,
        .suvlRegister = 0x8000U,
        .bovlRegister = 0x7FFFU,
        .buvlRegister = 0x0000U,
        .tempLimitRegister = 0x7FFFU,
        .pwrLimitRegister = 0xFFFFU,
        .adcrange = INA228_config_register_adcrange_16384mV,
        /* currentlsb = 预计最大电流 / 524288 */
        .currentlsb = 5.f / 524288.f,
        /* 传感器的I2C总线句柄和地址 */
        .hi2c = &hi2c1,
        .devAddr = 0x80U,
    };

    static const INA228_Handle INA228_0 = &INA228_0_state;

    /* Infinite loop */
    for (;;)
    {
        Udc = INA228_getVBUS_V(INA228_0);
        Idc = INA228_getCURRENT_A(INA228_0);
        osDelay(10);
    }
    /* USER CODE END dcSampling */
}

/**
 * @brief    电路保护控制
 * @param    *argument
 * @return
 */
void StartCircuitProtection(void *argument)
{
    /* USER CODE BEGIN circuitProtection */
    /* Infinite loop */
    for (;;)
    {
        float Uac = signal_V->basic->rms;
        float Iac = signal_I->basic->rms;
        if (Udc > protection_Udc || fabsf(Idc) > protection_Idc || Uac > protection_Uac || Iac > protection_Iac)
        {
            deviceState = FAULT;

            HAL_GPIO_WritePin(IR2104_SD_GPIO_Port, IR2104_SD_Pin, GPIO_PIN_RESET); /* 关闭输出 */
        }
        else if (deviceState == START && Uac > 10.f && Iac > 0.5f && fabsf(signal_V->basic->park_q) < 0.02f)
        {
            osDelay(500); /* 防止误判 */
            if (deviceState == START && Uac > 10.f && Iac > 0.5f && fabsf(signal_V->basic->park_q) < 0.02f)
            {
                deviceState = RUN;

                HAL_GPIO_WritePin(IR2104_SD_GPIO_Port, IR2104_SD_Pin, GPIO_PIN_SET); /* 打开输出 */
            }
        }
        osDelay(10);
    }
    /* USER CODE END circuitProtection */
}

/**
 * @brief    直流电压控制
 * @param    *argument
 * @return
 */
void StartDCControl(void *argument)
{
    /* USER CODE BEGIN dcControl */
    static PID pidDCV;
    pid_Init(&pidDCV, 0.1f, 0.01f, 0, 2.5f, 0.5f);
    /* Infinite loop */
    for (;;)
    {
        pid(&pidDCV, Utarget, Udc);
        Itarget = pidDCV.out;
        osDelay(20);
    }
    /* USER CODE END dcControl */
}
