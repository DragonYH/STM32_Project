/**
 * @file amoled.h
 * @brief 这个代码用于SH8601Z屏幕驱动
 * @author Hollies https://github.com/DragonYH
 * @date 2023年8月22日
 */
#include "main.h"
// 命令宏定义
#define RST                       0x01 // 软件重置
#define SLEEP_ON                  0x10 // 进入睡眠模式
#define SLEEP_OUT                 0x00 // 退出睡眠模式
#define MODE_PARTIAL              0x12 // 部分模式
#define MODE_NORMAL               0x13 // 正常模式
#define INV_OFF                   0x20 // 关闭反显
#define INV_ON                    0x21 // 打开反显
#define ALL_PIXELS_OFF            0x22 // 关闭所有像素
#define ALL_PIXELS_ON             0x23 // 打开所有像素
#define DISPLAY_OFF               0x28 // 关闭显示
#define DISPLAY_ON                0x29 // 打开显示
#define ROW                       0x2B // 设置行地址
#define COL                       0x2A // 设置列地址
#define MEMORY_DATA_WRITE         0x2C // 写入内存
#define PARTIAL_ROW               0x30 // 设置部分行
#define PARTIAL_COL               0x31 // 设置部分列
#define TE_OFF                    0x34 // 关闭抗撕裂模式
#define TE_ON                     0x35 // 打开抗撕裂模式
#define MEMORY_DATA_CONTROL       0x36 // 设置内存数据控制
#define IDLE_OFF                  0x38 // 关闭空闲模式
#define IDLE_ON                   0x39 // 打开空闲模式
#define PIXEL_FORMAT              0x3A // 设置像素格式
#define MEMORY_WRITE_CONTINUE     0x3C // 连续写入内存
#define TE_SCAN_LINE_WRITE        0x44 // 设置扫描线
#define TE_SCAN_LINE_REED         0x45 // 读取扫描线
#define SPI_READ_OFF              0x46 // 关闭SPI读取模式
#define SPI_READ_ON               0x47 // 打开SPI读取模式
#define AOD_OFF                   0x48 // 关闭AOD模式
#define AOD_ON                    0x49 // 打开AOD模式
#define AOD_BRIGHTNESS_WRITE      0x4A // 设置AOD亮度
#define AOD_BRIGHTNESS_READ       0x4B // 读取AOD亮度
#define DEEP_SLEEP_CONTROL        0x4F // 设置深度待机模式
#define NORMAL_BRIGHTNESS_WRITE   0x51 // 设置正常模式亮度
#define NORMAL_BRIGHTNESS_READ    0x52 // 读取正常模式亮度
#define BRIGHTNESS_CONTROL_WRITE1 0x53 // 亮度设置控制
#define BRIGHTNESS_CONTROL_READ   0x54 // 读取亮度设置控制
#define BRIGHTNESS_CONTROL_WRITE2 0x55 // 亮度预设控制
#define BRIGHTNESS_CONTROL_READ2  0x56 // 读取亮度预设控制
#define CE_BRIGHTNESS_WRITE       0x58 // 设置阳光下可读性增强
#define CE_BRIGHTNESS_READ        0x59 // 读取 sunlight 模式亮度
#define HBM_BRIGHTNESS_WRITE      0x63 // 设置全局高亮模式亮度
#define HBM_BRIGHTNESS_READ       0x64 // 读取全局高亮模式亮度
#define HBM_CONTROL_WRITE         0x66 // 设置全局高亮模式控制
#define COLOR_SET                 0x70 // spi三位色彩模式颜色设置0x70~0x7F
#define COLOR_OPTION              0x80 // spi三位和256色相关设置
#define DDB_READ                  0xA1 // 读取厂商信息
#define DDB_READ_CONTINUE         0xA8 // 持续读取厂商信息
#define CHECKSUM_READ             0xAA // 读取第一位校验和
#define CHECKSUM_READ_CONTINUE    0xAC // 持续读取第一位校验和
#define SPI_MODE                  0xC4 // 设置spi模式
#define READ_ID                   0xDA // 读取ID 0xDA~0xDC
// 设备结构体
//typedef struct AMOLED
//{
//    SPI_HandleTypeDef *hspi; // spi指针
//} AMOLED;
