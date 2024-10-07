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
	EventHandle()
        {
                DEBUG_RTT(LOG_WARNING, "EventHandle obj constructor called");
        }
	~EventHandle()
        {
		DEBUG_RTT(LOG_WARNING, "EventHandle obj destructor called");
	}

	virtual void setOutputPattern(void);
	void Run(void);
	void turnOff(void);

private:
	virtual void setupInit(void);
        virtual void startPWM(void);
	virtual void stopPWM(void);
        virtual void toggle(void);
	virtual void toggleInfinite(void);
};



#endif /* INC_EVENTHANDLE_HPP_ */
