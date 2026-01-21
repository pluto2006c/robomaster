/* 包含头文件 ----------------------------------------------------------------*/
#include "../../Core/Inc/bsp.h"
#include "../Inc/user_delay.h"
/* 私有变量 ------------------------------------------------------------------*/
static TIM_HandleTypeDef user_tick_timer;

/* 函数体 --------------------------------------------------------------------*/
void delay_init(const TIM_HandleTypeDef *htim) {

}


void delay_us(const uint32_t us) {
    uint32_t startFlag = TimeFlag;
    uint32_t startTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
    while (startFlag != TimeFlag) {
        startFlag = TimeFlag;
        startTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
    }

    uint32_t currentFlag;
    uint32_t currentTime;

    do {
        currentFlag = TimeFlag;
        currentTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
        while (currentFlag != TimeFlag) {
            currentFlag = TimeFlag;
            currentTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
        }

        LOOP_EVENT_Handle();


    }while (((currentFlag - startFlag) * 10000 + (currentTime - startTime) * 5 ) * 100 < us);
}

void delay_ms(const uint32_t ms) {
    uint32_t startFlag = TimeFlag;
    uint32_t startTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
    while (startFlag != TimeFlag) {
        startFlag = TimeFlag;
        startTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
    }

    uint32_t currentFlag;
    uint32_t currentTime;

    do {
        currentFlag = TimeFlag;
        currentTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
        while (currentFlag != TimeFlag) {
            currentFlag = TimeFlag;
            currentTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
        }

        for (uint16_t i = 0; i < 10; i++) {
            LOOP_EVENT_Handle();
        }

    }while ((currentFlag - startFlag) * 10000 + (currentTime - startTime) * 5 < ms * 10);
}

void delay_s(const uint32_t s) {
    uint32_t startFlag = TimeFlag;
    uint32_t startTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
    while (startFlag != TimeFlag) {
        startFlag = TimeFlag;
        startTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
    }

    uint32_t currentFlag;
    uint32_t currentTime;

    do {
        currentFlag = TimeFlag;
        currentTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
        while (currentFlag != TimeFlag) {
            currentFlag = TimeFlag;
            currentTime = __HAL_TIM_GET_COUNTER(&user_tick_timer);
        }

        for (uint16_t i = 0; i < 1000 * 10; i++) {
            LOOP_EVENT_Handle();
        }

    }while ((currentFlag - startFlag) * 10000 + (currentTime - startTime) * 5 < s * 10 * 1000);
}
