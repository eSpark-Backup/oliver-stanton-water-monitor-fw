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

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_RS_Pin GPIO_PIN_6
#define LCD_RS_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define VIOUT1_Pin GPIO_PIN_0
#define VIOUT1_GPIO_Port GPIOB
#define SW2_Pin GPIO_PIN_2
#define SW2_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_7
#define LCD_EN_GPIO_Port GPIOE
#define PWM_CNTRL_Pin GPIO_PIN_12
#define PWM_CNTRL_GPIO_Port GPIOE
#define DB4_Pin GPIO_PIN_12
#define DB4_GPIO_Port GPIOB
#define DB5_Pin GPIO_PIN_13
#define DB5_GPIO_Port GPIOB
#define DB6_Pin GPIO_PIN_14
#define DB6_GPIO_Port GPIOB
#define DB7_Pin GPIO_PIN_15
#define DB7_GPIO_Port GPIOB
#define SHDN_Pin GPIO_PIN_6
#define SHDN_GPIO_Port GPIOC
#define IOC9_Pin GPIO_PIN_9
#define IOC9_GPIO_Port GPIOC
#define Enable_Pin GPIO_PIN_12
#define Enable_GPIO_Port GPIOC
#define W_LED_CTRL_Pin GPIO_PIN_4
#define W_LED_CTRL_GPIO_Port GPIOD
#define SW3_Pin GPIO_PIN_3
#define SW3_GPIO_Port GPIOB
#define SW3_EXTI_IRQn EXTI2_3_IRQn
#define SW4_Pin GPIO_PIN_4
#define SW4_GPIO_Port GPIOB
#define SW5_Pin GPIO_PIN_5
#define SW5_GPIO_Port GPIOB
#define SW6_Pin GPIO_PIN_6
#define SW6_GPIO_Port GPIOB
#define INB_Pin GPIO_PIN_7
#define INB_GPIO_Port GPIOB
#define INA_Pin GPIO_PIN_8
#define INA_GPIO_Port GPIOB
#define PIR_OUT_Pin GPIO_PIN_9
#define PIR_OUT_GPIO_Port GPIOB
#define PIR_OUT_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
