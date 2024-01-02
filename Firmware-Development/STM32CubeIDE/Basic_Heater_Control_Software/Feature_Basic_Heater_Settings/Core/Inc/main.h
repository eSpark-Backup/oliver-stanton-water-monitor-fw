/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
static TIM_HandleTypeDef htim1;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_6
#define LED2_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_2
#define SW2_GPIO_Port GPIOB
#define SW2_EXTI_IRQn EXTI2_3_IRQn
#define LED3_Pin GPIO_PIN_8
#define LED3_GPIO_Port GPIOE
#define MEM_SAVER_Pin GPIO_PIN_10
#define MEM_SAVER_GPIO_Port GPIOE
#define PWM_CNTRL_Pin GPIO_PIN_12
#define PWM_CNTRL_GPIO_Port GPIOE
#define SW7_Pin GPIO_PIN_8
#define SW7_GPIO_Port GPIOD
#define SW7_EXTI_IRQn EXTI4_15_IRQn
#define AuxHeater_Pin GPIO_PIN_9
#define AuxHeater_GPIO_Port GPIOD
#define SHDN_shutdown_Pin GPIO_PIN_6
#define SHDN_shutdown_GPIO_Port GPIOC
#define AUTO_FILL_Pin GPIO_PIN_7
#define AUTO_FILL_GPIO_Port GPIOC
#define MainHeater_Pin GPIO_PIN_9
#define MainHeater_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define PUMPS_Control_Pin GPIO_PIN_12
#define PUMPS_Control_GPIO_Port GPIOC
#define RO_SAVER_Pin GPIO_PIN_2
#define RO_SAVER_GPIO_Port GPIOD
#define LEVEL_SENSOR_Pin GPIO_PIN_3
#define LEVEL_SENSOR_GPIO_Port GPIOD
#define W_LED_CTRL_Pin GPIO_PIN_4
#define W_LED_CTRL_GPIO_Port GPIOD
#define SW3_Pin GPIO_PIN_3
#define SW3_GPIO_Port GPIOB
#define SW3_EXTI_IRQn EXTI2_3_IRQn
#define SW4_Pin GPIO_PIN_4
#define SW4_GPIO_Port GPIOB
#define SW4_EXTI_IRQn EXTI4_15_IRQn
#define SW5_Pin GPIO_PIN_5
#define SW5_GPIO_Port GPIOB
#define SW5_EXTI_IRQn EXTI4_15_IRQn
#define SW6_Pin GPIO_PIN_6
#define SW6_GPIO_Port GPIOB
#define SW6_EXTI_IRQn EXTI4_15_IRQn
#define _PUMP2_Pin GPIO_PIN_7
#define _PUMP2_GPIO_Port GPIOB
#define _PUMP1_Pin GPIO_PIN_8
#define _PUMP1_GPIO_Port GPIOB
#define PIR_Pin GPIO_PIN_9
#define PIR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
