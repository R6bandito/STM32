#include "OLED_Drive.h"
#include "OLED_Font.h"



#ifdef USE_I2C_HAREWARE

	#define I2C_START()  I2C_GenerateSTART(I2C1, ENABLE) // 产生起始信号.
	
	#define I2C_STOP()  I2C_GenerateSTOP(I2C1, ENABLE)  // 产生终止信号.
	
#endif // USE_I2C_HAREWARE


#ifdef USE_I2C_SOFTWARE

	#define OLED_WRITE_SCL(x)  GPIO_WriteBit(I2C_Soft_SCL_PortSource, I2C_Soft_SCL_PinSource, (BitAction)(x))
	
	#define OLED_WRITE_SDA(x)  GPIO_WriteBit(I2C_Soft_SDA_PortSource, I2C_Soft_SDA_PinSource, (BitAction)(x))

#endif // USE_I2C_SOFTWARE

#ifdef USE_I2C_HAREWARE 


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
	I2C_InitStructure.I2C_ClockSpeed = 400000;
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


/*
		@brief: 指令写入.
		@parm: command  SSD1306的控制指令.
*/
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


/*
		@brief:  写入数据.
		@parm: Byte  要写入的字节数据.
*/
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

#endif // USE_I2C_HAREWARE


#ifdef USE_I2C_SOFTWARE
void I2C_Soft_Init(void)
{
	if (I2C_Soft_SCL_PortSource == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	if (I2C_Soft_SCL_PortSource == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	if (I2C_Soft_SCL_PortSource == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_Soft_SCL_PinSource | I2C_Soft_SDA_PinSource;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	if (I2C_Soft_SCL_PortSource == GPIOA) GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	if (I2C_Soft_SCL_PortSource == GPIOB) GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	if (I2C_Soft_SCL_PortSource == GPIOC) GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	OLED_WRITE_SCL(1);
	
	OLED_WRITE_SDA(1);
}


/*
		@brief: 软件I2C产生起始信号.
		@parm: NULL
*/
void I2C_Soft_Start(void)
{
	OLED_WRITE_SCL(1);
	
	OLED_WRITE_SDA(1);
	
	OLED_WRITE_SDA(0);
	
	OLED_WRITE_SCL(0);
}


/*
		@brief:	软件I2C产生停止信号.
		@parm: NULL
*/
void I2C_Soft_Stop(void)
{
	OLED_WRITE_SDA(0);
	
	OLED_WRITE_SCL(1);
	
	OLED_WRITE_SDA(1);
}


/*
		@brief:	软件I2C发送一个字节.
		@parm: Byte  所要发送的字节数据.
*/
void I2C_Soft_SendByte(uint8_t Byte)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		OLED_WRITE_SCL(0);
		
		if (((Byte << i )& 0x80) == 0x80)
		{
			OLED_WRITE_SDA(1);
			
			OLED_WRITE_SCL(1);
		}
		else
		{
			OLED_WRITE_SDA(0);
			
			OLED_WRITE_SCL(1);
		}
	}
}


/*
		@brief:	软件I2C接收应答.
		@parm: NULL.
*/
ErrorStatus I2C_Soft_RecvAck(void)
{
	uint16_t Timeout = 0;
	
	OLED_WRITE_SCL(0);
	
	OLED_WRITE_SDA(1);
	
	OLED_WRITE_SCL(1);
	
	while(GPIO_ReadInputDataBit(I2C_Soft_SDA_PortSource, I2C_Soft_SDA_PinSource))
	{
		Timeout++;
		
		if (Timeout > 1000)
		{
			OLED_WRITE_SCL(0);
			
			return ERROR;
		}
	}
	
	OLED_WRITE_SCL(0);
	
	return SUCCESS;
}


/*
		@brief:	软件I2C写入命令.
		@parm: command  所要写入的命令.
*/
void OLED_WriteCommand(uint8_t command)
{
	I2C_Soft_Start();
	
	I2C_Soft_SendByte(SLAVE_ADDRESS);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(SLAVE_ADDRESS); // 再次尝试重新发送.
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; // 依然未发送成功，终止此次通讯.
	}
	
	
	I2C_Soft_SendByte(0x00);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(0x00); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	
	I2C_Soft_SendByte(command);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(command); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	I2C_Soft_Stop();
}


/*
		@brief:	软件I2C写入数据.
		@parm: Byte  所要写入的数据.
*/
void OLED_WriteByte(uint8_t Byte)
{
	I2C_Soft_Start();
	
	I2C_Soft_SendByte(SLAVE_ADDRESS);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(SLAVE_ADDRESS); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	
	I2C_Soft_SendByte(0x40);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(0x40); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	
	I2C_Soft_SendByte(Byte);
	if (I2C_Soft_RecvAck() != SUCCESS)
	{
		I2C_Soft_SendByte(Byte); 
		
		if (I2C_Soft_RecvAck() != SUCCESS) return; 
	}
	
	I2C_Soft_Stop();
}

#endif // USE_I2C_SOFTWARE


void OLED_WriteCommand_Parm(uint8_t command, uint8_t parm)
{
	OLED_WriteCommand(command);
	OLED_WriteCommand(parm);
}


/*	
		@brief: 光标设置.
		@param: Pages  页坐标 值域 0~7.
		@param: Column  列坐标 值域 0~127.	
*/
void OLED_SetCursor(uint8_t Pages, uint8_t Column)
{
	OLED_WriteCommand(0xB0 + Pages);  // 写入页地址.
	OLED_WriteCommand(0x00 | (Column & 0x0F)); // 写入列地址(低4位)
	OLED_WriteCommand(0x10 | ((Column & 0xF0) >> 4)); // 写入列地址(高4位0)
}


/*
		@brief: OLED清屏.
*/
void OLED_ClearScreen(void)
{
	OLED_SetCursor(0, 0);
	for(uint8_t i = 0; i < 10; i++)
	{
		for(uint8_t j = 0; j < 128; j++)
		{
			OLED_WriteByte(0x00);
		}
	}
}


/*
		@brief:  OLED显示一个字符.
		@parm: Lines  字符所在行数.  
		@parm: Columns  字符所在列数.
		@parm: w_char  所要显示的字符.
*/
void OLED_DisplayChar(uint8_t Lines, uint8_t Columns, char w_char)
{
	OLED_SetCursor((Lines - 1) * 2, (Columns - 1) * 8);
	
	for(uint8_t i = 0; i < 8; i++)  // 显示上半部分.
	{
		OLED_WriteByte(OLED_F8x16[w_char - ' '][i]);
	}
	
	OLED_SetCursor(2 * Lines - 1, (Columns - 1) * 8);
	
	for(uint8_t j = 0; j < 8; j++)  // 显示下半部分.
	{
		OLED_WriteByte(OLED_F8x16[w_char - ' '][j + 8]);
	}
}


/*
		@brief:  OLED显示一个字符串.
		@parm: Lines  字符串所在行数.  
		@parm: Columns  字符串所在列数.
		@parm: w_char  所要显示的字符串.
*/

void OLED_DisplayStr(uint8_t Lines, uint8_t Columns, char* str)
{
	OLED_SetCursor((Lines - 1) * 2, (Columns - 1) * 8);
	
	for(uint8_t i = 0; str[i] != '\0'; i++) // 显示字符串上半部分.
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			OLED_WriteByte(OLED_F8x16[str[i] - ' '][j]);
		}
	}
	
	OLED_SetCursor(2 * Lines - 1, (Columns - 1) * 8);
	
	for(uint8_t i = 0; str[i] != '\0'; i++) // 显示字符串下半部分.
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			OLED_WriteByte(OLED_F8x16[str[i] - ' '][j + 8]);
		}
	}
}


/*
		@brief: 计算所传入数字的位数.
		@parm: Num 所要传入的数字.
*/
uint8_t CountNumLength(uint64_t Num)
{
	uint64_t Time = 10;
	uint8_t Count = 1;
	
	while(Num / Time != 0)
	{
		Num /= Time;
		Count++;
	}
	
	return Count;
}


/*
		@brief:  幂的次方运算.
		@parm: X  底数.
		@parm: Y  指数.
*/
uint64_t GetPow(uint8_t X, uint8_t Y)
{
	uint64_t Result = 1;
	
	while(Y --)
	{
		Result *= X;
	}
	
	return Result;
}


/*
		@brief: 显示数字.
		@parm: Lines 	显示位置(行).
		@parm: Columns  显示位置(列).
		@parm: Num	所要显示的数字.
*/
void OLED_DisplayNum(uint8_t Lines, uint8_t Columns, uint64_t Num)
{
	uint8_t Length = CountNumLength(Num); 
	
	OLED_SetCursor((Lines - 1) * 2, (Columns - 1) * 8);
	
	for(uint8_t i = 0; i < Length; i++)  // 显示上半部分.
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			OLED_WriteByte(OLED_F8x16[Num / GetPow(10, Length - i - 1) % 10 + 16][j]);
		}
	}
	
	OLED_SetCursor((2 * Lines) - 1, (Columns - 1) * 8);
	
	for(uint8_t i = 0; i < Length; i++)
	{
		for(uint8_t j = 0; j < 8; j++)
		{
			OLED_WriteByte(OLED_F8x16[Num / GetPow(10, Length - i - 1) % 10 + 16][j + 8]);
		}
	}
}


/*
		@brief: 显示有符号数字.
		@parm: Lines 	显示位置(行).
		@parm: Columns  显示位置(列).
		@parm: Num	所要显示的数字.
*/
void OLED_DisplaySignedNum(uint8_t Lines, uint8_t Columns, int64_t Num)
{
	uint64_t NumTemp;
	
	if (Num < 0)
	{
		OLED_DisplayChar(Lines, Columns++, '-');
		
		NumTemp = -Num;
	}
	else if (Num > 0)
	{
		OLED_DisplayChar(Lines, Columns++, '+');
		
		NumTemp = Num;
	}
	
	OLED_DisplayNum(Lines, Columns, NumTemp);
}


/*
		@brief: 16进制显示(1字节).
		@parm: Lines 显示位置(行).
		@parm: Columns 显示位置(列).
		@parm: HexNum 数据.
		@parm: Mode 显示模式.
*/

void OLED_DisplayHexNum_1Byte(uint8_t Lines, uint8_t Columns, uint8_t HexNum, uint8_t Mode)
{
	uint8_t num_low = HexNum & 0x0F;
	
	uint8_t num_high = ((HexNum >> 4) & 0x0F);
	
	num_low = (num_low < 10) ? (num_low + '0') : (num_low - 10 + 'A' );
	
	num_high = (num_high < 10) ? (num_high + '0') : (num_high - 10 + 'A' );
	
	if(Mode == HEX_SHOW_TYPE_INTEGRITY)
	{
		OLED_DisplayChar(Lines, Columns, '0');
		
		Columns++;
		
		OLED_DisplayChar(Lines, Columns, 'x');
		
		Columns++;
	}
	
	OLED_DisplayChar(Lines, Columns, num_high); // 显示高4位.
	
	OLED_DisplayChar(Lines, Columns + 1, num_low);  // 显示低4位.
}



/*
		@brief: 16进制显示. (4字节)
		@parm: Lines 显示位置(行).
		@parm: Columns 显示位置(列).
		@parm: HexNum 数据.
		@parm: Mode 显示模式.
*/
void OLED_DisplayHexNum_4Byte(uint8_t Lines, uint8_t Columns, uint32_t HexNum, uint8_t Mode)
{	
	uint8_t HexByte[4];
	
	HexByte[0] = (HexNum >> 24) & 0xFF; 
	
	HexByte[1] = (HexNum >> 16) & 0xFF; 
	
	HexByte[2] = (HexNum >> 8) & 0xFF;
	
	HexByte[3] = (HexNum) & 0xFF;
	
	for(uint8_t i = 0; i < 4; i++)
	{
		OLED_DisplayHexNum_1Byte(Lines, Columns, HexByte[i], Mode);
		
		if (Mode == HEX_SHOW_TYPE_INTEGRITY)
		{
			Mode = HEX_SHOW_TYPE_SIMPLE;
			
			Columns += 4;
			
			continue;
		}
		
		Columns += 2;
	}
}


/*
		@brief: 将一个字节数据以二进制形式显示.
		@parm: Lines 显示的位置(行).
		@parm: Columns 显示的位置(列).
		@parm: Num 数据.
*/
void OLED_DisplayBinNum(uint8_t Lines, uint8_t Columns, uint8_t Num)
{
	char IndexArr_High[5];
	
	char IndexArr_Low[5];
	
	IndexArr_High[4] = '\0';
	
	IndexArr_Low[4] = '\0';
	
	for(uint8_t i = 0; i < 4; i++)
	{
		IndexArr_High[i] = (Num & (0x80 >> i)) ? ('1') : ('0');
	}
	
	for(uint8_t i = 0; i < 4; i++)
	{
		IndexArr_Low[i] = ((Num << 4) & (0x80 >> i)) ? ('1') : ('0');
	}
	
	OLED_DisplayStr(Lines, Columns, IndexArr_High);
	
	OLED_DisplayStr(Lines, Columns + 3, IndexArr_Low);
}


void struct_default_init(OLED_InitTypeDef *OLED_InitStructure)
{
	OLED_InitStructure -> AddressMode = Address_Mode_Horizontal;
	
	OLED_InitStructure -> Contrast = 0xFF;
	
	OLED_InitStructure -> DispalyOffset = 0x00;
	
	OLED_InitStructure -> EntireDis = DISABLE;
	
	OLED_InitStructure -> Inverse = DISABLE;
	
	OLED_InitStructure -> SegmentRemap = DISABLE;
	
	OLED_InitStructure -> VerticalDis = DISABLE;
	
}


void OLED_Init(OLED_InitTypeDef *OLED_InitStructure)
{
	uint32_t i, j;
	
	OLED_InitTypeDef initstructure;
	
	struct_default_init(&initstructure);
	
	if (OLED_InitStructure != NULL)
	{
		memcpy(&initstructure, OLED_InitStructure, sizeof(OLED_InitTypeDef));
	}
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
#ifdef USE_I2C_HAREWARE
	I2C_Hard_Init();
#else 
	I2C_Soft_Init();
#endif // USE_I2C_HAREWARE
	
	// 关闭显示.
	OLED_WriteCommand(SET_DISPLAY_OFF); 
	
	// 设置显示时钟分频比/振荡器频率.  1000b : 振荡器频率; 0000 : 时钟分频系数 (均为默认值.)
	OLED_WriteCommand_Parm(SET_DISPLAY_CLK_DIV__OSC_FREQ, 0x80);
	
	// 多路复用率. 0011 1111 (默认值).
	OLED_WriteCommand_Parm(SET_MULTIPLEX_RATIO, 0x3F);
	
	// 显示偏移. 0000 0000 (默认无偏移).
	OLED_WriteCommand_Parm(SET_DISPLAY_OFFSET, initstructure.DispalyOffset);
	
	// 显示开始行.
	OLED_WriteCommand(SET_DISPALY_START_LINE0);
	
	// 设置寻址模式(0x00: 水平寻址).
	OLED_WriteCommand_Parm(SET_ADDRESSING_MODE, initstructure.AddressMode);
	
	// 是否开启水平翻转.
	if (initstructure.SegmentRemap == DISABLE)
	{
		OLED_WriteCommand(SET_SENGMENT_REMAP_OFF); 
	}
	else 
	{
		OLED_WriteCommand(SET_SENGMENT_REMAP_ON); 
	}

	// 是否开启垂直翻转.
	if (initstructure.VerticalDis == DISABLE)
	{
		OLED_WriteCommand(SET_SENGMENT_VERTICAL_NORMAL);
	}
	else
	{
		OLED_WriteCommand(SET_SENGMENT_VERTICAL_ON);
	}

	
	// COM引脚配置(0001 0010 为默认配置).
	OLED_WriteCommand_Parm(SET_COMPINS_HARDWARE_CONFIG, 0x12); 
	
	// 对比度设置.(0x00 ~ 0xFF).
	OLED_WriteCommand_Parm(SET_CONTRAST_CONTROL, initstructure.Contrast);
	
	// 预充电周期设置.
	OLED_WriteCommand_Parm(SET_PRE_CHARGE_PERIOD, 0xF1);
	
	// 设置VCOMH电压倍率.
	OLED_WriteCommand_Parm(SET_VCOMH_DESELECT_LEVEL, 0x30);
	
	// 是否跟随RAM显示.
	if (initstructure.EntireDis == DISABLE) 	
	{
		OLED_WriteCommand(SET_ENTIRE_DISPLAY_RAM);
	}
	else 
	{
		OLED_WriteCommand(SET_ENTIRE_DISPLAY_ON);
	}

	// 是否反相显示.
	if (initstructure.Inverse == DISABLE)
	{
		OLED_WriteCommand(SET_NORMAL_DISPLAY);		
	}
	else 
	{
		OLED_WriteCommand(SET_INVERSE_DISPLAY);
	}

	// 设置充电泵.
	OLED_WriteCommand_Parm(SET_CHARGE_PUMP, 0x14);
	
	// 开启显示.
	OLED_WriteCommand(SET_DISPLAY_ON);
	
	OLED_ClearScreen();
}

