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
#include "Buzzer.hpp"
#include "LED.h"
#include "switches.hpp"
#include "TDS.hpp"

//-------button press durations, in milliseconds
#define MINIMUM_PRESS		100
#define SINGLE_PRESS		800
#define SHORT_PRESS			2000
#define LONG_PRESS			3000
#define LONG_LONG_PRESS		5000

extern Switches ButtonE;			//Enter/OK button obj
extern Switches ButtonB;			//Back button obj
extern Switches ButtonU;			//UP button obj
extern Switches ButtonD;			//Down button obj

typedef enum
{
	LEVEL_SENSOR,
	PRESSURE_SENSOR,
	FLOW_METER_1,
	FLOW_METER_2,
	FLOW_METER_3,
	MEM_SAVER,
	MEM_SAVER_PAUSE,
	RESIN_SAVER,
	PURE_CYCLE,
	AUTO_FILL,
	WLED,
	RGB,
	BUZZER,
	FoD,
	MAINS_TDS_THRESHOLD,
	RO_TDS_THRESHOLD,
	DI_TDS_THRESHOLD,
	MAINS_TDS_TEMPERATURE,
	RO_TDS_TEMPERATURE,
	DI_TDS_TEMPERATURE,
	MAINS_TDS_SHUTDOWN,
	RO_TDS_SHUTDOWN,
	DI_TDS_SHUTDOWN,
	BATTERY_CHARGING_DETECTED,
	BATTERY_CRITICAL,			 // Status
	BATTERY_LOW,				 // Status
	BATTERY_HIGH,				 // Status
	BATTERY_LOW_SHUTDON,		 // SETTING
	BATTERY_VOLTAGE_LOW,		 // Status
	BATTERY_FLAT_SHUTDOWN,		 // Status
	BATTERY_WARNING_SPIKES,		 // Status
	BATTERY_POWER_MONITOR,		 // SETTING
	BATTERY_PUMP_IGNORE_WARNING, // Status
	LOG_TO_MASTER,
	TRIGGER_WARNING,
	_sensors_ // placeholder for the max number of sensors declared
} Sensors;

typedef enum{
	Settings,
	Status,
	icons,
	indication
}Task;


static Display* Disp = new CDC_Serial;
static FlagHandle& Flags = FlagHandle::getInstance();
extern Menu menu;

extern MenuItem *currentM;			//pointer to the menu item we are currently at
//static size_t currentE;
extern short choiceSelect;		//next menu item choice item we want to jump to, we can also pass this variable to functions where we need to set a certain value(whole number) as setting
extern short g_csIndex;
//extern short ro_saver_thresh_set_value;

typedef struct
{
	uint16_t mainsThreshold = 0;
	uint16_t mainsTemperature = 0;
	uint16_t roThreshold = 0;
	uint16_t roTemperature = 0;
	uint16_t diThreshold = 0;
	uint16_t diTemperature = 0;
}TdsValues;


/**
 * Functions that are directly called from MenuList
 * <condition> <Function name>(<choiceselect variable>)
 *
 * return condition is if we want the function to jump back to previous menuItem or not
 */

bool fodOFF(uint32_t argChoice, MenuItem *argMenu);
bool fodON(uint32_t argChoice, MenuItem* argMenu);
bool setRoSaverThresh(uint32_t argChoice, MenuItem *argMenu);
bool roSaverThreshAdj(uint32_t argChoice, MenuItem* argMenu);
bool roSaverThreshDefault(uint32_t argChoice, MenuItem* argMenu);
bool roSaverOptions(uint32_t argChoice, MenuItem* argMenu);
bool roSaverOn(uint32_t argChoice, MenuItem* argMenu);
bool roSaverOff(uint32_t argChoice, MenuItem* argMenu);
bool BatLowMenuOption(uint32_t argChoice, MenuItem* argMenu);
bool BatLowShutdown(uint32_t argChoice, MenuItem* argMenu);
bool BatLowShutdownOn(uint32_t argChoice, MenuItem* argMenu);
bool BatLowShutdownOff(uint32_t argChoice, MenuItem* argMenu);
bool BatPowerMonitor(uint32_t argChoice, MenuItem *argMenu);
bool BatPowerMonitorOn(uint32_t argChoice, MenuItem *argMenu);
bool BatPowerMonitorOff(uint32_t argChoice, MenuItem* argMenu);


/*	Support Functions	*/
void mainMenuFunc(void);
void fillOpen(void);
void invokeST3(bool);
void invokeST5(bool);

/*  Main Output Control functions  */
void memSaverST3(void);
bool roSaverST1(void);

float tdsSensor(uint32_t);

extern TdsValues setTDS;

#endif /* INC_CONTROLS_H_ */
