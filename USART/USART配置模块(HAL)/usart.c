#include "usart.h"

USART_HandleTypeDef USARTx_handle;

USART_HandleTypeDef USART1_dual_handle;

USART_HandleTypeDef USART2_dual_handle;

USART_SuspendType USART_BackUP[MAX_BACKUP];
uint8_t Index = 0;


uint8_t ErrorParmFlag = 0;

#if defined(__CC_ARM)     // Keil ARM 编译器
#pragma diag_suppress 1296
#endif

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

  #if defined(__SINGLE__)
    int fputc(int ch, FILE *f) {
    (void)f;

      HAL_USART_Transmit(&USARTx_handle, (uint8_t*)&ch, 1, HAL_MAX_DELAY);

      return ch;
    }
  #endif // __SINGLE__

  #if defined(__DUAL__)
    int fputc(int ch, FILE *f) {
      (void)f;

        HAL_USART_Transmit(&USART1_dual_handle, (uint8_t*)&ch, 1, HAL_MAX_DELAY);

        return ch;
      }
  #endif // __DUAL__
#endif


void __weak SystemClock_Config( void )
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1; // HSE预分频，不分频
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON; // Enable PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;  // 选择HSE作为PLL时钟源
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9; // 倍频9倍，8MHz * 9 = 72MHz
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    // 用户自定义的 ErrorHandler().
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // 选择PLL作为系统时钟源
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        // AHB总线时钟不分频
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;         // APB1总线时钟2分频（最大36MHz）
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         // APB2总线时钟不分频
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    // 用户自定义的 ErrorHandler().
  }
  // HAL库系统定时器配置为1ms中断周期
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  
  // 设置系统定时器时钟源
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  // 系统中断优先级分组设置
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void Cus_USART_BackUP_Initial( void )
{
  for(uint8_t i = 0; i < MAX_BACKUP; i++)
  {
    memset(&USART_BackUP[i].USART_BackUP, 0, sizeof(USART_HandleTypeDef));

    USART_BackUP[i].USART_BackUP.Instance = NULL;

    USART_BackUP[i].suspend_flag = RESET;
  }
}



void USART_GPIO_Init( void )
{
  if (USART_PortSource == GPIOA)   __HAL_RCC_GPIOA_CLK_ENABLE();

  if (USART_PortSource == GPIOB)   __HAL_RCC_GPIOB_CLK_ENABLE();

  if (USART_PortSource == GPIOC)   __HAL_RCC_GPIOC_CLK_ENABLE();

#if defined(__DUAL__)
  if (USART_2_PortsSource == GPIOA)   __HAL_RCC_GPIOA_CLK_ENABLE();

  if (USART_2_PortsSource == GPIOB)   __HAL_RCC_GPIOB_CLK_ENABLE();

  if (USART_2_PortsSource == GPIOC)   __HAL_RCC_GPIOC_CLK_ENABLE();

#endif // __DUAL__

  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;

#if defined(__DUAL__)
  if (USART_2_PortsSource != USART_PortSource)
  {
    GPIO_InitStructure.Pull = GPIO_NOPULL;

    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStructure.Pin = USART_PinSource_Tx;

    HAL_GPIO_Init(USART_PortSource, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = USART_2_PinSource_Tx;

    HAL_GPIO_Init(USART_2_PortsSource, &GPIO_InitStructure);
  }

  if (USART_2_PortsSource == USART_PortSource)
  {
    GPIO_InitStructure.Pull = GPIO_NOPULL;

    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStructure.Pin = USART_PinSource_Tx | USART_2_PinSource_Tx;

    HAL_GPIO_Init(USART_PortSource, &GPIO_InitStructure);
  }
#elif defined(__SINGLE__)
  GPIO_InitStructure.Pin = USART_PinSource_Tx;

  GPIO_InitStructure.Pull = GPIO_NOPULL;

  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(USART_PortSource, &GPIO_InitStructure);
#endif 

#if defined(__DUAL__)
  if (USART_2_PortsSource != USART_PortSource)
  {
    GPIO_InitStructure.Pull = GPIO_PULLUP;

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;

    GPIO_InitStructure.Pin = USART_PinSource_Rx;

    HAL_GPIO_Init(USART_PortSource, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = USART_2_PinSource_Rx;

    HAL_GPIO_Init(USART_2_PortsSource, &GPIO_InitStructure);
  }

  if (USART_2_PortsSource == USART_PortSource)
  {
    GPIO_InitStructure.Pull = GPIO_PULLUP;

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;

    GPIO_InitStructure.Pin = USART_PinSource_Rx | USART_2_PinSource_Rx;

    HAL_GPIO_Init(USART_PortSource, &GPIO_InitStructure);
  }
#endif // __DUAL__

}


void Cus_USART_Single_Init( uint32_t baudrate )
{
  if (baudrate <= 0 || baudrate > MAX_BAUDRATE)
  {
		ErrorParmFlag = 1;

    baudrate = 9600;
  }

  Cus_USART_BackUP_Initial();

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


void Cus_USART_Dual_Init( USART_HandleTypeDef *USART_1, 
                             USART_HandleTypeDef *USART_2,
                                Initial_Mode mode )
{
  Cus_USART_BackUP_Initial();

  USART_GPIO_Init();

  switch ((uint32_t)USART_1 -> Instance) 
  {

    case (uint32_t)USART1:
        __HAL_RCC_USART1_CLK_ENABLE();
        break;
    case (uint32_t)USART2:
        __HAL_RCC_USART2_CLK_ENABLE();
        break;
    case (uint32_t)USART3:
        __HAL_RCC_USART3_CLK_ENABLE();
        break;
    default:
        // 默认情况处理
        break;
  }

  switch ((uint32_t)USART_2 -> Instance) 
  {
    case (uint32_t)USART1:
        __HAL_RCC_USART1_CLK_ENABLE();
        break;
    case (uint32_t)USART2:
        __HAL_RCC_USART2_CLK_ENABLE();
        break;
    case (uint32_t)USART3:
        __HAL_RCC_USART3_CLK_ENABLE();
        break;
    default:
        // 默认情况处理
        break;
  }

  if (USART_1 == NULL && USART_2 == NULL)
  {
    if (USARTx_dual == USART1)   __HAL_RCC_USART1_CLK_ENABLE();

    if (USARTx_dual == USART2)   __HAL_RCC_USART2_CLK_ENABLE();
  
    if (USARTx_dual == USART3)   __HAL_RCC_USART3_CLK_ENABLE();

    Cus_USART_Single_Init(9600);

    USART1_dual_handle = USARTx_handle;

    USART1_dual_handle . Instance = USARTx_dual;

    HAL_USART_Init(&USART1_dual_handle);
  }


  if (mode == INITIAL_MODE_EQUAL)
  {
    USART1_dual_handle = *USART_1;

    USART_TypeDef *temp = USART_2 -> Instance;

    *USART_2 = *USART_1;

    USART2_dual_handle = *USART_2;

    USART2_dual_handle  .  Instance = temp;

    HAL_USART_Init(&USART1_dual_handle);

    HAL_USART_Init(&USART2_dual_handle);
  }

  if (mode == INITIAL_MODE_IND)
  {
    USART1_dual_handle = *USART_1;

    HAL_USART_Init(&USART1_dual_handle);

    USART2_dual_handle = *USART_2;

    HAL_USART_Init(&USART2_dual_handle);
  }
}


void Cus_USART_Suspend( USART_HandleTypeDef *USART )
{
  uint8_t Count = 0 ;

  for(uint8_t i = 0; i < MAX_BACKUP; i++)
  {
    if (USART_BackUP[i].suspend_flag == SET) Count++;

    if (Count == 5)
    {
      printf("Suspend Error.Suspend List has not enough memory.\n");

      return;
    }
  }

  USART_BackUP[Index].USART_BackUP = *USART;

  USART_BackUP[Index].suspend_flag = SET;

  HAL_USART_DeInit(USART);

  switch ((uint32_t)USART -> Instance) 
  {

    case (uint32_t)USART1:
        __HAL_RCC_USART1_CLK_DISABLE();
        break;
    case (uint32_t)USART2:
        __HAL_RCC_USART2_CLK_DISABLE();
        break;
    case (uint32_t)USART3:
        __HAL_RCC_USART3_CLK_DISABLE();
        break;
    default:
        // 默认情况处理
        break;
  }

  Index++;
}


void Cus_USART_Resume( USART_HandleTypeDef *USART )
{
  for(uint8_t i = 0; i < MAX_BACKUP; i++)
  {
    if (memcmp(&USART_BackUP[i].USART_BackUP, USART, sizeof(USART_HandleTypeDef)) == 0
          && USART_BackUP[i].suspend_flag == SET)
    {
      switch ((uint32_t)USART -> Instance) 
      {
    
        case (uint32_t)USART1:
            __HAL_RCC_USART1_CLK_ENABLE();
            break;
        case (uint32_t)USART2:
            __HAL_RCC_USART2_CLK_ENABLE();
            break;
        case (uint32_t)USART3:
            __HAL_RCC_USART3_CLK_ENABLE();
            break;
        default:
            // 默认情况处理
            break;
      }

      USARTx_handle = USART_BackUP[i].USART_BackUP;

      HAL_USART_Init(&USARTx_handle);

      USART_BackUP[i].suspend_flag = RESET;

      Index = i;

      return;
    }
  }

  printf("Resume Error. Not found this USART in Suspend List.\n"); 
}

