#pragma once

#include <main.h>
#include "stm32f0xx_hal.h"
#include "stdio.h"
#include <functional>

typedef std::function<void(void)> motionCallback ;
typedef void (*extiCallback)(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim);
typedef void (*timerCallback)(TIM_HandleTypeDef* htim);

class MainApp {
public:
	void setup();
	void loop();

};

void registerCallback(extiCallback ptr);
void registerCallback(timerCallback ptr);
void registerCallback(motionCallback ptr);
void WLEDCallback(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim);
void timerElapsed(TIM_HandleTypeDef* htim);

