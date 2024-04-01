/*
 * Display.h
 *
 *  Created on: Dec 18, 2023
 *      Author: Farhan Ahmed
 */

#pragma once

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "stm32f0xx_hal.h"
//#include "CDC_Serial.h"


class Display{
public:
//	virtual void displayInit(void) = 0;						//will use later for initializing LCD/CDC
	virtual void itemsToDisplay(int ) = 0;					//No. of items to be displayed in a menu, for OLED/LCD implementation
	virtual void display(const char*) = 0;
	virtual void display(short) = 0;
	virtual void display(int, char*) = 0;

	virtual ~Display() = default;							//virtual destructor
};

#endif /* INC_DISPLAY_H_ */
