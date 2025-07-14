#ifndef __KEY_PWM_H
#define __KEY_PWM_H

#include "stm32f10x.h"
#include "Delay.h"

extern volatile uint8_t modeChange;

void KEY_PWM_Init(void); // 初始化按键.

void EXTI15_10_IRQHandler(void); // 外部中断处理.

#endif // __KEY_PWM_H
