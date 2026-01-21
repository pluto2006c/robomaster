#ifndef __USER_DELAY_H__
#define __USER_DELAY_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "../../Core/Inc/bsp_config.h"

/* 扩展变量 ------------------------------------------------------------------*/
extern uint32_t TimeFlag;

/* 函数声明 ------------------------------------------------------------------*/
void delay_init(const TIM_HandleTypeDef *htim);
void delay_us(const uint32_t us);
void delay_ms(const uint32_t ms);
void delay_s(const uint32_t s);

#endif //__USER_DELAY_H__
