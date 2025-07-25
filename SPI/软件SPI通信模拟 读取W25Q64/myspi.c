#include "myspi.h"

void MYSPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// PA5 -> SCL , PA7 -> MOSI, PA4 -> SS(片选).
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA6 -> MISO.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	MYSPI_Wri_CS(1); // 默认状态下不选中.
	MYSPI_Wri_SCK(0); // SCK空闲状态为低电平.
}

void MYSPI_Wri_CS(uint8_t BitVal)
{
	GPIO_WriteBit(SPI_CS_PORT, SPI_CS_PIN, (BitAction)BitVal);
}

void MYSPI_Wri_SCK(uint8_t BitVal)
{
	GPIO_WriteBit(SPI_SCL_PORT, SPI_SCL_PIN, (BitAction)BitVal);
}

void MYSPI_Wri_MOSI(uint8_t BitVal)
{
	GPIO_WriteBit(SPI_MOSI_PORT, SPI_MOSI_PIN, (BitAction)BitVal);
}

uint8_t MYSPI_Read_MISO(void)
{
	return GPIO_ReadInputDataBit(SPI_MISO_PORT, SPI_MISO_PIN);
}

void MYSPI_Control(uint8_t Mode)
{
	assert(Mode == SPI_START || Mode == SPI_STOP);
	MYSPI_Wri_CS(Mode);
}

uint8_t MYSPI_SwapData(uint8_t ByteSend)
{
	for(int i = 0; i < 8; i++)
	{
		MYSPI_Wri_MOSI(ByteSend & 0x80);
		ByteSend <<= 1;
		MYSPI_Wri_SCK(1);
		if (MYSPI_Read_MISO() == 1) 
			ByteSend |= 0x01;
		MYSPI_Wri_SCK(0);
	}
	return ByteSend;
}
