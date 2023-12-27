//存储信号内部过程量
struct signal_PLL_Temp
{
    float lamda;
    float x;
    float y;
    float b0;
    float a1;
    float a2;
};
typedef struct signal_PLL
{
    float omiga;    //无阻尼自然频率，2*pi*频率
    float Ts;   //采样周期
    float d_0, d_1, d_2;
    float q_0, q_1, q_2;
    float u_0, u_1, u_2;
    struct signal_PLL_Temp temp;
}signal_PLL;

signal_PLL signal_PLL_Init(signal_PLL pll);