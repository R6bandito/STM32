#include "tim_it.h"

void TIM3_Init(void) {
  TIM_TimeBaseInitTypeDef TIM_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	TIM_InternalClockConfig(TIM3); // 内部时钟配置到TIM3时基单元
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 使能TIM3时钟
  TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
  TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
  TIM_InitStructure.TIM_Period = 10000 - 1;
  TIM_InitStructure.TIM_Prescaler = 7200 - 1; // 计时1s
  TIM_InitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &TIM_InitStructure); // 初始化TIM3
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 使能TIM3更新中断

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // 中断通道3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; // 响应优先级
  NVIC_Init(&NVIC_InitStructure); // 初始化中断
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
		timeCount++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
