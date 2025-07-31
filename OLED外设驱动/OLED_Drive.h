#ifndef __OLED_DRI_H
#define __OLED_DRI_H

#include "stm32f10x.h"                  // Device header

// 设备识别地址.
#define  SLAVE_ADDRESS  0x78  

// 硬件I2C引脚定义.
#define I2C_SCL_SDA_PortSource  GPIOB
#define I2C_SDA_PinSource GPIO_Pin_7
#define I2C_SCL_PinSource GPIO_Pin_6


#define HEX_SHOW_TYPE_INTEGRITY  0xAA
#define HEX_SHOW_TYPE_SIMPLE  0xBB

/*
	@命令地址映射.
*/
#define SET_CONTRAST_CONTROL  0x81  // 对比度设置.
#define SET_DISPLAY_ON  0xAF  	// 开启显示.
#define SET_DISPLAY_OFF  0xAE  	// 关闭显示.
#define SET_MULTIPLEX_RATIO  0xA8		// 多路复用率设置. 
#define SET_DISPLAY_OFFSET  0xD3		// 显示偏移设置.
#define SET_DISPLAY_CLK_DIV__OSC_FREQ  0xD5  // 时钟分频比/振荡器频率设置.
#define SET_DISPALY_START_LINE0  0x40  // 显示起始行设置为0(从0行开始显示).
#define SET_SENGMENT_REMAP_OFF  0xA1  //正常显示模式(关闭水平翻转). 
#define SET_SENGMENT_REMAP_ON  0xA0   // 开启水平翻转显示.
#define SET_SENGMENT_VERTICAL_NORMAL  0xC8 // 正常显示模式(关闭垂直翻转).
#define SET_SENGMENT_VERTICAL_ON  0xC0 // 开启垂直翻转.
#define SET_COMPINS_HARDWARE_CONFIG  0xDA  // COM引脚硬件配置.
#define SET_PRE_CHARGE_PERIOD  0xD9 		// 预充电周期设置.
#define SET_VCOMH_DESELECT_LEVEL  0xDB  // 设置VCOMH电压倍率.
#define SET_ENTIRE_DISPLAY_RAM  0xA4  // 跟随RAM显示.
#define SET_ENTIRE_DISPLAY_ON  0xA5		// 不跟随RAM显示.
#define SET_NORMAL_DISPLAY  0xA6  // 正常显示.
#define SET_INVERSE_DISPLAY  0xA7 // 倒转显示.
#define SET_CHARGE_PUMP  0x8D   // 设置充电泵.
#define SET_ADDRESSING_MODE 0x20  // 寻址模式设置.

void OLED_Init(void); // OLED初始化.

void OLED_ClearScreen(void); // OLED清屏.

void OLED_DisplayChar(uint8_t Lines, uint8_t Columns, char w_char); // 显示字符.

void OLED_DisplayStr(uint8_t Lines, uint8_t Columns, char* str); // 显示字符串.

void OLED_DisplayNum(uint8_t Lines, uint8_t Columns, uint64_t Num); // 显示数字.

void OLED_DisplayHexNum_4Byte(uint8_t Lines, uint8_t Columns, uint32_t HexNum,  uint8_t Mode); // 显示数字(十六进制)(4字节).

void OLED_DisplayHexNum_1Byte(uint8_t Lines, uint8_t Columns, uint8_t HexNum, uint8_t Mode); // 显示数字(十六进制)(1字节).

/*void OLED_SetCursor(uint8_t Y, uint8_t X)*/;

/* void OLED_WriteByte(uint8_t Byte); */

/* void I2C_Hard_Init(void);  // 硬件I2C初始化.*/

/* ErrorStatus IIC_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT); */

/* void OLED_WriteCommand(uint8_t command); */

#endif // __OLED_DRI_H
