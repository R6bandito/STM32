#ifndef __KEY_MORE_H__
#define __KEY_MORE_H__

#include "stm32f10x.h"

#define KEY_UNPRESS 1
#define KEY_PRESSED 0

#define KEY1_PRESSED  6
#define KEY2_PRESSED  8

//采样次数.
#define KEY_SAMPLES 40

void Key_Init(void);

uint8_t Get_Key_Num(void);

#endif  //__KEY_MORE_H__
