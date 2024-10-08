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
#include "mainApp.hpp"
#include "controls.h"
#include "eventHandle.hpp"



class BuzzerDriver: public EventHandle{
public:
	BuzzerDriver(GPIO_TypeDef* port, uint16_t pin, bool isPWM)
        : port_(port), pin_(pin), isPWM_(isPWM){

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

	void setOutputPattern(int8_t numBlinks, uint16_t timePeriod, ...) override;			// numBlinks < 0 means the event is supposed to run infinitely
	void Run(void) override;
	void turnOff(void) override;

private:
	void setupInit(void) override;
	void startPWM(void) override;
	void stopPWM(void) override;
	void toggle(void) override;
	void toggleInfinite(void) override;

private:
    GPIO_TypeDef* port_;						//GPIO port of output
    uint16_t pin_;								//pin number of output
    bool isPWM_;								//is output controlled by PWM
    uint16_t *onPeriodms_ = nullptr;						//output turn on duration
    uint16_t offPeriodms_ = 0;						//output turn off duration
    int8_t numBlinks_ = 0;							//How many times you want output to turn on
    uint16_t blinkCount_ = 0;						//current number of blink
    bool isOn_ = false;									//is object activated or not
    bool state_ = false;								//obj is currently in runnign or off state
    uint32_t lastToggleTime_ = false;					//Tick() when output was toggled
};



#endif /* BUZZER_H_ */

