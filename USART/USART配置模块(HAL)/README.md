## 概述：

​	这段代码是一个面向STM32微控制器的串口(USART)通信模块的底层初始化和支持性代码，分为单串口和双串口两种模式。代码主要包括**串口 GPIO 引脚初始化**、**单串口和双串口的配置初始化**，以及对串口调试输出的重定向，同时支持使用宏定义控制串口通信参数（如波特率、校验位、停止位等）。

## 通信参数配置:	

```c
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
/* 默认参数为使用USART3, 端口为GPIOB  Tx引脚:PB10  Rx引脚:PB11     */
```

​	上述宏定义为整个文件最关键部分，用于**映射串口的物理位置以及串口号**。在调用API时必须确保上述宏定义正确映射到了所用芯片的串口。在使用双串口配置模式时，也是根据上述宏定义进行配置的。



```c
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
```

​	此段宏开关用于控制配置USART时是否开启校验模式。当注释掉 `DISABLE_PARITY`时

```c
// #define DISABLE_PARITY
```

​	将会自动开启校验模式(`ENABLE_PARITY`)，同时，默认的校验模式为奇校验。 若需要使用偶校验，可注释掉`#define PARITY_ODD`

同时启用`#define PARITY_EVEN`。**注意：同时启用奇,偶校验将会产生覆盖行为，最终覆盖后的行为为 偶校验模式！**



```c
/*  此段宏定义用于配置USART通信中的停止位长度			 */
 #define STOPBITS_1  // 定义使用1位停止位（默认）

/* #define STOPBITS_2 */ // 可选：取消注释以使用2位停止位

/* #define STOPBITS_1_5 */  // 可选：取消注释以使用1.5位停止位

/* #define STOPBITS_0_5 */  // 可选：取消注释以使用0.5位停止位
/*  注：同时启用多个宏将发生覆盖！        */
```

​	该段宏定义用于停止位配置。默认为1个停止位，通过解除注释来选择不同的停止位。**注意：不允许同时启用两个及以上停止位，否则将发生覆盖行为！**



```c
typedef  uint8_t  Initial_Mode;

#define INITIAL_MODE_EQUAL  0xFF
#define INITIAL_MODE_IND  0xFE
#define INITIAL_MODE_NULL  0x00
```

​	双通道配置中的模式选择。

- `INITIAL_MODE_EQUAL`:  两个串口用相同参数初始化。
- `INITIAL_MODE_IND`: 允许两个串口不同配置。
- `INITIAL_MODE_NULL`: 作为默认处理模式，用于单串口扩展。