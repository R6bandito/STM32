#include "usart.h"

USART_HandleTypeDef USARTx_handle;

uint8_t ErrorParmFlag = 0;

#if 1
#if (__ARMCC_VERSION >= 6010050)            /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");  /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)
#endif

struct __FILE
{
    int handle;
        
};
FILE __stdout;

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

int fputc(int ch, FILE *f) {
 (void)f;

	HAL_USART_Transmit(&USARTx_handle, (uint8_t*)&ch, 1, HAL_MAX_DELAY);

	return ch;
}
#endif



void USART_GPIO_Init( void )
{
  if (USART_PortSource == GPIOA)   __HAL_RCC_GPIOA_CLK_ENABLE();

  if (USART_PortSource == GPIOB)   __HAL_RCC_GPIOB_CLK_ENABLE();

  if (USART_PortSource == GPIOC)   __HAL_RCC_GPIOC_CLK_ENABLE();


  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;

  GPIO_InitStructure.Pin = USART_PinSource_Tx;

  GPIO_InitStructure.Pull = GPIO_NOPULL;

  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(USART_PortSource, &GPIO_InitStructure);

  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;

  GPIO_InitStructure.Pin = USART_PinSource_Rx;

  GPIO_InitStructure.Pull = GPIO_PULLUP;

  HAL_GPIO_Init(USART_PortSource, &GPIO_InitStructure);

}


void Cus_USART_Init( uint32_t baudrate )
{
  if (baudrate <= 0 || baudrate > MAX_BAUDRATE)
  {
		ErrorParmFlag = 1;

    baudrate = 9600;
  }

  USART_GPIO_Init();

  if (USARTx == USART1)   __HAL_RCC_USART1_CLK_ENABLE();

  if (USARTx == USART2)   __HAL_RCC_USART2_CLK_ENABLE();

  if (USARTx == USART3)   __HAL_RCC_USART3_CLK_ENABLE();

  USARTx_handle.Instance = USARTx;

  USARTx_handle.Init.BaudRate = baudrate;

  USARTx_handle.Init.Mode = USART_MODE_TX_RX;

  USARTx_handle.Init.WordLength = USART_WORDLENGTH_8B;

#ifdef DISABLE_PARITY 
  USARTx_handle.Init.Parity = USART_PARITY_NONE;
#endif // DISABLE_PARITY

#if defined (ENABLE_PARITY) && !defined(DISABLE_PARITY)
  USARTx_handle.Init.WordLength = USART_WORDLENGTH_9B;

  #if defined(ENABLE_PARITY) && defined(PARITY_ODD)
    USARTx_handle.Init.Parity = USART_PARITY_ODD;
  #endif // ENABLE_PARITY  PARITY_ODD

  #if defined(ENABLE_PARITY) && defined(PARITY_EVEN)  
    USARTx_handle.Init.Parity = USART_PARITY_EVEN;
  #endif // ENABLE_PARITY   PARITY_EVEN
#endif // ENABLE_PARITY  

#ifdef STOPBITS_1
  USARTx_handle.Init.StopBits = USART_STOPBITS_1;
#endif // STOPBITS_1

#ifdef STOPBITS_2
  USARTx_handle.Init.StopBits = USART_STOPBITS_2;
#endif // STOPBITS_2

#ifdef STOPBITS_1_5
  USARTx_handle.Init.StopBits = USART_STOPBITS_1_5;
#endif // STOPBITS_1_5

#ifdef STOPBITS_0_5
  USARTx_handle.Init.StopBits = USART_STOPBITS_0_5;
#endif // STOPBITS_0_5

  HAL_USART_Init(&USARTx_handle);
	
	if (ErrorParmFlag == 1)
	{
		printf("Invalid BaudRate. Set to Default(9600)\n");
		
		ErrorParmFlag = 0;
	}
}

