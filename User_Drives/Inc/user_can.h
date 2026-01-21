#ifndef __USER_CAN_H__
#define __USER_CAN_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "../../Core/Inc/bsp_config.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef void (*CAN_Callback)(void*);

/**
* @brief  CAN Tx message structure definition
* 发送结构体
*/
typedef struct {
    uint8_t IDE;     /*存储IDE扩展标志 */
    uint8_t RTR;     /*存储RTR远程帧标志*/
} CanTxMsg;

/**
* @brief  CAN Rx message structure definition
* 接收结构体
*/
typedef struct {
    uint32_t StdId;  /*存储了报文的标准标识符11位，0-0x7FF. */
    uint32_t ExtId;  /*存储了报文的扩展标识符29位，0-0x1FFFFFFF. */
    uint8_t IDE;     /*存储了IDE扩展标志 */
    uint8_t RTR;     /*存储了RTR远程帧标志*/
    uint8_t DLC;     /*存储了报文数据段的长度，0-8 */
    uint8_t Data[8]; /*存储了报文数据段的内容 */
    uint8_t FMI;     /*存储了本报文是由经过筛选器存储进FIFO的，0-0xFF */
} CanRxMsg;

typedef struct  {
    CAN_HandleTypeDef* hcan;
    CAN_Callback callback;
    CanTxMsg tx_msg;
    CanRxMsg rx_msg;
} CAN_DRIVES;

/* 函数声明 ------------------------------------------------------------------*/
void CAN_Init(CAN_DRIVES* user_can, CAN_HandleTypeDef* hcan, const CAN_Callback callback);
void CAN_Send(const CAN_DRIVES* user_can, const uint32_t id, const uint8_t *data, const uint8_t len);



#endif //__USER_CAN_H__