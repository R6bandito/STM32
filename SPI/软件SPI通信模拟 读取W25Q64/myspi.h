#ifndef __MYSPI_H__
#define __MYSPI_H__

#include "stm32f10x.h"
#include "assert.h"

#define SPI_CS_PORT  GPIOA
#define SPI_CS_PIN   GPIO_Pin_4

#define SPI_SCL_PORT  GPIOA
#define SPI_SCL_PIN  GPIO_Pin_5

#define SPI_MOSI_PORT  GPIOA 
#define SPI_MOSI_PIN  GPIO_Pin_7

#define SPI_MISO_PORT  GPIOA
#define SPI_MISO_PIN  GPIO_Pin_6

#define SPI_START  0
#define SPI_STOP  1

void MYSPI_Init(void); // 软件SPI通信初始化.

void MYSPI_Wri_CS(uint8_t BitVal); // 写片选线.

void MYSPI_Wri_SCK(uint8_t BitVal); // 写时钟线.

void MYSPI_Wri_MOSI(uint8_t BitVal); // 写MOSI.

uint8_t MYSPI_Read_MISO(void); // 读取数据.

void MYSPI_Control(uint8_t Mode); // 使能失能控制.

uint8_t MYSPI_SwapData(uint8_t ByteSend); // 数据交换.

#endif // __MYSPI_H__
