//
// Created by Pluto on 2026/2/4.
//

#ifndef DJI_A_BOARD_USER_DJI_REMOTE_H
#define DJI_A_BOARD_USER_DJI_REMOTE_H

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "../../User_Drives/Inc/user_uart.h"
#include "../../Core/Inc/bsp_config.h"
#include <stdbool.h>
/* 宏定义 --------------------------------------------------------------------*/
#define remote_BUFLEN      (21)

/* 类定义 --------------------------------------------------------------------*/
typedef __packed struct
{
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
}remote_data_t;

/* 函数声明 ------------------------------------------------------------------*/
#include "../../User_Drives/Inc/user_uart.h"
extern UART_DRIVES remote_data_uart;
extern remote_data_t remote;
void user_remote_uart_callback(void * user_uart);
void REMOTE_Init();
bool verify_crc16_check_sum(uint8_t *p_msg, uint16_t len);
uint16_t parse_11bit(const uint8_t *data, int offset_bits);
#endif //DJI_A_BOARD_USER_DJI_REMOTE_H