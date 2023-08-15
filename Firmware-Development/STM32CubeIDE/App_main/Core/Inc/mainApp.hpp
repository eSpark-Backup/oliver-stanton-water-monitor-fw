#pragma once

#include <main.h>
#include "stm32f0xx_hal.h"
#include "stdio.h"

//#ifdef __cplusplus
//extern "C"{
//#endif

typedef void (*extiCallback)(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim);
typedef void (*motionCallback)(void);
typedef void (*timerCallback)(TIM_HandleTypeDef* htim);

class MainApp {
public:
	void setup();
	void loop();

};

void lcdSequence();
void lcdSequence(bool motion);
void motionDetected();
void registerCallback(extiCallback ptr);
void registerCallback(timerCallback ptr);
void registerCallback(motionCallback ptr);
void WLED_callback(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim);
void timerElapsed(TIM_HandleTypeDef* htim);


//#ifdef __cplusplus
//}
//#endif
