#ifndef __USER_QUEUE_H__
#define __USER_QUEUE_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stdint.h>

/* 类型定义 ------------------------------------------------------------------*/

// 节点
typedef struct Node {
    void* data;
    uint16_t len;
    uint16_t address;
    struct Node* next;
} Node;

// 队列
typedef struct {
    Node* front;
    Node* rear;
    Node* popped;
    int size;
} Queue;

/* 函数声明 ------------------------------------------------------------------*/

void Queue_Push(Queue* queue, const void* data, const uint16_t len);
void Queue_PushWA(Queue* queue, const void* data, const uint16_t len, const uint16_t address);
Node* Queue_Pop(Queue* queue);

uint8_t Queue_IsEmpty(Queue* queue);
uint16_t Queue_GetSize(Queue* queue);
void Queue_FreeNode(Node* node);
void Queue_Free(Queue* queue);


#endif //__USER_QUEUE_H__