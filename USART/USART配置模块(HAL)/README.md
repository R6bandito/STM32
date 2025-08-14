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

```c
/*  单，双串口初始化开关.    */

 #define __SINGLE__

 /* #define __DUAL__ */

/*  注意：使用双串口初始化API时，需要解锁__DUAL__宏. 注释掉__SINGKE__ */
```

​	该宏用于是否启动双通道配置。 代码中提供了单串口初始化与双串口初始化API。若要使用双串口API，务必先解除`__DUAL__`，同时注释掉 `__SINGLE__`。 **注意：严禁同时开启 `__SINGLE__` 与 `__DUAL__`，会产生未定义行为！**

```c
#define MAX_BAUDRATE   115200
#define MAX_BACKUP 5
```

- `MAX_BAUDRATE` : 最大传输波特率限制（默认为115200.）可根据实际情况进行修改。
- `MAX_BACKUP`: 串口挂起列表长度。 默认可支持最大5个串口的挂起状态。

```c
extern USART_HandleTypeDef USARTx_handle;

extern USART_HandleTypeDef USART1_dual_handle;

extern USART_HandleTypeDef USART2_dual_handle;
```

​	`USARTx_handle`，`USART1_dual_handle`，`USART2_dual_handle`为3个关键串口操纵句柄。`USARTx_handle`用于单串口模式以及双串口模式的`NULL`条件(详见API介绍)。其余用于双串口模式配置。**当使用本文件代码配置了USART时，在使用由HAL提供的USART相关API时传入的应是上述3个句柄之一，切记！**



## API介绍：

#### `void Cus_USART_Single_Init()`

```c
#if defined(__SINGLE__)
  void Cus_USART_Single_Init( uint32_t baudrate );
#endif // __SINGLE__
```

​	该API由宏`__SINGLE__`进行管理，只有在启用`__SINGLE__`时才能进行使用。该API用于单串口初始化，所配置的串口由头文件`usart.h`中的通信参数配置参数进行设置。

参数： `uint32_t baudrate`  传输波特率，由用户传入。注意：传入的波特率会进行范围检查，因此确保波特率处于

 `0 ~ MAX_BAUDRATE`之间。 **若检查未通过则将会采用默认波特率进行配置(9600)并输出提示信息**。

使用示例：

```c
	Cus_USART_Single_Init( 9600 );  


/*	将串口USART1配置为 Tx_Rx模式，无校验功能， 1位停止位， 数据字长为8位， 通信波特率9600。	*/
/*
	通信参数配置如下：
	#define USARTx   USART1

	#define USART_PortSource  GPIOA

	#define USART_PinSource_Tx  GPIO_PIN_9 

	#define USART_PinSource_Rx  GPIO_PIN_10	
	
	#define DISABLE_PARITY  // 默认关闭校验.

	#ifndef DISABLE_PARITY
	#define ENABLE_PARITY
	#endif

	#if defined(ENABLE_PARITY) && !defined(DISABLE_PARITY)
	#define PARITY_ODD   // 开启校验后，默认为奇校验.
	/* #define PARITY_EVEN */		// 可选：取消注释以启用偶校验

 	#define STOPBITS_1  // 定义使用1位停止位（默认）

	/* #define STOPBITS_2 */ // 可选：取消注释以使用2位停止位

	/* #define STOPBITS_1_5 */  // 可选：取消注释以使用1.5位停止位

	/* #define STOPBITS_0_5 */  // 可选：取消注释以使用0.5位停止位
*/
```

​	使用单串口API配置成功后，操纵所配置的串口应使用 `USARTx_handle`句柄进行操纵。