#include "i2c.h"

void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 配置PB10与PB11为开漏输出.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 开漏输出.
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void IIC_Wri_SCL(uint8_t Val) 
{
	GPIO_WriteBit(SCL_PORT, SCL_Pin, (BitAction)Val);
	Delay_us(10);  // 进行一定的延时.
}

uint8_t IIC_Read_SCL(void) 
{
	uint8_t Result;
	Result = GPIO_ReadInputDataBit(SCL_PORT, SCL_Pin);
	Delay_us(10);
	return Result;
}

void IIC_Wri_SDA(uint8_t Val)
{
	GPIO_WriteBit(SDA_PORT, SDA_Pin, (BitAction)Val);
	Delay_us(10);
}

uint8_t IIC_Read_SDA(void)
{
	uint8_t Result;
	Result = GPIO_ReadInputDataBit(SDA_PORT, SDA_Pin);
	Delay_us(10);
	return Result;
}

void IIC_StartChannel(void) 
{
	// 同时释放SDA SCL,使其保持高电平.
	IIC_Wri_SDA(HIGH_LEVEL);
	IIC_Wri_SCL(HIGH_LEVEL);
	
	// 先拉低SDA,起始信号出现,再拉低SCL.
	IIC_Wri_SDA(LOW_LEVEL);
	IIC_Wri_SCL(LOW_LEVEL);
}

void IIC_StopChannel(void)
{
	// 先拉低SDA.确保之后在SCL高电平工作期间一定能够产生上升沿.
	IIC_Wri_SDA(LOW_LEVEL);
	IIC_Wri_SCL(HIGH_LEVEL);
	IIC_Wri_SDA(HIGH_LEVEL);
}

void IIC_SendByte(uint8_t Byte)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		IIC_Wri_SDA(Byte & (0x80 >> i));
		
		// 经过一个时钟周期.在该周期内主要是从机从SDA上读取数据.
		IIC_Wri_SCL(HIGH_LEVEL);
		IIC_Wri_SCL(LOW_LEVEL);
	}
}

uint8_t IIC_RecvByte(void)
{
	uint8_t Byte = 0x00;
  IIC_Wri_SDA(HIGH_LEVEL);
	for(uint8_t i = 0; i < 8; i++)
	{
		IIC_Wri_SCL(HIGH_LEVEL);
		if (IIC_Read_SDA() == 1) { Byte |= (0x80 >> i); }
		IIC_Wri_SCL(LOW_LEVEL);
	}
	
	return Byte;
}

void IIC_SendACk(uint8_t Ack)
{
	IIC_Wri_SDA((BitAction)Ack);
	IIC_Wri_SCL(HIGH_LEVEL);
	IIC_Wri_SCL(LOW_LEVEL);
}

uint8_t IIC_RecvAck(void)
{
	uint8_t Ack;
	IIC_Wri_SDA(HIGH_LEVEL); // 主机释放SDA.将SDA交给从机.
	IIC_Wri_SCL(HIGH_LEVEL); // SCL高电平期间主机读取SDA上的应答信号
	Ack = IIC_Read_SDA();
	IIC_Wri_SCL(LOW_LEVEL);
	return Ack;
}

