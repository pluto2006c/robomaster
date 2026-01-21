#ifndef __USER_RING_BUFFER_H__
#define __USER_RING_BUFFER_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>

/* 类型定义 ------------------------------------------------------------------*/
#define RING_BUFFER_SIZE 1024
typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    uint16_t readIndex;
    uint16_t writeIndex;
}RingBuffer;

/* 宏定义 --------------------------------------------------------------------*/

/* 扩展变量 ------------------------------------------------------------------*/

/* 函数声明 ------------------------------------------------------------------*/

uint16_t RBuffer_GetLength(const RingBuffer* buffer);
uint16_t RBuffer_Put(RingBuffer* buffer, const uint8_t *data, uint8_t length);
uint16_t RBuffer_GetWithHT(RingBuffer* buffer, uint8_t *message, const char* head, const char* tail);
uint16_t RBuffer_GetWithHLen(RingBuffer* buffer, uint8_t *message, const char* head, const uint16_t len);
uint16_t RBuffer_GetWithLen(RingBuffer* buffer, uint8_t *message, const uint16_t len);
uint16_t RBuffer_GetWithH(RingBuffer* buffer, uint8_t *message, const char* head);
uint16_t RBuffer_GetWithT(RingBuffer* buffer, uint8_t *message, const char* tail);


#endif //__USER_RING_BUFFER_H__