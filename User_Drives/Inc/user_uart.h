#ifndef __USER_UART_H__
#define __USER_UART_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "../../Core/Inc/bsp_config.h"
#include "../../User_Architect/Inc/user_queue.h"
#include "../../User_Architect/Inc/user_ring_buffe.h"

/* 类型定义 ------------------------------------------------------------------*/
enum UART_Status {
    UART_IDLE = 0,
    UART_SENDING = 1,
};

typedef void (*UART_Callback)(void*);

typedef struct  {
    UART_HandleTypeDef* huart;
    enum UART_Status status;
    uint8_t is_buffer_a;
    uint8_t rx_buffer_a[UART_BUFFER_SIZE];
    uint8_t rx_buffer_b[UART_BUFFER_SIZE];
    RingBuffer rx_ringBuffer;
    Queue tx_queue;
    UART_Callback callback;
} UART_DRIVES;

/* 函数声明 ------------------------------------------------------------------*/
void UART_Init(UART_DRIVES* user_uart,UART_HandleTypeDef* huart, const UART_Callback callback);
void UART_QSend(UART_DRIVES* user_uart, const char* str);

#endif //__USER_UART_H__
