/*
 * switches.hpp
 *
 *  Created on: Aug 15, 2023
 *      Author: eSpark
 */

#pragma once
#include "stm32f0xx_hal.h"

class Switches {
public:
	bool getState(GPIO_TypeDef * GPIO_Port, uint16_t GPIO_Pin);
	void setState(GPIO_TypeDef * GPIO_Port, uint16_t GPIO_Pin,GPIO_PinState state);
};
