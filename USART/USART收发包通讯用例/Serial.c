#include "Serial.h"

uint8_t RxData[PACK_LENGTH]; // 接收数据包的数据载荷部分(不含包头包尾).
uint8_t TxData[PACK_LENGTH]; // 发送数据包的数据载荷部分(不含包头包尾).
uint8_t RxFlag; // 接收标志位.

void Serial_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE); // 时钟使能.
	
	// 配置PA9为 USART1_TX.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置PA10为 USART1_RX.
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	// 配置USART1参数.
	USART_InitStructure.USART_BaudRate = 115200; // 波特率.
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控.
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No; // 无校验位.
	USART_InitStructure.USART_StopBits = USART_StopBits_1; // 1位停止位.
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 8位字长.
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

// 按键PB1初始化.
void Serial_Key_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Serial_SendByte(uint8_t Byte) {
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
}

void Serial_SendArray(uint8_t *array, uint8_t Length) {
	assert(Length > 0);
	for(uint8_t i = 0; i < Length; i++) {
		Serial_SendByte(array[i]);
	}
}

void Serial_SendPacket(void) {
	Serial_SendByte(0xFF); // 发送包头.
	Serial_SendArray(TxData, 4); // 发送数据载荷.
	Serial_SendByte(0xFF); // 发送包尾.
}

// 按键扫描. (按键按下发包)
uint8_t Serial_Key_Scan(void) {
	static uint8_t key_last_state = KEY_RELEASE;
	static uint8_t debounce_counter = 0; // 采样计数次数.
	uint8_t key_current_state = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
	
	if (key_current_state == KEY_PRESSED && key_last_state == KEY_RELEASE) {
		debounce_counter++;
		if (key_current_state == KEY_PRESSED && key_last_state == KEY_RELEASE && debounce_counter > DEBOUNCE) {
			key_last_state = KEY_PRESSED;
			return KEY_PRESSED;
		}
	}
	
	if (key_last_state == KEY_PRESSED && key_current_state == KEY_RELEASE) {
		key_last_state = KEY_RELEASE;
		debounce_counter = 0;
	}
	return KEY_RELEASE;
}

void USART1_IRQHandler(void) {
	static uint8_t RxState = WAIT_HEAD;
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
		
		// 等待包头.
		if (RxState == WAIT_HEAD && RxFlag == RX_FLAG_RESET) {
			if (USART_ReceiveData(USART1) == 0xFF) { RxState = RECEIVE_DATA; goto LABEL; }
			else RxState = WAIT_HEAD;
		}
		
		// 接收数据包载荷.
		if (RxState == RECEIVE_DATA && RxFlag == RX_FLAG_RESET) {
			static uint8_t DataCount = 0;
			if (DataCount < PACK_LENGTH) {
				RxData[DataCount] = USART_ReceiveData(USART1);
				DataCount++;
			} 
			else { DataCount = 0; RxState = WAIT_TAIL; } // 数据接收完毕，切换状态机到等待包尾态.
		}
		
		// 等待包尾.
		if (RxState == WAIT_TAIL && RxFlag == RX_FLAG_RESET) {
			if (USART_ReceiveData(USART1) == 0xFE) { RxState = WAIT_HEAD; RxFlag = RX_FLAG_SET; }
			else RxState = WAIT_TAIL;
		}

		LABEL:		
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
}
