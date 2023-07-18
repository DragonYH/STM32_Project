#include "main.h"
/*定义结构体和公用体*/
typedef struct
{
    float target;		/*设定值*/
    float kp;			/*比例系数*/
    float ki;			/*积分系数*/
    float kd;			/*微分系数*/
    float lastError;	/*前一拍偏差*/
    float preError;		/*前两拍偏差*/
    float deadBand;		/*死区*/
    float result;       /*PID控制器计算结果*/
    float maximum;		/*输出值上限, 0-1.0*/
    float minimum;		/*输出值下限, 0-1.0*/
    float errorAbsMax;	/*偏差绝对值最大值*/
    float errorAbsMin;	/*偏差绝对值最小值*/
    float alpha;		/*不完全微分系数, 0-1.0, 0为不生效, 1为无微分项*/
    float deltaDiff;	/*微分增量*/
    float timCount;		/*控制时钟的计数值*/
} PID;

void PID_init(PID* pid, TIM_HandleTypeDef* htim, float kp, float ki, float kd, float target);
float PID_regulator(PID* PID, float messure);
static float VariableIntegralCoefficient(float error, float absMax, float absMin);
