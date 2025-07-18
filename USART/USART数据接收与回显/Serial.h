#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "stm32f10x.h"

#define RECEIVE 1

#define NONE 0

#define TRANSFER 1

void Serial_Init(void); 

void USART2_IRQHandler(void);

void USART_Receive(void);

void USART_SendByte(uint8_t Byte);


#endif // __SERIAL_H__
