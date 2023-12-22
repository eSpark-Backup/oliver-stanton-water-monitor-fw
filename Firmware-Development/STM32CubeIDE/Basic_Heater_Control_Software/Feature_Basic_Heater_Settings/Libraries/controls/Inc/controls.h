/*
 * controls.h
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

#pragma once

#ifndef INC_CONTROLS_H_
#define INC_CONTROLS_H_

//#include "mainApp.hpp"
#include "main.h"
#include "menu.h"
#include "Display.h"
#include "CDC_Serial.h"
#include "flagHandler.h"


extern TIM_HandleTypeDef htim1;

typedef enum{
	Level_Sensor,
	Pressure_Sensor,
	Flow_Meter_1,
	Flow_Meter_2,
	Flow_Meter_3,
	WLED,
	RGB,
	Buzzer,
	FoD
}sensors;

typedef enum{
	Settings,
	Status
}Task;

static Display* disp = new CDC_Serial;
static flagHandle flags;
//static flagHandle& flags = flagHandle::getInstance();
static Menu menu;

extern int *currentM;// = (int*)(fodMenu);
static size_t currentE;// = sizeof(fodMenu) / sizeof(fodMenu[0]);
extern short choiceSelect;



bool PureFill(void);
void Buzzer_BUZZ(uint16_t);
void fillOpen(void);
void fillClosed(void);
bool fodON(void);
bool fodRun(void);


#endif /* INC_CONTROLS_H_ */
