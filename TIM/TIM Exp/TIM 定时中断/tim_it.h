#ifndef __TIM_IT_H
#define __TIM_IT_H

#include "stm32f10x.h"

extern uint8_t timeCount;

void TIM3_Init(void); // 初始化TIM3.

void TIM3_IRQHandler(void); // 中断处理

#endif // __TIM_IT_H
