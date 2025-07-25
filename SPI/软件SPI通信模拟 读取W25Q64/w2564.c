#include "w2564.h"

void W25Q64_Init(void)
{
	MYSPI_Init();
}

void W25_GetDeviceId(uint8_t *Manuf, uint16_t *DID)
{
	MYSPI_Control(SPI_START);
	MYSPI_SwapData(W25__CMD_JEDEC_ID);  // 读取ID指令.
	
	*Manuf = MYSPI_SwapData(USELESS_DATA);  
	*DID = MYSPI_SwapData(USELESS_DATA);
	*DID <<= 8;
	*DID |= MYSPI_SwapData(USELESS_DATA);
	
	MYSPI_Control(SPI_STOP);
}

void W25_EnableWri(void)
{
	MYSPI_Control(SPI_START);
	MYSPI_SwapData(W25__CMD_WRITE_ENABLE);
	MYSPI_Control(SPI_STOP);
}

void W25__WaitBusy(void)
{
	uint32_t Timeout;
	MYSPI_Control(SPI_START);
	MYSPI_SwapData(W25__CMD_READ_STATUS_REG_1);
	Timeout = 100000;
	while ((MYSPI_SwapData(USELESS_DATA) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	MYSPI_Control(SPI_STOP);
}

void W25__PageProgram(uint32_t Address, uint8_t* TxDataArray, uint16_t Count)
{
	W25_EnableWri(); // 每次写入操作前必须进行写使能.
	
	MYSPI_Control(SPI_START);
	MYSPI_SwapData(W25__CMD_PAGE_PROGRAM);
	MYSPI_SwapData(Address >> 16);
	MYSPI_SwapData(Address >> 8);
	MYSPI_SwapData(Address);
	
	for(uint8_t i = 0; i < Count; i++)
	{
		MYSPI_SwapData(TxDataArray[i]);
	}
	MYSPI_Control(SPI_STOP);
	
	W25__WaitBusy();
}

void W25__EraseSector(uint32_t Address) 
{
	W25_EnableWri();
	
	MYSPI_Control(SPI_START);
	MYSPI_SwapData(W25__CMD_SECTOR_ERASE);
	MYSPI_SwapData(Address >> 16);
	MYSPI_SwapData(Address >> 8);
	MYSPI_SwapData(Address);
	MYSPI_Control(SPI_STOP);
	
	W25__WaitBusy();
}

void W25__ReadData(uint32_t Address, uint8_t* RxDataArray, uint32_t Count)
{
	MYSPI_Control(SPI_START);
	MYSPI_SwapData(W25__CMD_READ_DATA);
	MYSPI_SwapData(Address >> 16);
	MYSPI_SwapData(Address >> 8);
	MYSPI_SwapData(Address);
	
	for(uint32_t i = 0; i < Count; i++)
	{
		RxDataArray[i] = MYSPI_SwapData(USELESS_DATA);
	}
	MYSPI_Control(SPI_STOP);
}




