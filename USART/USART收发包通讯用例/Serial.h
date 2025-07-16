#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "stm32f10x.h"

#include "assert.h"

#define WAIT_HEAD 0 // 等待包头状态.

#define RECEIVE_DATA 1 // 接收数据状态.

#define WAIT_TAIL 2 // 等待包尾状态.

#define PACK_LENGTH 4 // 数据载荷长度.(不含包头包尾)

#define RX_FLAG_SET 1

#define RX_FLAG_RESET 0

#define KEY_RELEASE 1 // 按键松开.

#define KEY_PRESSED 0 // 按键按下.

#define DEBOUNCE 15 // 采样计数.

void Serial_Init(void); // 串口初始化.

void Serial_Key_Init(void); // 串口按键初始化.

uint8_t Serial_Key_Scan(void); // 按键扫描.

void Serial_SendByte(uint8_t Byte); // 单字节发送.

void Serial_SendArray(uint8_t *array, uint8_t Length); // 发送数组.

void Serial_SendPacket(void); // 数据包发送.

void USART1_IRQHandler(void); // 中断处理.

#endif // __SERIAL_H__
