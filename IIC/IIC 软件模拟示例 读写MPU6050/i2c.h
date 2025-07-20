#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f10x.h"
#include "Delay.h"

#define SCL_PORT  GPIOB
#define SCL_Pin  GPIO_Pin_10

#define SDA_PORT  GPIOB
#define SDA_Pin  GPIO_Pin_11

#define HIGH_LEVEL 1 // 高电平.
#define LOW_LEVEL 0 // 低电平.

void IIC_Init(void); // i2c通信层初始化.

void IIC_Wri_SCL(uint8_t Val); // 写SCL信号线.

uint8_t IIC_Read_SCL(void); // 读SCL信号线.

void IIC_Wri_SDA(uint8_t Val); // 写SDA信号线.

uint8_t IIC_Read_SDA(void); // 读SDA信号线.

void IIC_StartChannel(void); // 起始信号.

void IIC_StopChannel(void); // 中止信号.

void IIC_SendByte(uint8_t Byte); // 发送一个字节数据.

uint8_t IIC_RecvByte(void); // 接受一个字节.

void IIC_SendACk(uint8_t Ack); // 发送一个应答.

uint8_t IIC_RecvAck(void); // 接收应答.

#endif // __IIC_H__
