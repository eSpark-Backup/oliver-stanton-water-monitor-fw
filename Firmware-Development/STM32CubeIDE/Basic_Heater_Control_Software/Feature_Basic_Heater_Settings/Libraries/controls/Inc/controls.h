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
#include "_DEBUG_.h"
#include "flagHandle.h"
#include "main.h"
#include "menu.h"
#include "Display.h"
#include "CDC_Serial.h"
#include "eventHandle.hpp"
#include "switches.hpp"
#include "TDS.hpp"

//-------button press durations
#define SINGLEPRESS	800
#define SHORTPRESS		2000
#define LONGPRESS		3000
#define LONGLONGPRESS	5000

extern Switches ButtonE;			//Enter/OK button obj
extern Switches ButtonB;			//Back button obj
extern Switches ButtonU;			//UP button obj
extern Switches ButtonD;			//Down button obj

typedef enum{
	Level_Sensor,
	Pressure_Sensor,
	Flow_Meter_1,
	Flow_Meter_2,
	Flow_Meter_3,
	MEM_Saver,
	Mem_Saver_Pause,
	RESIN_Saver,
	Pure_CYCLE,
	AUTO_Fill,
	WLED,
	RGB,
	Buzzer,
	FoD,
	_sensors_						//placeholder for the max number of sensors declared
}Sensors;

typedef enum{
	Settings,
	Status,
	icons,
	indication
}Task;

static Display* Disp = new CDC_Serial;
static FlagHandle& Flags = FlagHandle::getInstance();
extern Menu menu;

extern int *currentM;			//pointer to the menu item we are currently at
//static size_t currentE;
extern short choiceSelect;		//next menu item choice item we want to jump to, we can also pass this variable to functions where we need to set a certain value(whole number) as setting
//extern short ro_saver_thresh_set_value;



/**
 * Functions that are directly called from MenuList
 * <condition> <Function name>(<choiceselect variable>)
 *
 * return condition is if we want the function to jump back to previous menuItem or not
 */
bool fodOFF(uint32_t);
bool fodON(uint32_t);
bool roSaverThreshAdj(uint32_t);
bool roSaverThreshDefault(uint32_t);
bool roSaverOn(uint32_t);
bool roSaverOff(uint32_t);


/*	Support Functions	*/
void mainMenuFunc(void);
void fillOpen(void);
void invokeST3(bool);

/*  Main Output functions  */
void memSaverST3(void);
bool roSaverST1(void);
bool buzzer(uint8_t, uint16_t timePeriod, ...);

float tds_Sensor(uint32_t);

#endif /* INC_CONTROLS_H_ */
