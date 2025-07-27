#include "myrtc.h"

Time_t MyTime_Struct;

void MYRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	Time_t tmt;
	
	PWR_BackupAccessCmd(ENABLE);
	
  if (BKP_ReadBackupRegister(BKP_DR1) != IS_RESET) // 复位后若电池正常供电则不进行初始化.
	{	
		RCC_LSEConfig(RCC_LSE_ON); // 启用LSE.
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET); // 等待LSE时钟准备就绪.
	
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForSynchro(); // 等待同步.
		RTC_WaitForLastTask();
		RTC_SetPrescaler(32768 - 1); // 分频系数.
		RTC_WaitForLastTask();
	
		tmt.Year = 2025 - 1900;
		tmt.Month = 7 - 1;
		tmt.Day = 27;
		tmt.Hour = 11;
		tmt.Min = 0;
		tmt.Sec = 0;
		MYRTC_SetTimeStruct(&tmt);
		MYRTC_SetTime();
	
		BKP_WriteBackupRegister(BKP_DR1, IS_RESET);
	}
}

void MYRTC_GetTime(void)
{
	time_t time;
	struct tm *tm_p;
	
	time = RTC_GetCounter() + (8 * 60 * 60);  // 时区设置到东八区.
	tm_p = localtime(&time);
	
	MyTime_Struct.Year = tm_p->tm_year + 1900;
	MyTime_Struct.Month = tm_p->tm_mon + 1;
	MyTime_Struct.Day = tm_p->tm_mday;
	MyTime_Struct.Hour = tm_p->tm_hour;
	MyTime_Struct.Min = tm_p->tm_min;
	MyTime_Struct.Sec = tm_p->tm_sec;
}

void MYRTC_SetTime(void)
{
	time_t time;
	struct tm tm_p;
	
	tm_p.tm_year = MyTime_Struct.Year;
	tm_p.tm_mon = MyTime_Struct.Month;
	tm_p.tm_mday = MyTime_Struct.Day;
	tm_p.tm_hour = MyTime_Struct.Hour;
	tm_p.tm_min = MyTime_Struct.Min;
	tm_p.tm_sec = MyTime_Struct.Sec;
	
	time = mktime(&tm_p);
	RTC_SetCounter(time);
	RTC_WaitForLastTask();
}

void MYRTC_SetTimeStruct(Time_t *tmt)
{
	MyTime_Struct.Year = tmt->Year;
	MyTime_Struct.Month = tmt->Month;
	MyTime_Struct.Day = tmt->Day;
	MyTime_Struct.Hour = tmt->Hour;
	MyTime_Struct.Min = tmt->Min;
	MyTime_Struct.Sec = tmt->Sec;
}



