/*
 * switches.hpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan Ahmed
 */
#pragma once

#include "stm32f0xx_hal.h"
#include "main.h"
#include "mainApp.hpp"


#define DEBOUNCE_PERIOD 70

typedef enum{
	ENTER_OK,
	BACK,
	UP,
	DOWN,
	RODI,
//	DEL
	NONE
}SWITCHES;


extern bool sw2Flag;
extern bool sw3Flag;
extern bool sw4Flag;
extern bool sw5Flag;
extern bool sw6Flag;
extern bool sw7Flag;
extern bool timeStamped;			//flag for the time being saved


extern uint32_t buttonPressTime;

class Switches {
public:
	void getState(SWITCHES SW);
	bool state(void);
	uint16_t period(void);
	void enableButton(bool enable);
	bool isButtonEnabled();
	bool isButtonPressed(SWITCHES SW);
	bool isButtonStillPressed(SWITCHES SW);
	void resetButtonState(SWITCHES SW);
	void reset(void);
private:
	struct BUTTON{
		uint16_t period;
		bool 	 state;
		bool	 enabled = true;
	}Button;
};

void saveTime(void);
