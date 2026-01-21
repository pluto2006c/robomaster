/* 包含头文件 ----------------------------------------------------------------*/
#include "../Inc/user_led.h"
#include "../../Core/Inc/bsp.h"

/* 函数体 --------------------------------------------------------------------*/
void LED_Init(LED_DRIVES *led,GPIO_TypeDef *GPIO, const uint16_t pin, const uint8_t is_reversal){
  led->GPIO = GPIO;
  led->pin = pin;
  led->is_reversal = is_reversal;
}

void LED_StateSet(const LED_DRIVES *led, const LED_State_TypeDef state){
  if (led->is_reversal) {
    switch (state) {
      case LEDState_ON:
        HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_RESET);  /* LED点亮 */
        break;
      case LEDState_OFF:
        HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_SET);    /* LED熄灭 */
        break;
    }
  }else {
    switch (state) {
      case LEDState_ON:
        HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_SET);    /* LED点亮 */
        break;
      case LEDState_OFF:
        HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_RESET);  /* LED熄灭 */
        break;
    }
  }

}

void LED_On(const LED_DRIVES *led){
  if (led->is_reversal) {
    HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_RESET);  /* LED点亮 */

  }else {
    HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_SET);    /* LED点亮 */
  }
}

void LED_Off(const LED_DRIVES *led){
  if (led->is_reversal) {
    HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_SET);    /* LED熄灭 */
  }else {
    HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_RESET);  /* LED熄灭 */
  }
}

void LED_Toggle(const LED_DRIVES *led){
  HAL_GPIO_TogglePin(led->GPIO, led->pin);
}

LED_State_TypeDef LED_StateGet(const LED_DRIVES *led){
  const GPIO_PinState pinState = HAL_GPIO_ReadPin(led->GPIO, led->pin);

  if (led->is_reversal) {
    switch (pinState) {
      case GPIO_PIN_RESET:
        return LEDState_ON;
      case GPIO_PIN_SET:
        return LEDState_OFF;
    }
  }else {
    switch (pinState) {
      case GPIO_PIN_SET:
        return LEDState_ON;
      case GPIO_PIN_RESET:
        return LEDState_OFF;
    }
  }


  return LEDState_OFF;
}