#ifndef __USER_LED_H__
#define __USER_LED_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
  LEDState_OFF = 0,
  LEDState_ON = 1,
} LED_State_TypeDef;

typedef struct {
  GPIO_TypeDef * GPIO;
  uint16_t pin;
  uint8_t is_reversal;
} LED_DRIVES;


/* 函数声明 ------------------------------------------------------------------*/
void LED_Init(LED_DRIVES *led, GPIO_TypeDef *GPIO, const uint16_t pin, const uint8_t is_reversal);
void LED_StateSet(const LED_DRIVES *led, const LED_State_TypeDef state);
void LED_On(const LED_DRIVES *led);
void LED_Off(const LED_DRIVES *led);
void LED_Toggle(const LED_DRIVES *led);
LED_State_TypeDef LED_StateGet(const LED_DRIVES *led);


#endif  // __USER_LED_H__