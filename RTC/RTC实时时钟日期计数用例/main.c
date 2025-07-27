#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "myrtc.h"

extern Time_t MyTime_Struct;

int main(void)
{
	OLED_Init();
	MYRTC_Init();
	
	OLED_ShowString(1, 1, "Data:");
	OLED_ShowString(2, 1, "Time:");
	while(1)
	{
		MYRTC_GetTime();
		OLED_ShowNum(1, 6, MyTime_Struct.Year, 4);
		OLED_ShowNum(1, 11, MyTime_Struct.Month, 2);
		OLED_ShowNum(1, 14, MyTime_Struct.Day, 2);
		
		OLED_ShowNum(2, 6, MyTime_Struct.Hour, 2);
		OLED_ShowNum(2, 9, MyTime_Struct.Min, 2);
		OLED_ShowNum(2, 12, MyTime_Struct.Sec, 2);
		
		OLED_ShowNum(3, 1, RTC_GetCounter(), 10);
	}
}
