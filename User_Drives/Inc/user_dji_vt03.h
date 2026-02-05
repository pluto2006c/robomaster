//
// Created by Pluto on 2026/2/4.
//

#ifndef __USER_DJI_VTO3_H__
#define __USER_DJI_VTO3_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "../../User_Drives/Inc/user_uart.h"
#include "../../Core/Inc/bsp_config.h"
#include <stdbool.h>
/* 宏定义 --------------------------------------------------------------------*/
#define DJI_VT03_BUFFLEN      (21)

/* 类定义 --------------------------------------------------------------------*/
typedef struct
{
    UART_DRIVES* remote_uart;
    uint8_t sof_1;
    uint8_t sof_2;
    uint64_t ch0:11;
    uint64_t ch1:11;
    uint64_t ch2:11;
    uint64_t ch3:11;
    uint64_t mode_sw:2;
    uint64_t pause:1;
    uint64_t fn1:1;
    uint64_t fn2:1;
    uint64_t wheel:11;
    uint64_t trigger:1;

    int16_t mouse_x;
    int16_t mouse_y;
    int16_t mouse_z;
    uint8_t mouse_left:2;
    uint8_t mouse_right:2;
    uint8_t mouse_middle:2;
    uint16_t key;
    uint16_t crc16;
}VT03_DRIVES;

/* 函数声明 ------------------------------------------------------------------*/


void DJI_VT03_Init(VT03_DRIVES* vt03);



#endif //__USER_DJI_VTO3_H__