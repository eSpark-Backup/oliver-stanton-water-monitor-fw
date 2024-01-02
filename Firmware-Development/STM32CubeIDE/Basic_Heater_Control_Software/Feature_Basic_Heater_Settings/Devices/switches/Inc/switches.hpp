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


#define Debounce_Period 50

typedef enum{
	EnterOK,
	Back,
	UP,
	DOWN
//	RODI,
//	DEL
}SWITCHES;

struct BUTTON{
	uint16_t period;
	bool state;
};

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
	BUTTON getState(SWITCHES SW);
	bool getState(SWITCHES SW, uint16_t period);
	bool isButtonPressed(SWITCHES SW);
	bool isButtonStillPressed(SWITCHES SW);
	bool resetButtonState(SWITCHES SW);
	void saveTime(void);
};
