
#ifndef mainApp_H_
#define mainApp_H_

#include "stm32f0xx_hal.h"
#include "stdio.h"
#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef void (*funcPtrEXTI)(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim);

typedef void (*funcPtrTimer)(TIM_HandleTypeDef* htim);

void program_init();
void lcd_sequence();
void registerHandler(funcPtrEXTI ptr);
void registerHandler1(funcPtrTimer ptr);
void handler(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim);
void handler1(TIM_HandleTypeDef* htim);
void WLED_func(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim);
void timerElapsed_func(TIM_HandleTypeDef* htim);

#ifdef __cplusplus
}
#endif

#endif /* mainApp_H_ */
