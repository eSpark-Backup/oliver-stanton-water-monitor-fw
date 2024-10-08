/*
 * eventHandle.hpp
 *
 *  Created on: Dec 27, 2023
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef INC_EVENTHANDLE_HPP_
#define INC_EVENTHANDLE_HPP_



/*
 *  			Class for handling outputs for time based events
 *  define objects which will enable/disable outputs for a certain period of time
 *         define objects for toggling output for a certain period of time
 */

class EventHandle{
public:
	EventHandle()
	{
		DEBUG_RTT(LOG_WARNING, "EventHandle obj constructor called");
	}
	~EventHandle()
	{
		DEBUG_RTT(LOG_WARNING, "EventHandle obj destructor called");
	}

	virtual void setOutputPattern(int8_t numBlinks, uint16_t timePeriod, ...) = 0;
	virtual void Run(void) = 0;
	virtual void turnOff(void) = 0;

private:
	virtual void setupInit(void) = 0;
	virtual void startPWM(void) = 0;
	virtual void stopPWM(void) = 0;
	virtual void toggle(void) = 0;
	virtual void toggleInfinite(void) = 0;
};



#endif /* INC_EVENTHANDLE_HPP_ */
