#ifndef __MYRTC_H__
#define __MYRTC_H__

#include "stm32f10x.h"                  // Device header
#include <time.h>

#define IS_RESET  0xffff	// 检测装置是否发生复位.

typedef struct MyTime { // 时间结构体.
	uint16_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
} Time_t;

void MYRTC_Init(void); // RTC时钟初始化.

void MYRTC_GetTime(void); // 获取时间.

void MYRTC_SetTime(void);  // 设置时间.

void MYRTC_SetTimeStruct(Time_t *tmt); // 配置时间结构体.

#endif // __MYRTC_H__
