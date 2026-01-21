/* 包含头文件 ----------------------------------------------------------------*/
#include "../Inc/user_uart.h"
#include "../../Core/Inc/bsp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 私有变量 ------------------------------------------------------------------*/
static UART_DRIVES *uart_drives[UART_NUM];
static uint8_t uart_num = 0;
static uint8_t is_init_loop_event_sign = 0;

/* 函数体 --------------------------------------------------------------------*/
void UART_QueueHandle(void) {
	for (uint8_t uart_index = 0 ; uart_index < uart_num ; uart_index++){
		// 串口接收队列
		if (RBuffer_GetLength(&uart_drives[uart_index]->rx_ringBuffer)) {
			uart_drives[uart_index]->callback(uart_drives[uart_index]);
		}

		//串口发送队列
		if (!Queue_IsEmpty(&uart_drives[uart_index]->tx_queue)) {
			if (uart_drives[uart_index]->status == UART_IDLE) {
				const Node* temp = Queue_Pop(&uart_drives[uart_index]->tx_queue);
				HAL_UART_Transmit_DMA(uart_drives[uart_index]->huart, (uint8_t*)temp->data, temp->len);
				uart_drives[uart_index]->status = UART_SENDING;
			}
		}

	}
}

void UART_Init(UART_DRIVES* user_uart,UART_HandleTypeDef* huart, const UART_Callback callback){
	user_uart->huart = huart;
	user_uart->status = UART_IDLE;
	user_uart->is_buffer_a = 1;
	user_uart->callback = callback;

	uart_drives[uart_num] = user_uart;
	uart_num ++;

	HAL_UARTEx_ReceiveToIdle_DMA(huart, user_uart->rx_buffer_a, UART_BUFFER_SIZE);

	if (is_init_loop_event_sign == 0) {
		loop_event[loop_event_num] = &UART_QueueHandle;
		loop_event_num ++;
		is_init_loop_event_sign = 1;
	}
}

void UART_QSend(UART_DRIVES* user_uart, const char* str) {
    Queue_Push(&user_uart->tx_queue, (char*)str, strlen(str));
}

/* 覆写中断回调函数 -----------------------------------------------------------*/

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	for (uint8_t uart_index = 0 ; uart_index < uart_num ; uart_index++){
	    if (huart == uart_drives[uart_index]->huart) {
	        if (uart_drives[uart_index]->tx_queue.popped) {
	            Queue_FreeNode(uart_drives[uart_index]->tx_queue.popped);
	            uart_drives[uart_index]->tx_queue.popped = NULL;
	            uart_drives[uart_index]->status = UART_IDLE;
	        }
	    }
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	for (uint8_t uart_index = 0 ; uart_index < uart_num ; uart_index++) {
		if (huart == uart_drives[uart_index]->huart) {
			if (uart_drives[uart_index]->is_buffer_a) {
				HAL_UARTEx_ReceiveToIdle_DMA(uart_drives[uart_index]->huart, uart_drives[uart_index]->rx_buffer_b, UART_BUFFER_SIZE);
				__HAL_DMA_DISABLE_IT(uart_drives[uart_index]->huart->hdmarx, DMA_IT_HT);
				RBuffer_Put(&uart_drives[uart_index]->rx_ringBuffer, uart_drives[uart_index]->rx_buffer_a, Size);
				uart_drives[uart_index]->is_buffer_a = 0;
			} else {
				HAL_UARTEx_ReceiveToIdle_DMA(uart_drives[uart_index]->huart, uart_drives[uart_index]->rx_buffer_a, UART_BUFFER_SIZE);
				__HAL_DMA_DISABLE_IT(uart_drives[uart_index]->huart->hdmarx, DMA_IT_HT);
				RBuffer_Put(&uart_drives[uart_index]->rx_ringBuffer, uart_drives[uart_index]->rx_buffer_b, Size);
				uart_drives[uart_index]->is_buffer_a = 1;
			}
		}
	}
}

