/*
	芯片：STM32F103C8T6
	外设：TIM2 , 发光二极管
	描述：呼吸灯效果：
		使用PA1引脚驱动LED（低电平点亮），通过PWM实现呼吸灯效果（亮度从暗到亮再到暗循环）。
		PWM周期固定为 20ms，占空比从 0% 线性增加到 100%，再线性减少到 0%，单次变化步进为 1%，每 10ms 更新一次占空比。
		使用PA10引脚连接按键（按下为低电平），通过外部中断检测按键。每次按键按下时，切换PWM占空比更新速度：
			模式1：每10ms更新一次占空比（默认）
			模式2：每20ms更新一次占空比
			PA8引脚作为模式指示灯.当切换到模式1(20ms)时，灯亮起。当切回模式0(10ms)时，灯熄灭.
*/

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "key_pwm.h"
#include "tim_pwm.h"
#include "Delay.h"

volatile uint8_t modeChange = 0; // 模式切换标志.

int main(void) {
	OLED_Init();
	TIM_PWM_Init();
	KEY_PWM_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断优先级分组.
	while(1) {
		for(int k = 0 ; k < 200 ; k++) {
			TIM_SetCompare2(TIM2, k);
			if (modeChange == 0) Delay_ms(10);
			else Delay_ms(20);
		}
		for(int i = 200 ; i > 0 ; i--) {
			TIM_SetCompare2(TIM2, i);
			if (modeChange == 0) Delay_ms(10);
			else Delay_ms(20);
		}
	}
}


