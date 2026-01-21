#ifndef __USER_DJI_BUS_H__
#define __USER_DJI_BUS_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "../../Core/Inc/bsp_config.h"

/* 宏定义 --------------------------------------------------------------------*/
#define DBUS_BUFLEN      (18)

/* 类型定义 ------------------------------------------------------------------*/
typedef struct  {
    UART_HandleTypeDef* huart;
    uint8_t is_update;
    int16_t ch0;
    int16_t ch1;
    int16_t ch2;
    int16_t ch3;
    int16_t roll;
    uint8_t sw1;
    uint8_t sw2;
} DBUS_DRIVES;

typedef long long LL;

/* 函数声明 ------------------------------------------------------------------*/
void DBUS_Init(DBUS_DRIVES* user_dbus,UART_HandleTypeDef* huart);

void DBUS_receive_handler(UART_HandleTypeDef *huart);

#endif //__USER_DJI_BUS_H__
