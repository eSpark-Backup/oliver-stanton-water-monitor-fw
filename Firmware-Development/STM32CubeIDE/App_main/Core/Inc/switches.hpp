/*
 * switches.hpp
 *
 *  Created on: Aug 15, 2023
 *      Author: eSpark
 */

#pragma once
#include "stm32f0xx_hal.h"

typedef enum{
	SW2,
	SW3,
	SW4,
	SW5,
	SW6,
	SW7
}SWITCH;
class Switches {
public:
	bool getState(SWITCH);
};
