/*
 * controlsequence.h
 *
 *  Created on: Apr 30, 2025
 *      Author: Nehal Abdul Rehman
 */

#pragma once

#ifndef INC_CONTROLS07_H_
#define INC_CONTROLS07_H_

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
#include "stm32f0xx_hal_conf.h"

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
	AUTO_FLUSH,
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
	uint16_t mainsThreshold 	= 0;
	uint8_t mainsTemperature 	= 0;
	uint8_t roDynamicThreshold 	= 0;
	uint8_t roFixedThreshold 	= 0;
	uint8_t roTemperature 		= 0;
	uint8_t diThreshold 		= 0;
	uint8_t diTemperature 		= 0;
}	TdsValues;

typedef enum
{
	TDS1,
	TDS2,
	TDS3,
	TDS4
} tdsSensor;

typedef union
{
	struct
	{
		uint16_t Tds1_Thresh : 1;
		uint16_t Tds1_Temp : 1;
		uint16_t Tds2_DynamicThresh : 1;
		uint16_t Tds2_FixedThresh : 1;
		uint16_t Tds2_Temp : 1;
		uint16_t Tds3_Thresh : 1;
		uint16_t Tds3_Temp : 1;
		uint16_t Tds_Shutdown : 1;
	};
	uint16_t all_flags; // Full-width member to reset all flags
} WarningFlags;

typedef enum
{
	VEHICLE,
	STATIC
} PRESSURE_SYSTEM_TYPE;

/**
 * Functions that are directly called from MenuList
 * <condition> <Function name>(<choiceselect variable>)
 *
 * return condition is if we want the function to jump back to previous menuItem or not
 */

/*	Support Functions	*/
void fillOpen(void);
void invokeST3(bool);
void invokeST4(bool);
void invokeST5(bool);

/*  Main Output Control functions  */
void memSaverST3(void);
bool roSaverST1(void);
bool isTDSConnected(tdsSensor);
void controlPeripherals(void);
void controlSetup(void);


extern TdsValues setTDS;
extern WarningFlags TdsWarn;
extern WarningFlags isTdsWarningEnable;
extern PRESSURE_SYSTEM_TYPE presSysType;

#endif /* INC_CONTROLS07_H_ */
