/* 包含头文件 ----------------------------------------------------------------*/
#include "../Inc/user_ring_buffe.h"

/* 函数体 --------------------------------------------------------------------*/

/**
* @brief 增加读索引
* @param buffer
* @param length 要增加的长度
*/
void RBuffer_AddReadIndex(RingBuffer* buffer, const uint16_t length) {
    buffer->readIndex += length;
    buffer->readIndex %= RING_BUFFER_SIZE;
}


/**
* @brief 读取第i位数据 超过缓存区长度自动循环
* @param buffer
* @param i 要读取的数据索引
*/
uint8_t RBuffer_Read(const RingBuffer* buffer, const uint16_t i) {
    return buffer->buffer[i % RING_BUFFER_SIZE];
}


/**
* @brief 计算未处理的数据长度
* @return 未处理的数据长度
* @retval 0 缓冲区为空
* @retval 1~BUFFER_SIZE-1 未处理的数据长度
* @retval BUFFER_SIZE 缓冲区已满
*/
uint16_t RBuffer_GetDis(const uint16_t statIndex, const uint16_t tailIndex) {
    return (tailIndex + RING_BUFFER_SIZE - statIndex) % RING_BUFFER_SIZE;
}


/**
* @brief 计算未处理的数据长度
* @return 未处理的数据长度
* @retval 0 缓冲区为空
* @retval 1~BUFFER_SIZE-1 未处理的数据长度
* @retval BUFFER_SIZE 缓冲区已满
*/
uint16_t RBuffer_GetLength(const RingBuffer* buffer) {
    return (buffer->writeIndex + RING_BUFFER_SIZE - buffer->readIndex) % RING_BUFFER_SIZE;
}


/**
* @brief 计算缓冲区剩余空间
* @return 剩余空间
* @retval 0 缓冲区已满
* @retval 1~BUFFER_SIZE-1 剩余空间
* @retval BUFFER_SIZE 缓冲区为空
*/
uint16_t RBuffer_GetRemain(const RingBuffer* buffer) {
    return RING_BUFFER_SIZE - RBuffer_GetLength(buffer);
}


/**
* @brief 向缓冲区写入数据
* @param buffer
* @param data 要写入的数据指针
* @param length 要写入的数据长度
* @return 写入的数据长度
*/
uint16_t RBuffer_Put(RingBuffer* buffer, const uint8_t *data, const uint8_t length) {
    // 如果缓冲区不足 则不写入数据
    if (RBuffer_GetRemain(buffer) < length) {
        // Error_Handler();
        return 0;
    }
    // 将数据写入缓冲区
    if (buffer->writeIndex + length < RING_BUFFER_SIZE) {
        memcpy(buffer->buffer + buffer->writeIndex, data, length);
        buffer->writeIndex += length;
    } else {
        uint8_t firstLength = RING_BUFFER_SIZE - buffer->writeIndex;
        memcpy(buffer->buffer + buffer->writeIndex, data, firstLength);
        memcpy(buffer->buffer, data + firstLength, length - firstLength);
        buffer->writeIndex = length - firstLength;
    }
    return length;
}

/**
* @brief 获取一段带有头尾的数据帧
* @param buffer
* @param message
* @param head
* @param tail
* @return 获取的数据帧长度
* @retval 0 没有获取到数据帧
*/
uint16_t RBuffer_GetWithHT(RingBuffer* buffer, uint8_t *message, const char* head, const char* tail) {
    const uint16_t buffLen = RBuffer_GetLength(buffer);
    if (buffLen >= (uint32_t)strlen(head) + (uint32_t)strlen(tail)) {
        for (int32_t start_i = (int32_t)buffLen - (int32_t)(uint32_t)strlen(head) - (int32_t)(uint32_t)strlen(tail); start_i >= 0; start_i--){
            uint8_t head_sign = 1;
            for (uint32_t x = 0; x <= strlen(head) - 1; x++) {
                head_sign &= RBuffer_Read(buffer,buffer->readIndex+start_i+x) == *(head+x);
            }
            if (head_sign) {
                for (uint32_t tail_i = strlen(head); tail_i <= buffLen - (uint32_t)strlen(tail) - start_i; tail_i++) {
                    uint8_t tail_sign = 1;
                    for (uint32_t x = 0; x <= strlen(tail) - 1; x++) {
                        tail_sign &= RBuffer_Read(buffer,buffer->readIndex+start_i+tail_i+x) == *(tail+x);
                    }
                    if (tail_sign) {
                        if (buffer->readIndex+start_i+tail_i+strlen(tail) >= RING_BUFFER_SIZE) {
                            memcpy(message,buffer->buffer+buffer->readIndex+start_i, RING_BUFFER_SIZE-(buffer->readIndex+start_i));
                            memcpy(message+RING_BUFFER_SIZE-(buffer->readIndex+start_i),buffer->buffer, buffer->readIndex+start_i+tail_i+strlen(tail)-RING_BUFFER_SIZE);
                        }else {
                            memcpy(message,buffer->buffer+buffer->readIndex+start_i, tail_i+strlen(tail));
                        }
                        RBuffer_AddReadIndex(buffer,start_i+tail_i+strlen(tail));
                        return tail_i+strlen(tail);
                    }
                }
            }
        }
    }
    return 0;
}

/**
* @brief 通过数据帧的长度和帧头获取数据帧
* @param buffer
* @param message
* @param head
* @param len
* @return 是否获取到数据帧
* @retval 0 没有获取到数据帧
*/
uint16_t RBuffer_GetWithHLen(RingBuffer* buffer, uint8_t *message, const char* head, const uint16_t len) {
    const uint16_t buffLen = RBuffer_GetLength(buffer);
    if (buffLen >= len) {
        for (int32_t start_i = (int32_t)buffLen - len; start_i >= 0; start_i--){
            uint8_t head_sign = 1;
            for (uint32_t x = 0; x <= strlen(head) - 1; x++) {
                head_sign &= RBuffer_Read(buffer,buffer->readIndex+start_i+x) == *(head+x);
            }
            if (head_sign) {
                if (buffer->readIndex+start_i+len >= RING_BUFFER_SIZE) {
                    memcpy(message,buffer->buffer+buffer->readIndex+start_i, RING_BUFFER_SIZE-(buffer->readIndex+start_i));
                    memcpy(message+RING_BUFFER_SIZE-(buffer->readIndex+start_i),buffer->buffer, buffer->readIndex+start_i+len-RING_BUFFER_SIZE);
                }else {
                    memcpy(message,buffer->buffer+buffer->readIndex+start_i, len);
                }
                RBuffer_AddReadIndex(buffer,start_i+len);
                return len;
            }
        }
    }
    return 0;
}


/**
* @brief 通过数据帧的长度获取数据帧
* @param buffer
* @param message
* @param len
* @return 是否获取到数据帧
* @retval 0 没有获取到数据帧
*/
uint16_t RBuffer_GetWithLen(RingBuffer* buffer, uint8_t *message, const uint16_t len) {
    const uint16_t buffLen = RBuffer_GetLength(buffer);
    if (buffLen >= len) {
        if (buffer->readIndex+len >= RING_BUFFER_SIZE) {
            memcpy(message,buffer->buffer+buffer->readIndex, RING_BUFFER_SIZE-(buffer->readIndex));
            memcpy(message+RING_BUFFER_SIZE-(buffer->readIndex),buffer->buffer, buffer->readIndex+len-RING_BUFFER_SIZE);
        } else {
            memcpy(message,buffer->buffer+buffer->readIndex, len);
        }
        RBuffer_AddReadIndex(buffer,len);
        return len;
    } else {
        return 0;
    }
}

/**
* @brief 获取从帧头到下一个帧头的数据帧
* @param buffer
* @param message
* @param head
* @return 获取的数据帧长度
* @retval 0 没有获取到数据帧
*/
uint16_t RBuffer_GetWithH(RingBuffer* buffer, uint8_t *message, const char* head) {
    const uint16_t buffLen = RBuffer_GetLength(buffer);
    if (buffLen >= 2 * (uint32_t)strlen(head)) {
        for (int32_t start_i = 0; start_i <= (int32_t)buffLen - (int32_t)(uint32_t)strlen(head); start_i++){
            uint8_t head_sign = 1;
            for (uint32_t x = 0; x <= strlen(head) - 1; x++) {
                head_sign &= RBuffer_Read(buffer,buffer->readIndex+start_i+x) == *(head+x);
            }
            if (head_sign) {
                for (uint32_t tail_i = strlen(head); tail_i <= buffLen - (uint32_t)strlen(head) - start_i; tail_i++) {
                    uint8_t tail_sign = 1;
                    for (uint32_t x = 0; x <= strlen(head) - 1; x++) {
                        tail_sign &= RBuffer_Read(buffer,buffer->readIndex+start_i+tail_i+x) == *(head+x);
                    }
                    if (tail_sign) {
                        if (buffer->readIndex+start_i+tail_i >= RING_BUFFER_SIZE) {
                            memcpy(message,buffer->buffer+buffer->readIndex+start_i, RING_BUFFER_SIZE-(buffer->readIndex+start_i));
                            memcpy(message+RING_BUFFER_SIZE-(buffer->readIndex+start_i),buffer->buffer, buffer->readIndex+start_i+tail_i-RING_BUFFER_SIZE);
                        }else {
                            memcpy(message,buffer->buffer+buffer->readIndex+start_i, tail_i);
                        }
                        RBuffer_AddReadIndex(buffer,start_i+tail_i);
                        return tail_i;
                    }
                }
            }
        }
    }
    return 0;
}

/**
* @brief 获取从帧尾到下一个帧尾的数据帧
* @param buffer
* @param message
* @param tail
* @return 获取的数据帧长度
* @retval 0 没有获取到数据帧
*/
uint16_t RBuffer_GetWithT(RingBuffer* buffer, uint8_t *message, const char* tail) {
    const uint16_t buffLen = RBuffer_GetLength(buffer);
    for (uint32_t tail_i = 0; tail_i <= buffLen - (uint32_t)strlen(tail); tail_i++) {
        uint8_t tail_sign = 1;
        for (uint32_t x = 0; x <= strlen(tail) - 1; x++) {
            tail_sign &= RBuffer_Read(buffer,buffer->readIndex+tail_i+x) == *(tail+x);
        }
        if (tail_sign) {
            if (buffer->readIndex+tail_i+strlen(tail) >= RING_BUFFER_SIZE) {
                memcpy(message,buffer->buffer+buffer->readIndex, RING_BUFFER_SIZE-(buffer->readIndex));
                memcpy(message+RING_BUFFER_SIZE-(buffer->readIndex),buffer->buffer, buffer->readIndex+tail_i+strlen(tail)-RING_BUFFER_SIZE);
            }else {
                memcpy(message,buffer->buffer+buffer->readIndex, tail_i+strlen(tail));
            }
            RBuffer_AddReadIndex(buffer,+tail_i+strlen(tail));
            return tail_i+strlen(tail);
        }
    }
    return 0;
}

