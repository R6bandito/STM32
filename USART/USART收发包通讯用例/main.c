/*
	芯片: STM32F103C8T6
  外设: USART1 , GPIOB , GPIOA
	描述: 自定义数据包协议，通过USART串口与电脑进行数据包收发通讯.

*/
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

extern uint8_t TxData[];

extern uint8_t RxData[];

extern uint8_t RxFlag;

int main(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Serial_Init();
	Serial_Key_Init();
	OLED_Init();
	
	TxData[0] = 0x45;
	TxData[1] = 0x12;
	TxData[2] = 0xab;
	TxData[3] = 0xfa;
	
	OLED_ShowString(1, 2, "TxData:");
	OLED_ShowString(3, 2, "RxData:");
	while(1) {
		uint8_t KeyNum = Serial_Key_Scan();
		if (KeyNum == KEY_PRESSED) Serial_SendPacket();
		
		if (RxFlag == RX_FLAG_SET) {
			OLED_ShowHexNum(4, 1, RxData[0], 2);
			OLED_ShowHexNum(4, 4, RxData[1], 2);
			OLED_ShowHexNum(4, 7, RxData[2], 2);
			OLED_ShowHexNum(4, 10, RxData[3], 2);
			RxFlag = RX_FLAG_RESET; 
		}
		
		OLED_ShowHexNum(2, 1, TxData[0], 2);
		OLED_ShowHexNum(2, 4, TxData[1], 2);
		OLED_ShowHexNum(2, 7, TxData[2], 2);
		OLED_ShowHexNum(2, 10, TxData[3], 2);
	}
}
