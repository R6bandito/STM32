#include "key_git.h"

void KeyInit(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  //GPIOB相关引脚初始化.
  GPIO_InitTypeDef GPIOB_InitStructure;
  GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入.
  GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_1;
  GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIOB_InitStructure);
}

uint8_t GetKeyNum(void) {
  //记录上次按键的状态.
  static uint8_t last_key1_state = KEY_UNPRESS;  //默认逻辑电平为按键松开(1).
  static uint8_t last_key2_state = KEY_UNPRESS;

  uint8_t key1_state , key2_state;
  key1_state = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
  key2_state = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);

  if (key1_state == KEY_PRESSED && last_key1_state == KEY_UNPRESS) {
    Delay_ms(KEY_DELAY);  //延时一段时间
    //再次采样.
    key1_state = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
    if (key1_state == KEY_PRESSED && last_key1_state == KEY_UNPRESS) {
      last_key1_state = KEY_PRESSED;
      return KEY1_PRESSED;
    }
  }
  else if (key1_state != KEY_PRESSED && last_key1_state != KEY_UNPRESS) {
    last_key1_state = KEY_UNPRESS;
  }

  if (key2_state == KEY_PRESSED && last_key2_state == KEY_UNPRESS) {
    Delay_ms(KEY_DELAY);
    key2_state = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
    if (key2_state == KEY_PRESSED && last_key2_state == KEY_UNPRESS) {
      last_key2_state = KEY_PRESSED;
      return KEY2_PRESSED;
    }
  }
  else if (key2_state != KEY_PRESSED && last_key2_state != KEY_UNPRESS) {
    last_key2_state = KEY_UNPRESS;
  }

  return 0;
}

