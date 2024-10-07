/*
 * eventHandle.hpp
 *
 *  Created on: Dec 27, 2023
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef INC_EVENTHANDLE_HPP_
#define INC_EVENTHANDLE_HPP_

#include <stdarg.h>
#include "main.h"
#include "mainApp.hpp"
#include "controls.h"



/*
 *  			Class for handling outputs for time based events
 *  define objects which will enable/disable outputs for a certain period of time
 *         define objects for toggling output for a certain period of time
 */

class EventHandle{
public:
	EventHandle(GPIO_TypeDef* port, uint16_t pin, bool isPWM)
        : port_(port), pin_(pin), isPWM_(isPWM), onPeriodms_(0), offPeriodms_(0), numBlinks_(0), blinkCount_(0), isOn_(false), state_(false) {

        // Initialize GPIO pin
        if(!isPWM_)
        {
                GPIO_InitTypeDef GPIO_InitStruct = {0};
                GPIO_InitStruct.Pin = pin_;
                GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
                GPIO_InitStruct.Pull = GPIO_NOPULL;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
                HAL_GPIO_Init(port_, &GPIO_InitStruct);
        }

        DEBUG_RTT(LOG_WARNING, "EventHandle obj constructor called");
    }
	~EventHandle(){
		delete[] onPeriodms_;
		DEBUG_RTT(LOG_WARNING, "EventHandle obj destructor called");
	}

	bool setTimer(void);
	bool stopTimer(void);
	bool outputPeriodActive(void);			//turn on an output for certain period of time

	bool Init(int8_t numBlinks, uint16_t timePeriod, ...);			// numBlinks < 0 means the event is supposed to run infinitely
	void Run(void);
	void turnOff(void);
	void toggle(void);
	void toggleInfinite(void);

private:
    GPIO_TypeDef* port_;						//GPIO port of output
    uint16_t pin_;								//pin number of output
    bool isPWM_;								//is output controlled by PWM
    uint16_t *onPeriodms_;						//output turn on duration
    uint16_t offPeriodms_;						//output turn off duration
    int8_t numBlinks_;							//How many times you want output to turn on
    uint16_t blinkCount_;						//current number of blink
    bool isOn_;									//is object activated or not
    bool state_;								//obj is currently in runnign or off state
    uint32_t lastToggleTime_;					//Tick() when output was toggled
};



#endif /* INC_EVENTHANDLE_HPP_ */


/*
 * P.S.
 * Available timers are		TIM1(ADVANCED, will use for PWM generation purpose (most probably for buzzer))
 * 							TIM6/7(Will use these for setting up counts and such)
 *
 */
