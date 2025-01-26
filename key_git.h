#ifndef __KEYS_H__
#define __KEYS_H__

#include <stm32f10x.h>
#include "Delay.h"

#define KEY_UNPRESS 1
#define KEY_PRESSED 0
#define KEY1_PRESSED 6
#define KEY2_PRESSED 8

#define KEY_DELAY  30

void KeyInit(void);  //按键初始化.

uint8_t GetKeyNum(void);  //获取按键.


#endif

