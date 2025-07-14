/*
	芯片：STM32F103C8T6
	外设：TIM3 , OLED
	描述：定时器更新中断的简单运用(秒数计时)
*/

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "tim_it.h"

uint8_t timeCount = 0;

int main(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组
	OLED_Init();
	TIM3_Init();
	OLED_ShowString(2, 4, "Count:");
	while(1) {
		OLED_ShowNum(2, 10, timeCount, 5);
		OLED_ShowNum(3, 7, TIM_GetCounter(TIM3), 5);
	}
}


