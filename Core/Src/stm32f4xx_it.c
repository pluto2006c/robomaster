
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "bsp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern CAN_HandleTypeDef hcan1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern UART_HandleTypeDef huart6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  static uint8_t user_m2006_mode = 0 ;

// //拨弹盘归位后回复控制
  if (get_motor_information(&TP_M2006 , rotor_speed) == 100) {
    user_m2006_mode = 1 ;
  }


  if (user_m2006_mode == 1 && get_motor_information(&TP_M2006 , rotor_speed) < 1 ) {
    DJI_Motor_Init(&TP_M2006, &user_can_1, 1, M2006, Rotor_angle, 10.0f , 0.0f, 3.0f, 10000, 5000);
  }

  if (user_vt03.mode_sw == 0) {
    //单发
    DJI_Motor_Target(&LW_M3508, -7500);
    DJI_Motor_Target(&RW_M3508, 7500);
    //激发模式
    if (user_vt03.trigger == 1) {
      //发射频率计时
      if (user_time_counyer % 28 == 0) {
        //检测是否卡弹
        if (get_motor_information(&TP_M2006 , torque_current) >= 15900) {
          //堵转自动反转
          DJI_Motor_Target(&TP_M2006, angle_ring((uint16_t)((float)get_motor_information(&TP_M2006 , rotor_angle) + 8191)));
        }else {
          DJI_Motor_Target(&TP_M2006, angle_ring((uint16_t)((float)get_motor_information(&TP_M2006 , rotor_angle) - 819.1)));
        }
      }
    }else {
      DJI_Motor_Target(&LW_M3508, 0);
      DJI_Motor_Target(&RW_M3508, 0);
      DJI_Motor_Target(&TP_M2006, get_motor_information(&TP_M2006 , rotor_angle));
    }
  }
  if (user_vt03.mode_sw == 2) {
    //连发
    DJI_Motor_Target(&LW_M3508, -7500);
    DJI_Motor_Target(&RW_M3508, 7500);
    if (user_vt03.trigger == 1) {
      //发射频率计时
      if (user_time_counyer % 5 == 0) {
        //检测是否卡弹
        if (get_motor_information(&TP_M2006 , torque_current) >= 15900) {
          //堵转自动反转
          DJI_Motor_Target(&TP_M2006, angle_ring((uint16_t)((float)get_motor_information(&TP_M2006 , rotor_angle) + 8191)));
        }else {
          DJI_Motor_Target(&TP_M2006, angle_ring((uint16_t)((float)get_motor_information(&TP_M2006 , rotor_angle) - 5*819.1)));
        }
      }
    }else {
        DJI_Motor_Target(&TP_M2006, get_motor_information(&TP_M2006 , rotor_angle));
    }
  }else {
    DJI_Motor_Target(&LW_M3508, 0);
    DJI_Motor_Target(&RW_M3508, 0);
    DJI_Motor_Target(&TP_M2006, get_motor_information(&TP_M2006 , rotor_angle));
  }

//调试代码
  DJI_Motor_Target(&LW_M3508, -7500);
  DJI_Motor_Target(&RW_M3508, 7500);
  if (get_motor_information(&TP_M2006 , torque_current) >= 15900) {
    //堵转自动反转
    DJI_Motor_Target(&TP_M2006, angle_ring((uint16_t)((float)get_motor_information(&TP_M2006 , rotor_angle) + 8191)));
  }else {
    DJI_Motor_Target(&TP_M2006, angle_ring((uint16_t)((float)get_motor_information(&TP_M2006 , rotor_angle) - 819.1)));
  }

  DJI_Motor_Execute(&user_can_1);

  static uint16_t v = 0 ;
  if (user_vt03.fn1 == 1) {
    v = user_vt03.wheel;
  }
      //发送遥控器数据

  uint8_t user_can_2_send_frame[8] = {0};

  user_can_2_send_frame [0] = (uint8_t) (user_vt03.ch3 >> 0);
  user_can_2_send_frame [1] = (uint8_t) (user_vt03.ch3 >> 8);
  user_can_2_send_frame [2] = (uint8_t) (user_vt03.ch2 >> 0);
  user_can_2_send_frame [3] = (uint8_t) (user_vt03.ch2 >> 8);
  user_can_2_send_frame [4] = (uint8_t) (v >> 0);
  user_can_2_send_frame [5] = (uint8_t) (v >> 8);
  user_can_2_send_frame [6] = (uint8_t) (user_vt03.ch0 >> 0);
  user_can_2_send_frame [7] = (uint8_t) (user_vt03.ch0 >> 8);

  CAN_Send(&user_can_2, Re_control_data_ID_1 , user_can_2_send_frame, 8);



      /* USER CODE END SysTick_IRQn 0 */
      HAL_IncTick();
      /* USER CODE BEGIN SysTick_IRQn 1 */

      /* USER CODE END SysTick_IRQn 1 */
  }

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream1 global interrupt.
  */
void DMA2_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream1_IRQn 0 */

  /* USER CODE END DMA2_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
  /* USER CODE BEGIN DMA2_Stream1_IRQn 1 */

  /* USER CODE END DMA2_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream6_IRQn 0 */

  /* USER CODE END DMA2_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
  /* USER CODE BEGIN DMA2_Stream6_IRQn 1 */

  /* USER CODE END DMA2_Stream6_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */

  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
