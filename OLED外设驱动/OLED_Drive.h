#ifndef __OLED_DRI_H
#define __OLED_DRI_H

#include "stm32f10x.h"                  // Device header
#include <string.h>


/*
		@notice: 下列两个宏定义 (USE_I2C_HAREWARE) (USE_I2C_SOFTWARE)分别用于启用不同的I2C通讯方式.
			USE_I2C_HAREWARE : 当启用该宏，且注释掉 USE_I2C_SOFTWARE时，将使用硬件I2C通信. (默认)
			USE_I2C_SOFTWARE : 当启用该宏，且注释掉 USE_I2C_HAREWARE时，将使用软件模拟I2C通信.
			注意：禁止同时启用	USE_I2C_HAREWARE  USE_I2C_SOFTWARE.
*/

/* **************************  */
 #define USE_I2C_HAREWARE 

// #define USE_I2C_SOFTWARE 
/* ************************* */



/*
		@notice:	下列宏定义将仅在 USE_I2C_SOFTWARE 启用时生效.
			I2C_Soft_SCL_PortSource , I2C_Soft_SDA_PortSource :通信时钟线,数据线GPIO端口的代号.通过解除对应端口的注释来启用对应的通信端口.
				(最多支持到GPIOC).
			I2C_Soft_SCL_PinSource , I2C_Soft_SDA_PinSource :为对应GPIO口的引脚，使用时应将其更换为用户实际使用的引脚.
			
			默认情况: PB6 -> I2C_SCL  PB7 -> I2C_SDA.
*/
#ifdef USE_I2C_SOFTWARE
/* *************************************************************  */
	#define SLAVE_ADDRESS  0x78  // OLED设备地址.
	
	#define I2C_Soft_SCL_PortSource  GPIOB
	
//  #define I2C_Soft_SCL_PortSource  GPIOA

//	#define I2C_Soft_SCL_PortSource  GPIOC
	
	#define I2C_Soft_SDA_PortSource GPIOB
	
//	#define I2C_Soft_SDA_PortSource GPIOA

//	#define I2C_Soft_SDA_PortSource GPIOC
	
	#define I2C_Soft_SCL_PinSource  GPIO_Pin_6
	
	#define I2C_Soft_SDA_PinSource  GPIO_Pin_7
/* *************************************************************  */
#endif // USE_I2C_SOFTWARE



/*
		@notice:	下列宏定义将仅在 USE_I2C_HAREWARE 启用时生效.
			I2C_SCL_SDA_PortSource :	硬件I2C所采用的GPIO端口.
			I2C_SDA_PinSource : 通信数据引脚.
			I2C_SCL_PinSource : 时钟线引脚.
			
		注意: 硬件I2C仅支持 I2C1. 因此实际应填入I2C1对应的GPIO端口与引脚. 
*/

#ifdef USE_I2C_HAREWARE
/* *************************************************************  */
	// 设备识别地址.
	#define  SLAVE_ADDRESS  0x78  

	// 硬件I2C引脚定义.
	#define I2C_SCL_SDA_PortSource  GPIOB
	
	#define I2C_SDA_PinSource GPIO_Pin_7
	
	#define I2C_SCL_PinSource GPIO_Pin_6
/* *************************************************************  */
#endif // USE_I2C_HAREWARE


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
#define SET_INVERSE_DISPLAY  0xA7 // 反相显示.
#define SET_CHARGE_PUMP  0x8D   // 设置充电泵.
#define SET_ADDRESSING_MODE 0x20  // 寻址模式设置.

/*
	@notice: OLED配置结构体，利用该结构体进行配置可调节OLED亮度，显示反转，正反相显示等功能.
						可传入NULL，将使用默认参数进行配置.
*/
typedef struct oled_type {
	
	uint8_t DispalyOffset;
	uint8_t AddressMode;
	FunctionalState SegmentRemap;
	uint8_t Contrast;
	FunctionalState VerticalDis;
	FunctionalState Inverse;
	FunctionalState EntireDis;
	
} OLED_InitTypeDef;

#define Address_Mode_Horizontal  0x00
#define Address_Mode_Vertical  0x01
#define Address_Mode_Page  0x02

void OLED_Init(OLED_InitTypeDef *OLED_InitStructure); // OLED初始化.

void OLED_ClearScreen(void); // OLED清屏.

void OLED_DisplayChar(uint8_t Lines, uint8_t Columns, char w_char); // 显示字符.

void OLED_DisplayStr(uint8_t Lines, uint8_t Columns, char* str); // 显示字符串.

void OLED_DisplayNum(uint8_t Lines, uint8_t Columns, uint64_t Num); // 显示数字.

void OLED_DisplaySignedNum(uint8_t Lines, uint8_t Columns, int64_t Num); // 显示数字(正负).

void OLED_DisplayHexNum_4Byte(uint8_t Lines, uint8_t Columns, uint32_t HexNum,  uint8_t Mode); // 显示数字(十六进制)(4字节).

void OLED_DisplayHexNum_1Byte(uint8_t Lines, uint8_t Columns, uint8_t HexNum, uint8_t Mode); // 显示数字(十六进制)(1字节).

void OLED_DisplayBinNum(uint8_t Lines, uint8_t Columns, uint8_t Num);  // 二进制显示.


#endif // __OLED_DRI_H
