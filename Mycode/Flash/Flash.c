#include "Flash.h"
#include <stdio.h>


/**
 * @brief Flash擦除函数
 * 
 * @param StartAddress 擦除目标地址 
 */
uint32_t FlashErase(uint32_t StartAddress)
{  	
    int sect = 0;
    HAL_FLASH_Unlock();//解锁
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);//清除一些错误标志
                          
    sect = GetSector(StartAddress);//获取地址所在的扇区
    FLASH_Erase_Sector(sect, FLASH_VOLTAGE_RANGE_3);//擦除指定的闪存扇区(0~7)
    printf("Erase Success!\r\n");
    HAL_FLASH_Lock();//上锁
    return HAL_OK;
}


/**
 * @brief Flash写函数
 * 
 * @param StartAddress 起始地址 
 * @param data 数据指针
 * @param dataSize 数据大小
 */
uint32_t FlashWrite(uint32_t StartAddress, uint32_t *data, uint32_t dataSize)
{
	//可以添加一些 StartAddress地址 是否有效的判断
  HAL_FLASH_Unlock();//解锁
  for (int i = 0; i < dataSize; i ++)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(StartAddress + i * sizeof(uint32_t)), *(uint32_t *)(data + i)) == HAL_OK)
    // 每次擦除后，地址向后偏移4个字节
    {
      printf("Write Data: %d Success!\r\n", *(data + i));
    }
    else
    {
      printf("Write Data: %d Fail!\r\n", *(data + i));
      HAL_FLASH_Lock();//上锁
      return HAL_ERROR;
    }
  }
  // printf("Write All Data Success!\r\n");
  HAL_FLASH_Lock();//上锁
  return HAL_OK;
}
 


/**
 * @brief Flash写函数
 * 
 * @param StartAddress 写目标地址 
 * @param data 指向拷贝数组的指针
 * @param dataSize 数据大小
 */
uint32_t FlashRead(uint32_t StartAddress, uint32_t *copyData, uint32_t dataSize)
{
  for (int i = 0; i < dataSize; i ++)
  {
    *(copyData + i) = *(uint32_t *)(StartAddress + i * sizeof(uint32_t));
  }
  return HAL_OK;
}

/**
 * @brief Get the Sector object
 * 
 * @param Address 擦写目标扇区起始地址
 * @return uint32_t 返回目标地址所在扇区
 */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else
  {
    sector = FLASH_SECTOR_11;  
  }
 
  return sector;
}




