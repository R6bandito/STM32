#include "key_more.h"

void Key_Init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitTypeDef GPIOB_InitStructure;

  GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_1;
  GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIOB_InitStructure);
}

uint8_t Get_Key_Num(void) {
  static uint8_t last_key1_state = KEY_UNPRESS;
  static uint8_t last_key2_state = KEY_UNPRESS;

  //按键1，2状态（初始状态为松开）.
  static uint32_t key1_state = KEY_UNPRESS;
  static uint32_t key2_state = KEY_UNPRESS;

  //按键1，2的稳定计数.
  static uint32_t key1_stable_count = 0;
  static uint32_t key2_stable_count = 0;

  uint32_t key1_samples = 0;  //按键1多次采样结果.
  uint32_t key2_samples = 0;  //按键2多次采样结果.

  //多次采样.
  for(uint32_t i = 0 ; i < KEY_SAMPLES ; ++i) {
    key1_samples |= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) << i;
    key2_samples |= GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) << i;
  }

  if ((key1_samples == 0) && (key1_stable_count) < KEY_SAMPLES) {
    key1_stable_count++;
  }
  else if ((key1_samples != 0) && (key1_stable_count > 0)) {
    key1_stable_count--;
  }

  if ((key2_samples == 0) && (key2_stable_count < KEY_SAMPLES)) {
    key2_stable_count++;
  }
  else if ((key2_samples != 0) && (key2_stable_count) > 0) {
    key2_stable_count--;
  }

  //确认按键1状态.
  if (key1_stable_count == KEY_SAMPLES) {
    key1_state = KEY_PRESSED;
  }
  else if (key1_stable_count == 0) {
    key1_state = KEY_UNPRESS;
    last_key1_state = KEY_UNPRESS;
  }

  //确认按键2状态.
  if (key2_stable_count == KEY_SAMPLES) {
    key2_state = KEY_PRESSED;
  }
  else if (key2_stable_count == 0) {
    key2_state = KEY_UNPRESS;
    last_key2_state = KEY_UNPRESS;
  }

  if (key1_state == KEY_PRESSED && last_key1_state == KEY_UNPRESS) {
    last_key1_state = KEY_PRESSED;
    return KEY1_PRESSED;
  }
  if (key2_state == KEY_PRESSED && last_key2_state == KEY_UNPRESS) {
    last_key2_state = KEY_PRESSED;
    return KEY2_PRESSED;
  }

  return 0;
}

