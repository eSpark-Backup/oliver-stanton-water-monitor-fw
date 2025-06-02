/*
 * eventHandle.hpp
 *
 *  Created on: Dec 27, 2023
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef _EVENTHANDLE_HPP_
#define _EVENTHANDLE_HPP_


#include "stm32f0xx_hal.h"
#include "_DEBUG_.h"


/*
 *  			Class for handling outputs for time based events
 *  define objects which will enable/disable outputs for a certain period of time
 *         define objects for toggling output for a certain period of time
 */
typedef enum
{
	IndWLED,
	IndRGB,
	IndBUZZER,
}	IndicatorType;

class EventHandle{
public:
	EventHandle(IndicatorType actuatorType) : ActuatorType(actuatorType)
	{
		DEBUG_RTT(LOG_WARNING, "EventHandle obj constructor called");
	}
	~EventHandle()
	{
		DEBUG_RTT(LOG_WARNING, "EventHandle obj destructor called");
	}

	virtual void setupInit(void) = 0;
	virtual void setOutputPattern(int8_t numBlinks, uint16_t timePeriod, ...);
	virtual void Run(void);
	virtual void turnOff(void);

private:
	virtual void setOutput(void) = 0;
	virtual void stopOutput(void) = 0;

protected:
	virtual void toggle(void);
	virtual void toggleInfinite(void);

protected:
	IndicatorType ActuatorType;								//is output controlled by PWM
    uint16_t *onPeriodms_ = nullptr;						//output turn on duration
    uint16_t offPeriodms_ = 0;						//output turn off duration
    int8_t numBlinks_ = 0;							//How many times you want output to turn on
    uint16_t blinkCount_ = 0;						//current number of blink
    bool isOn_ = false;									//is object activated or not
    bool state_ = false;								//obj is currently in runnign or off state
    uint32_t lastToggleTime_ = false;					//Tick() when output was toggled

};


extern EventHandle* Buzzzer;
extern EventHandle* WLED1;


#endif /* _EVENTHANDLE_HPP_ */
