/*
 * timeHandle.hpp
 *
 *  Created on: Dec 27, 2023
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef INC_EVENTHANDLE_HPP_
#define INC_EVENTHANDLE_HPP_

#include "main.h"
#include "mainApp.hpp"
#include "controls.h"

extern TIM_HandleTypeDef htim1;


/*
 *  			Class for handling outputs for time based events
 *  define functions which will enable/disable outputs for a certain period of time
 *  define functions for toggling output for a ceratin period of time
 */
class eventHandle{
public:
	static eventHandle& getInstance() {
			static eventHandle instance; // Guaranteed to be destroyed, instantiated on first use
			return instance;
	}

	bool setTimer(void);
	bool stopTimer(void);
	bool outputPeriodActive(void);			//turn on an output for certain period of time
	bool outputPeriodToggle(void);

private:
	eventHandle(){}						//default constructor
	~eventHandle(){}
};

#endif /* INC_EVENTHANDLE_HPP_ */


/*
 * P.S.
 * Available timers are		TIM1(ADVANCED, will use for PWM generation purpose (most probably for buzzer))
 * 							TIM6/7(Will use these for setting up counts and such)
 * 							TIM()
 */
