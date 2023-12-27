#include <protect_switch.h>

int V_flag=0,I_flag=0;

/**
* @brief 过压保护
* @param[in] V_threshold 过压阈值
* @param[in] V_samping 电压采样值
*/
void V_protect(int V_threshold,int V_samping) //过压保护：过压阈值，电压采样值
{
    if (V_samping > V_threshold)
    {
        HAL_GPIO_WritePin(P_protect_GPIO_Port, P_protect_Pin, GPIO_PIN_RESET);
				V_flag=1;
    }
		else if(V_flag==0)
		{
				HAL_GPIO_WritePin(P_protect_GPIO_Port, P_protect_Pin, GPIO_PIN_SET);
		}
}

/**
* @brief 过流保护
* @param[in] I_threshold 过流阈值
* @param[in] I_samping 电流采样值
*/
void I_protect(int I_threshold,int I_samping) //过流保护：过流阈值，电流采样值
{
    if (I_samping > I_threshold)
    {
        HAL_GPIO_WritePin(P_protect_GPIO_Port, P_protect_Pin, GPIO_PIN_RESET);
				I_flag=1;
    }
		else if(I_flag==0)
		{
			HAL_GPIO_WritePin(P_protect_GPIO_Port, P_protect_Pin, GPIO_PIN_SET);
		}
}

/**
* @brief 开关切换
* @param[in] V_threshold 切换电压阈值
* @param[in] V_samping 电压采样值
*/
void P_switch(int V_threshold,int V_samping) //开关切换：切换电压阈值，电压采样值
{
    if (V_samping > V_threshold)
    {
        HAL_GPIO_WritePin(P_switch_GPIO_Port, P_switch_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(P_switch_GPIO_Port, P_switch_Pin, GPIO_PIN_SET);
    }
}

