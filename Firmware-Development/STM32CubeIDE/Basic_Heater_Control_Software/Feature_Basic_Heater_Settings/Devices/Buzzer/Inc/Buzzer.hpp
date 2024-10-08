/*
 * Buzzer.hpp
 *
 *	Created on: Oct 07, 2024
 *  Author: Nehal Abdul Rehman
 */
#pragma once

#ifndef _BUZZER_HPP_
#define _BUZZER_HPP_

#include <stdarg.h>
#include "main.h"
#include "eventHandle.hpp"



class BuzzerDriver: public EventHandle{
public:
	BuzzerDriver(GPIO_TypeDef* port, uint16_t pin, IndicatorType indicatorType)
        : EventHandle(indicatorType),port_(port), pin_(pin)
    {
        DEBUG_RTT(LOG_WARNING, "BuzzerDriver obj constructor called");
    }
	~BuzzerDriver(){
        if (onPeriodms_ != nullptr)
        {
            delete[] onPeriodms_;
            onPeriodms_ = nullptr;  // Reset to prevent future issues
        }
		DEBUG_RTT(LOG_WARNING, "BuzzerDriver obj destructor called");
	}

	void Run(void) override;
	void turnOff(void) override;

private:
	void setupInit(void) override;
	void setOutput(void) override;
	void stopOutput(void) override;

private:
    GPIO_TypeDef* port_;						//GPIO port of output
    uint16_t pin_;								//pin number of output
};



#endif /* BUZZER_H_ */

