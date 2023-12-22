/*
 * CDC_Serial.h
 *
 *  Created on: Dec 15, 2023
 *      Author: Farhan Ahmed
 */

#ifndef INC_CDC_SERIAL_H_
#define INC_CDC_SERIAL_H_

#include "stm32f0xx_hal.h"
#include "Display.h"


class CDC_Serial: public Display{
public:
	void displayString(const char* strng) override;
	void displayChoice(short var) override;
	void displayVar(short var, char* n) override;
};



#endif /* INC_CDC_SERIAL_H_ */
