#include "stm32f10x.h"
#include "OLED.h"
#include "w2564.h"

uint8_t Manuf;
uint16_t DID;

uint8_t TxArray[3] = {0x45, 0xa5, 0x8d};
uint8_t RxArray[3] = {0x00};

int main(void) 
{
	OLED_Init();
	W25Q64_Init();
	OLED_ShowString(1, 1, "MID,DID:"); 
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");
	
	W25__EraseSector(0x000000);
	
	W25_GetDeviceId(&Manuf, &DID);	
	OLED_ShowHexNum(1, 9, Manuf, 2);
	OLED_ShowHexNum(1, 12, DID, 4);
	
	W25__PageProgram(0x0000ff, TxArray, 3);
	W25__ReadData(0x000000, RxArray, 3);
	
	
	OLED_ShowHexNum(2, 4, TxArray[0], 2);
	OLED_ShowHexNum(2, 8, TxArray[1], 2);
	OLED_ShowHexNum(2, 12, TxArray[2], 2);
	
	OLED_ShowHexNum(3, 4, RxArray[0], 2);
	OLED_ShowHexNum(3, 8, RxArray[1], 2);
	OLED_ShowHexNum(3, 12, RxArray[2], 2);
	
	
	while(1)
	{
		
	}
}


