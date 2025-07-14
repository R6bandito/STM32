#include "tim_pwm.h"

void TIM_PWM_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	GPIO_InitTypeDef GPIOA_InitStructure;
	TIM_OCInitTypeDef TIMOC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启TIM2时钟.
	TIM_InternalClockConfig(TIM2); // 将TIM2配置到内部时钟(CK_INT).
	
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出.
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOA_InitStructure);
	
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数.
	TIM_InitStructure.TIM_Period = 200 - 1;
	TIM_InitStructure.TIM_Prescaler = 7200 - 1;
	TIM_InitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
	TIM_ARRPreloadConfig(TIM2, ENABLE); // 自动重装寄存器预加载使能.
	TIM_Cmd(TIM2, ENABLE);
	
	TIMOC_InitStructure.TIM_OCIdleState = TIM_OCNIdleState_Set; // PWM通道空闲状态输出高电平.
	TIMOC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM1模式.
	TIMOC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 设置输出极性为高电平.
	TIMOC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIMOC_InitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM2, &TIMOC_InitStructure);
}
