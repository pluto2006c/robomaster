/* 包含头文件 ----------------------------------------------------------------*/
#include "../Inc/user_queue.h"
#include <stdlib.h>
#include <string.h>

/* 函数体 --------------------------------------------------------------------*/

/**
 * @brief  入队操作
 * @param  queue: 队列指针
 * @param  data: 数据指针
 * @param  len: 数据长度
 * @retval None
 */
void Queue_Push(Queue* queue, const void* data, const uint16_t len) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    
    void* mem = malloc(len);
    memcpy(mem, data, len);

    newNode->data = mem;
    newNode->len = len;
    newNode->next = NULL;
    newNode->address = 0;
    
    if (queue->rear == NULL) {
        // 队列为空
        queue->front = queue->rear = newNode;
    } else {
        // 队列不为空
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    
    queue->size++;
}


/**
 * @brief  入队操作
 * @param  queue: 队列指针
 * @param  data: 数据指针
 * @param  len: 数据长度
 * @param  address: 传输目的地
 * @retval None
 */
void Queue_PushWA(Queue* queue, const void* data, const uint16_t len, const uint16_t address) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    void* mem = malloc(len);
    memcpy(mem, data, len);

    newNode->data = mem;
    newNode->len = len;
    newNode->next = NULL;
    newNode->address = address;

    if (queue->rear == NULL) {
        // 队列为空
        queue->front = queue->rear = newNode;
    } else {
        // 队列不为空
        queue->rear->next = newNode;
        queue->rear = newNode;
    }

    queue->size++;
}

/**
 * @brief  出队操作
 * @param  queue: 队列指针
 * @retval Node*: 出队的节点指针，队列为空时返回NULL
 */
Node* Queue_Pop(Queue* queue) {
    if (queue->front == NULL) {
        return NULL; // 队列为空
    }
    
    queue->popped = queue->front;
    queue->front = queue->front->next;
    
    if (queue->front == NULL) {
        // 队列变为空
        queue->rear = NULL;
    }

    queue->size--;
    return queue->popped;
}

/**
 * @brief  检查队列是否为空
 * @param  queue: 队列指针
 * @retval int: 1表示为空，0表示不为空
 */
uint8_t Queue_IsEmpty(Queue* queue) {
    return queue->front == NULL;
}

/**
 * @brief  获取队列大小
 * @param  queue: 队列指针
 * @retval int: 队列中元素的数量
 */
uint16_t Queue_GetSize(Queue* queue) {
    return queue->size;
}

/**
 * @brief  释放节点内存
 * @param node 节点指针
 * @retval None
 */
void Queue_FreeNode(Node* node) {
    free(node->data);
    free(node);
}

/**
 * @brief  释放队列内存
 * @param  queue: 队列指针
 * @retval None
 */
void Queue_Free(Queue* queue) {
    while (!Queue_IsEmpty(queue)) {
        Node* node = Queue_Pop(queue);
        Queue_FreeNode(node);
    }
}

