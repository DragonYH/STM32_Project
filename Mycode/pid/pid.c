#include  "pid.h"

PID_TypeDef  PID_V;

PID_TypeDef  MPPT_V;

void PID_Init(void)    //PID参数初始化
  {

		PID_V.Kp = 20.0;     //5.5
		PID_V.Ki = 0.1;  //0.05
		PID_V.Kd = 0;     //0
		PID_V.Result_min = -100;
		PID_V.Result_max = 100;


		MPPT_V.Kp = 10.0;     //5.5
		MPPT_V.Ki = 0.1;  //0.05
		MPPT_V.Kd = 0;     //0
		MPPT_V.Result_min = -50;
		MPPT_V.Result_max = 50;

}


/*************************************************************************
函数功能：增量PID控制器
入口参数:PID名称，测量值，目标值
返回  值：计算结果
根据增量式离散PID公式
dac+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差
e(k-1)代表上一次的偏差  以此类推
Result代表增量输出
Dac+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
float Incremental_PID(PID_TypeDef *PID,float Measured,float Target)
{
	 PID->Bias = Target - Measured;                                                          //计算偏差
	 PID->Result += PID->Kp * (PID->Bias - PID->Now_bias)
	 + PID->Ki * PID->Bias + PID->Kd * (PID->Bias - 2 * PID->Now_bias + PID->Previous_bias);  //增量式PID控制器
	 PID->Previous_bias=PID->Now_bias;                                                       //保存上次偏差
	 PID->Now_bias=PID->Bias;	                                                               //保存本次偏差
	 if( PID->Result> PID->Result_max)       PID->Result= PID->Result_max;                   //输出限幅
	 else if( PID->Result< PID->Result_min)  PID->Result = PID->Result_min;
	 return PID->Result;
}


