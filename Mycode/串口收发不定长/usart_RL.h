#include "usart.h"
#include "string.h"
#define RXBUFFERSIZE  4096         //�������ֽ���
typedef struct UsartData
{
  uint8_t RxBuffer[RXBUFFERSIZE];  //��������
  uint8_t aRxBuffer;            //�����жϻ���
  uint32_t Uart_Rx_Cn;          //���ջ������

}UsartData;

void UART_SendMessage(char * cmd, UART_HandleTypeDef *huart);
void UART_ShowMessage(struct UsartData* Usart, UART_HandleTypeDef *huart);
void UART_MyRxCpltCallback_OnlyReceive_Message(struct UsartData* myUsartData);
void UART_MyRxCpltCallback_Receive_Send_Messasge(struct UsartData* myUsartData, UART_HandleTypeDef *huart);
