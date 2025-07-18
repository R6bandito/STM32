#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include "OLED.h"

extern uint8_t RxData;

extern uint8_t RxFlag;

extern uint8_t TxFlag;

int main(void) 
{
	OLED_Init();
	Serial_Init();
	OLED_ShowString(1, 2, "RxData:");
	OLED_ShowString(3, 2, "TxData:");
	while(1)
	{
		if (RxFlag == RECEIVE)
		{
			OLED_ShowHexNum(1, 10, RxData, 2);
			TxFlag = TRANSFER;
		}
		
		if (TxFlag == TRANSFER) 
		{
			USART_SendByte(RxData);
			OLED_ShowHexNum(3, 10, RxData, 2);
			TxFlag = 0; RxFlag = NONE;
		}
		
	}
}
