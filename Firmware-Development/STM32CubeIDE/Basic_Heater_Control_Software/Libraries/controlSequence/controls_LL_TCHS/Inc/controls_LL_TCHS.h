/*
 * controls_LL_TCHS.h
 *
 *  Created on: June 12, 2025
 *      Author: Abdul Ahad
 */

#pragma once

#ifndef _CONTROLS_LLTCHS_
#define _CONTROLS_LLTCHS_

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
extern Switches ButtonR;			//RODI button obj
extern Switches ButtonDel;			//Del button obj



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
	PUMP_1,
	AUTO_FILL,
	WLED,
	RGB,
	BUZZER,
	FoD,
	MAINS_TDS_THRESHOLD,
	RO_TDS_THRESHOLD,
	DI_TDS_THRESHOLD,
	DEL_TDS_THRESHOLD,
	MAINS_TDS_TEMPERATURE,
	RO_TDS_TEMPERATURE,
	DI_TDS_TEMPERATURE,
	DEL_TDS_TEMPERATURE,
	DEL_COLD_DYNAMICS,
	MAINS_TDS_SHUTDOWN,
	RO_TDS_SHUTDOWN,
	DI_TDS_SHUTDOWN,
	DEL_TDS_SHUTDOWN,
	DEL_TEMP_SHUTDOWN,
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
	MAIN_HEATER,
	FROST_PREVENT,
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

extern uint32_t lastTick;
extern bool menuSwitched;

typedef struct
{
	uint32_t preHeatingTime;
	uint32_t frostPreventTime;
	uint8_t frostPreventTempThresh;
	bool tempFormat;
	
	int16_t celsiusToFahrenheit(int16_t celsius) {
    	return (int16_t)((celsius * 9) / 5 + 32);
	}

} FrostPreventParams;

typedef struct 
{
	int16_t tds1Temp;	/*Celsius All from API*/
	int16_t tds2Temp;	
	int16_t tds3Temp;	
	int16_t tds4Temp;	
}TempValues;

typedef struct
{
	uint16_t mainsThreshold 	= 0;
	uint8_t mainsTemperature 	= 0;
	uint8_t roDynamicThreshold 	= 0;
	uint8_t roFixedThreshold 	= 0;
	uint8_t roTemperature 		= 0;
	uint8_t diThreshold 		= 0;
	uint8_t diTemperature 		= 0;
	uint8_t delThreshold 		= 0;
	uint8_t delTemperature 		= 0;
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
		uint16_t Tds4_Thresh : 1;
		uint16_t Tds4_Temp : 1;
		uint16_t Tds_Shutdown : 1;
	};
	uint16_t all_flags; // Full-width member to reset all flags
} WarningFlags;

typedef enum
{
	TDS1_THRESHOLD,
	TDS1_TEMPERATURE,
	TDS2_DYNAMIC_THRESHOLD,
	TDS2_FIXED_THRESHOLD,
	TDS2_TEMPERATURE,
	TDS3_THRESHOLD,
	TDS3_TEMPERATURE,
	TDS4_THRESHOLD,
	TDS4_TEMPERATURE,
	TDS_SHUTDOWN,
} SELECT_TDS;

typedef struct
{
	uint8_t diResinSvrIntrvl 		= 0;
}	selectST;

typedef union
{
	struct
	{
		uint16_t diResinSaver 		: 1;
	};
	uint16_t all_tempStateFlags; 	// Full-width member to reset all flags
} tempState;



/**
 * Functions that are directly called from MenuList
 * <condition> <Function name>(<choiceselect variable>)
 *
 * return condition is if we want the function to jump back to previous menuItem or not
 */


/*	Support Functions	*/
void fillOpen(void);
void invokeST1(bool);
void invokeST2(bool);
void invokeST3(bool);
void invokeST4(bool);
void invokeST5(bool);
void invokeDT1(bool);

/*  Main Output Control functions  */
void mainHeaterOn();
void controlPeripherals(void);
void mainMenuFuncHeater();

extern FrostPreventParams frostParam;
extern TdsValues setTDS;
extern WarningFlags TdsWarn;
extern WarningFlags isTdsWarningEnable;
extern WarningFlags tempTdsWarning;





















#endif /*_CONTROLS_LLTCHS_*/