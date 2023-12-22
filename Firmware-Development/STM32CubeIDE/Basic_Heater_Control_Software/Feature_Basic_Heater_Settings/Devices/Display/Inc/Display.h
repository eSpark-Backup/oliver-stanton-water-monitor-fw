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
	virtual void displayString(const char*) = 0;
	virtual void displayChoice(short) = 0;
	virtual void displayVar(short, char*) = 0;
};

#endif /* INC_DISPLAY_H_ */
