#include "usart_RL.h"
/**
 * @brief �� cmd ָ��͵�ָ���Ĵ���
 * 
 * @param cmd �ַ�����"1234141" 
 * @param huart �����յ������ݷ��ʹ˴���
 */
void UART_SendMessage(char * cmd, UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit(huart, (uint8_t *)cmd,strlen(cmd),0xFFFF); 
    while(HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX);  //���UART���ͽ���
}



/**
 * @brief �����ݰ����͵�ָ��������
 * @param myUsartData �洢���ݵĶ��
 * @param huart �����յ������ݷ��ʹ˴���
 */
void UART_ShowMessage(struct UsartData* myUsartData, UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit(huart, (uint8_t *)myUsartData->aRxBuffer , myUsartData->Uart_Rx_Cn,0xFFFF); //���յ�����Ϣ���ͳ�ȥ
    while(HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX);//���UART���ͽ���
    myUsartData->Uart_Rx_Cn = 0;
    memset(myUsartData->RxBuffer, 0x00, sizeof(myUsartData->RxBuffer)); //�������
}


/**
 * @brief ֻ���ж��н������ݣ������з���
 *        �˺����ڽ����ַ��������ݷ�Χʱ��������κ���ʾ�����������޸�
 * @param myUsartData �������ݵĽṹ��
 */

void UART_MyRxCpltCallback_OnlyReceive_Message(struct UsartData* myUsartData)
{
    myUsartData->RxBuffer[myUsartData->Uart_Rx_Cn ++] = myUsartData->aRxBuffer;
    if(myUsartData->Uart_Rx_Cn >= RXBUFFERSIZE) // ���糬�����ݷ�Χ
    {
        myUsartData->Uart_Rx_Cn = 0;
        // uint8_t errInfo[]="Data Overflow\r\n";
        // while(HAL_UART_Transmit_IT(huart,errInfo,18) == HAL_OK);
    }
      // while(HAL_UART_Receive_IT(huart, (uint8_t *)&(myUsartData->aRxBuffer), 1) == HAL_OK);
}


/**
 * @brief ���ж��н������ݣ������͵�ָ���Ĵ�����
 * @param myUsartData �������ݵĽṹ��
 * @param huart �����յ������ݷ��ʹ˴���
 */
void UART_MyRxCpltCallback_Receive_Send_Messasge(struct UsartData* myUsartData, UART_HandleTypeDef *huart)
{
    // UART_MyRxCpltCallback_ReceiveMessage(&myUsart1);
      myUsartData->RxBuffer[myUsartData->Uart_Rx_Cn ++] = myUsartData->aRxBuffer;
      if(myUsartData->aRxBuffer == 0x0A)
      {
          while(HAL_UART_Transmit_IT(huart, myUsartData->RxBuffer, myUsartData->Uart_Rx_Cn)==HAL_OK);
          myUsartData->Uart_Rx_Cn = 0;
      }
      if(myUsartData->Uart_Rx_Cn >= RXBUFFERSIZE) // ���糬�����ݷ�Χ
      {
          myUsartData->Uart_Rx_Cn = 0;
          uint8_t errInfo[]="too many data!!!\r\n";
          while(HAL_UART_Transmit_IT(huart,errInfo,18) == HAL_OK);
      }
      while(HAL_UART_Receive_IT(huart, (uint8_t *)&(myUsartData->aRxBuffer), 1) == HAL_OK);
      // ע�����ж����������� HAL_UART_Receive_IT�������ж�
}
