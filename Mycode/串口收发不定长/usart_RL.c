#include "usart_RL.h"
/**
 * @brief 将 cmd 指令发送到指定的串口
 * 
 * @param cmd 字符串："1234141" 
 * @param huart 将接收到的数据发送此串口
 */
void UART_SendMessage(char * cmd, UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit(huart, (uint8_t *)cmd,strlen(cmd),0xFFFF); 
    while(HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX);  //检测UART发送结束
}



/**
 * @brief 将数据包发送到指定串口上
 * @param myUsartData 存储数据的额构体
 * @param huart 将接收到的数据发送此串口
 */
void UART_ShowMessage(struct UsartData* myUsartData, UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit(huart, (uint8_t *)myUsartData->aRxBuffer , myUsartData->Uart_Rx_Cn,0xFFFF); //将收到的信息发送出去
    while(HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX);//检测UART发送结束
    myUsartData->Uart_Rx_Cn = 0;
    memset(myUsartData->RxBuffer, 0x00, sizeof(myUsartData->RxBuffer)); //清空数组
}


/**
 * @brief 只在中断中接收数据，不进行发送
 *        此函数在接收字符超过数据范围时不会进行任何提示，可以自行修改
 * @param myUsartData 接收数据的结构体
 */

void UART_MyRxCpltCallback_OnlyReceive_Message(struct UsartData* myUsartData)
{
    myUsartData->RxBuffer[myUsartData->Uart_Rx_Cn ++] = myUsartData->aRxBuffer;
    if(myUsartData->Uart_Rx_Cn >= RXBUFFERSIZE) // 假如超出数据范围
    {
        myUsartData->Uart_Rx_Cn = 0;
        // uint8_t errInfo[]="Data Overflow\r\n";
        // while(HAL_UART_Transmit_IT(huart,errInfo,18) == HAL_OK);
    }
      // while(HAL_UART_Receive_IT(huart, (uint8_t *)&(myUsartData->aRxBuffer), 1) == HAL_OK);
}


/**
 * @brief 在中断中接收数据，并发送到指定的串口中
 * @param myUsartData 接收数据的结构体
 * @param huart 将接收到的数据发送此串口
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
      if(myUsartData->Uart_Rx_Cn >= RXBUFFERSIZE) // 假如超出数据范围
      {
          myUsartData->Uart_Rx_Cn = 0;
          uint8_t errInfo[]="too many data!!!\r\n";
          while(HAL_UART_Transmit_IT(huart,errInfo,18) == HAL_OK);
      }
      while(HAL_UART_Receive_IT(huart, (uint8_t *)&(myUsartData->aRxBuffer), 1) == HAL_OK);
      // 注意在中断中重启启动 HAL_UART_Receive_IT，开启中断
}
