#ifndef _FLASH_H_
#define _FLASH_H_
#include "main.h"
#include "stm32f4xx_hal_flash_ex.h" 
 
 
#define STM32_FLASH_SIZE 	1024 	 	//所选STM32的FLASH容量大小(单位为K)
    #if STM32_FLASH_SIZE <  256         //设置扇区大小
        #define STM_SECTOR_SIZE     128*1024    //1K字节
    #else 
        #define STM_SECTOR_SIZE	    1024*128    //128K字节
#endif
 
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */
// #define STM32F4ZGT6_FLASh_SECTION_6_SIZE    0x0804 0000
#define FLASH_USER_START_ADDR   ( ADDR_FLASH_SECTOR_10 )  //写Flash的地址，这里从第10个扇区开始
#define FLASH_USER_END_ADDR     ( ADDR_FLASH_SECTOR_11 )  //写Flash的地址，这里以第11个扇区结束
 
 
uint32_t FlashErase(uint32_t StartAddress);
uint32_t FlashWrite(uint32_t StartAddress, uint32_t *data, uint32_t dataSize);
uint32_t FlashRead(uint32_t StartAddress, uint32_t *copyData, uint32_t dataSize);
static uint32_t GetSector(uint32_t Address);
 
#endif
