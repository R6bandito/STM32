#ifndef __CUS_USART_H
#define __CUS_USART_H

#include "stm32f1xx_hal.h"

#include "stdio.h"

extern USART_HandleTypeDef USARTx_handle;

extern USART_HandleTypeDef USART1_dual_handle;

extern USART_HandleTypeDef USART2_dual_handle;

#define MAX_BAUDRATE   115200

typedef  uint8_t  Initial_Mode;

#define INITIAL_MODE_EQUAL  0xFF
#define INITIAL_MODE_IND  0xFE
#define INITIAL_MODE_NULL  0x00

/*                     */
 #define __SINGLE__

 /* #define __DUAL__ */
/*                     */



/*  此段宏定义用于配置单个所要使用的 USART 模块        */
#define USARTx   USART1

#define USART_PortSource  GPIOA

#define USART_PinSource_Tx  GPIO_PIN_9 

#define USART_PinSource_Rx  GPIO_PIN_10
/*  默认参数为使用USART1, 端口为GPIOA  Tx引脚:PA9  Rx引脚:PA10  */



/*  此段宏定义用于配置第二个所要使用的 USART 模块(双串口配置)        */
#define USARTx_dual   USART3

#define USART_2_PortsSource  GPIOB

#define USART_2_PinSource_Tx  GPIO_PIN_10

#define USART_2_PinSource_Rx  GPIO_PIN_11
/*                                                    */



/*  此段宏定义用于配置USART通信中的奇偶校验功能     */
#define DISABLE_PARITY  // 默认关闭校验.

#ifndef DISABLE_PARITY
#define ENABLE_PARITY
#endif

#if defined(ENABLE_PARITY) && !defined(DISABLE_PARITY)
#define PARITY_ODD   // 开启校验后，默认为奇校验.
/* #define PARITY_EVEN */		// 可选：取消注释以启用偶校验

#endif
/*  注：启用校验功能后同时开启奇偶校验宏，将会被覆盖为偶校验.  */



/*  此段宏定义用于配置USART通信中的停止位长度			 */
 #define STOPBITS_1  // 定义使用1位停止位（默认）

/* #define STOPBITS_2 */ // 可选：取消注释以使用2位停止位

/* #define STOPBITS_1_5 */  // 可选：取消注释以使用1.5位停止位

/* #define STOPBITS_0_5 */  // 可选：取消注释以使用0.5位停止位
/*  注：同时启用多个宏将发生覆盖！        */



void Cus_USART_Single_Init( uint32_t baudrate );


void Cus_USART_Dual_Init( USART_HandleTypeDef *USART_1, 
                            USART_HandleTypeDef *USART_2,
                              Initial_Mode mode );


#endif // __CUS_USART_H
