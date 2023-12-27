#include "usart.h"
#include "string.h"
#define RXBUFFERSIZE  4096         //最大接收字节数
typedef struct UsartData
{
  uint8_t RxBuffer[RXBUFFERSIZE];  //接收数据
  uint8_t aRxBuffer;            //接收中断缓冲
  uint32_t Uart_Rx_Cn;          //接收缓冲计数

}UsartData;

void UART_SendMessage(char * cmd, UART_HandleTypeDef *huart);
void UART_ShowMessage(struct UsartData* Usart, UART_HandleTypeDef *huart);
void UART_MyRxCpltCallback_OnlyReceive_Message(struct UsartData* myUsartData);
void UART_MyRxCpltCallback_Receive_Send_Messasge(struct UsartData* myUsartData, UART_HandleTypeDef *huart);
