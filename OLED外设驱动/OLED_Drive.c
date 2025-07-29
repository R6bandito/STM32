#include "OLED_Drive.h"

#define I2C_START()  I2C_GenerateSTART(I2C1, ENABLE) // 产生起始信号.
#define I2C_STOP()  I2C_GenerateSTOP(I2C1, ENABLE)  // 产生终止信号.

void I2C_Hard_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// I2C_SCL_PinSource -> I2C1_SCL.  I2C_SDA_PinSource -> I2C1_SDA.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PinSource | I2C_SDA_PinSource;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SCL_SDA_PortSource, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; 	
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0xF0;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	I2C_Cmd(I2C1, ENABLE);
}

ErrorStatus IIC_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint16_t Timeout = 5000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout--;
		if (Timeout == 0) return ERROR;
	}
	return SUCCESS;
}

void OLED_WriteCommand(uint8_t command)
{
	I2C_START();
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, SLAVE_ADDRESS, I2C_Direction_Transmitter);
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, 0x00); // 写命令.
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C1, command);
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_STOP();
}

void OLED_WriteByte(uint8_t Byte)
{
	I2C_START();
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, SLAVE_ADDRESS, I2C_Direction_Transmitter);
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, 0x40); // 写数据.
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C1, Byte);
	IIC_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_STOP();
}

void OLED_WriteCommand_Parm(uint8_t command, uint8_t parm)
{
	OLED_WriteCommand(command);
	OLED_WriteCommand(parm);
}

void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1500; j++);
	}
	
	I2C_Hard_Init();
	
	// 关闭显示.
	OLED_WriteCommand(SET_DISPLAY_OFF); 
	
	// 设置显示时钟分频比/振荡器频率.  1000b : 振荡器频率; 0000 : 时钟分频系数 (均为默认值.)
	OLED_WriteCommand_Parm(SET_DISPLAY_CLK_DIV__OSC_FREQ, 0x80);
	
	// 多路复用率. 0011 1111 (默认值).
	OLED_WriteCommand_Parm(SET_MULTIPLEX_RATIO, 0x3F);
	
	// 显示偏移. 0000 0000 (默认无偏移).
	OLED_WriteCommand_Parm(SET_DISPLAY_OFFSET, 0x00);
	
	// 显示开始行.
	OLED_WriteCommand(SET_DISPALY_START_LINE0);
	
	// 正常显示模式(关闭水平翻转).
	OLED_WriteCommand(SET_SENGMENT_REMAP_OFF); 
	
	// 正常显示模式(关闭垂直翻转).
	OLED_WriteCommand(SET_SENGMENT_VERTICAL_NORMAL);
	
	// COM引脚配置(0001 0010 为默认配置).
	OLED_WriteCommand_Parm(SET_COMPINS_HARDWARE_CONFIG, 0x12); 
	
	// 对比度设置.(0x00 ~ 0xFF).
	OLED_WriteCommand_Parm(SET_CONTRAST_CONTROL, 0x98);
	
	// 预充电周期设置.
	OLED_WriteCommand_Parm(SET_PRE_CHARGE_PERIOD, 0xF1);
	
	// 设置VCOMH电压倍率.
	OLED_WriteCommand_Parm(SET_VCOMH_DESELECT_LEVEL, 0x30);
	
	// 跟随RAM显示.
	OLED_WriteCommand(SET_ENTIRE_DISPLAY_RAM);
	
	// 正常显示.
	OLED_WriteCommand(SET_NORMAL_DISPLAY);
	
	// 设置充电泵.
	OLED_WriteCommand_Parm(SET_CHARGE_PUMP, 0x14);
	
	// 开启显示.
	OLED_WriteCommand(SET_DISPLAY_ON);
	
}

