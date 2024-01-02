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
#include "flagHandle.h"
#include "main.h"
#include "menu.h"
#include "Display.h"
#include "CDC_Serial.h"
#include "eventHandle.hpp"
#include "switches.hpp"

//-------button press durations
#define single_press	800
#define short_press		2000
#define long_press		3000
#define long_long_press	5000

//extern TIM_HandleTypeDef htim1;

extern BUTTON buttonStateE;			//Enter/OK button obj
extern BUTTON buttonStateB;			//Back button obj
extern BUTTON buttonStateU;			//UP button obj
extern BUTTON buttonStateD;			//Down button obj

typedef enum{
	Level_Sensor,
	Pressure_Sensor,
	Flow_Meter_1,
	Flow_Meter_2,
	Flow_Meter_3,
	MEM_Saver,
	Mem_Saver_Pause,
	RESIN_Saver,
//	AUTO_Fill,
	WLED,
	RGB,
	Buzzer,
	FoD,
	_sensors_						//placeholder for the max number of sensors declared
}sensors;

typedef enum{
	Settings,
	Status,
	icons,
	indication
}Task;

static Display* disp = new CDC_Serial;
static flagHandle& flags = flagHandle::getInstance();
static Menu menu;

extern int *currentM;		//pointer to the menu item we are currently at
static size_t currentE;
extern short choiceSelect;				//next menu item choice item we want to jump to, we can also pass this variable to functions where we need to set a certain value(whole number) as setting
extern short ro_saver_thresh_set_value;


/**
 * Functions that are directly called from MenuList
 * <condition> <Function name>(<choiceselect variable>)
 *
 * return condition is if we want the function to jump back to previous menuItem or not
 */
bool fodOFF(uint32_t);
bool fodON(uint32_t);
bool ro_saver_thresh_adj(uint32_t);
bool ro_saver_thresh_default(uint32_t);
bool ro_saver_On(uint32_t);
bool ro_saver_Off(uint32_t);

/*	Support Functions	*/
void mainMenufunc(void);
void fillOpen(void);
void invoke_ST3(bool);
//bool fodRun(void);

/*  Main Output functions  */
void Mem_Saver_ST3(void);
void RO_Saver_ST1(void);
void Buzzer_BUZZ(uint16_t);
//void debug();

#endif /* INC_CONTROLS_H_ */
