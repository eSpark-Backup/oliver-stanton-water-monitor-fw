/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "controls07.h"
#if MENU_07 == 1
#include "TDS.hpp"
#include "menu_01.h"
#include "menu_07.h"
#include "masterLog.h"

#define SECONDS_TO_MS(x) 				((x) * 1000) 						//converts seconds to milliseconds
#define MIN_TO_MS(x) 					((x) * SECONDS_TO_MS(60))			//converts minutes to milliseconds
#define FOD_AUTORUN_DELAY_PERIOD		SECONDS_TO_MS(60)					//60secs
#define MEM_SAVER_PAUSE_TIMEOUT			MIN_TO_MS(15)						//15mins or 900secs
#define AUTO_FLUSH_TIMEOUT				MIN_TO_MS(20)						//20 mins or 1200secs
#define AUTO_FLUSH_FINISH_5M			MIN_TO_MS(07)

TdsValues setTDS{
	DEFAULT_MAINS_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE,
	DEFAULT_DYNAMIC_TDS_THRESHOLD,
	DEFAULT_FIXED_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE,
	DEFAULT_DI_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE
};


//***************************Buzzer*******************************
EventHandle* Buzzzer = new BuzzerDriver(BUZZER_GPIO_Port, BUZZER_Pin, IndBUZZER);
EventHandle* WLED1 = new RgbDriver(R_CNTRL_GPIO_Port, R_CNTRL_Pin, G_CNTRL_Pin, B_CNTRL_Pin, IndRGB);

//***************************TDS Sensor*******************************
GPIOConfig tds1PinConfig  = {TDS1_T2_GPIO_Port, TDS1_T1_GPIO_Port, ADC_CHANNEL_10, TDS1_T2_Pin, TDS1_T1_Pin, ADC_CHANNEL_11};
GPIOConfig tds2PinConfig  = {TDS2_T2_GPIO_Port, TDS2_T1_GPIO_Port, ADC_CHANNEL_14, TDS2_T2_Pin, TDS2_T1_Pin, ADC_CHANNEL_15};
GPIOConfig tds3PinConfig  = {TDS3_T2_GPIO_Port, TDS3_T1_GPIO_Port, ADC_CHANNEL_12, TDS3_T2_Pin, TDS3_T1_Pin, ADC_CHANNEL_13};
GPIOConfig tds4PinConfig  = {TDS4_T2_GPIO_Port, TDS4_T1_GPIO_Port, ADC_CHANNEL_1, TDS4_T2_Pin, TDS4_T1_Pin, ADC_CHANNEL_2};
TDS tds1 = TDS(tds1PinConfig);
TDS tds2 = TDS(tds2PinConfig);
TDS tds3 = TDS(tds3PinConfig);
TDS tds4 = TDS(tds4PinConfig);
//*******************************************************************
Menu menu;
PRESSURE_SYSTEM_TYPE presSysType = STATIC;
//--------------------------------Functions definitions-------------------------

bool isTDSConnected(tdsSensor sensor)
{
	switch (sensor)
	{
		case TDS1:
			if (tds1.readTDS())
				return true;
			break;
		case TDS2:
			if (tds2.readTDS())
				return true;
			break;
		case TDS3:
			if (tds3.readTDS())
				return true;
			break;
		case TDS4:
			if (tds4.readTDS())
				return true;
			break;
	}
	return false;
}

//***********************Purification Cycle**************************

void fillOpen()
{
	Disp->display("Tank Full - System not Filling");
	Buzzzer->setOutputPattern(1, 500, 1000);
	((RgbDriver*)WLED1)->setRgbColor(RED);
	WLED1->setOutputPattern(1, 1000, 1000);

	invokeST3(false);
	invokeST5(false);
	Log.logToMaster("Float Circuit Open - Unable to commence filling");
	mainMenuFunc();
	//add buzzer/LED warning handler here
}

void invokeST3(bool status)
{
	if(Flags.flagStatus(Status, MEM_SAVER) != status)
	{
		Flags.setFlag(Status, MEM_SAVER, status);
	}
	if(status)
	{
		Disp->display("Starting Purification - System Filling.");
		Flags.setFlag(Settings, MEM_SAVER, status);
		Flags.setFlag(Status, PURE_CYCLE, status);
		Log.logToMaster("Float Circuit Closed - System filling commenced");
		Buzzzer->setOutputPattern(2, 500, 500, 500);
		((RgbDriver *)WLED1)->setRgbColor(BLUE);
		WLED1->setOutputPattern(2, 500, 500, 500);					// -1 to run infinitely
	}
	else
	{
		Flags.setFlag(Settings, MEM_SAVER, false);
		// WLED1->turnOff();
	}
}

void invokeST1(bool status)
{
	if(Flags.flagStatus(Status, RESIN_SAVER) != status)
	{
		Flags.setFlag(Status, RESIN_SAVER, status);
	}
}

void invokeST4(bool status)
{
	if (Flags.flagStatus(Status, AUTO_FLUSH) != status)
	{
		Flags.setFlag(Status, AUTO_FLUSH, status);
	}
}

void invokeST5(bool status)
{
	if(Flags.flagStatus(Status, AUTO_FILL) != status)
	{
		Flags.setFlag(Status, AUTO_FILL, status);
	}
}

bool roSaverST1()
{
	Flags.update();
	if (!Flags.flagStatus(Settings, MEM_SAVER_PAUSE) && Flags.flagStatus(Status, PURE_CYCLE)
	&& Flags.flagStatus(Settings, RESIN_SAVER) && !Flags.flagStatus(Status, MEM_SAVER) && !Flags.flagStatus(Status, AUTO_FILL))
	{
		if (!isTDSConnected(TDS2)) // Assuming a pulldown resistor [not connected means 0 value]
		{
			Flags.setFlag(Status, PURE_CYCLE, false);
			Buzzzer->setOutputPattern(1, 500, 1000);
			((RgbDriver *)WLED1)->setRgbColor(RED);
			WLED1->setOutputPattern(1, 500, 1000);
			return false;
		}
		Disp->display("Completing Purification Cycle - Dosing RO Membrane.");
		invokeST1(true); 
		invokeST5(true);
		Flags.update();
		return true;
	}
	else if (Flags.flagStatus(Status, PURE_CYCLE) && Flags.flagStatus(Status, RESIN_SAVER) && Flags.flagStatus(Status, AUTO_FILL))
	{
		if(tds2.readTDS() <= setTDS.roFixedThreshold)
		{
			Disp->display("Purification Cycle Complete.");
			tds2.resetTDS(); //for simulation value
			invokeST1(false);
			invokeST5(false);
			Flags.update();
			Flags.setFlag(Status, PURE_CYCLE, false);
			mainMenuFunc();
			return true;
		}
	}
	else if(!Flags.flagStatus(Settings, RESIN_SAVER) && Flags.flagStatus(Status, PURE_CYCLE) && Flags.flagStatus(Status, LEVEL_SENSOR))
	{
		Flags.setFlag(Status, PURE_CYCLE, false);
		Disp->display("Tank Full, System Purification Stopped.");
		mainMenuFunc();
		return true;
	}
//	else
		return false;
}

void stopAutoFillCycle(void)
{
	Flags.setFlag(Settings, MEM_SAVER, false);
	Flags.setFlag(Settings, MEM_SAVER_PAUSE, false);
	invokeST3(false);
	invokeST5(false);
}

void memSaverST3()
{
	Flags.update();
	if(Flags.timeElapsed(MEM_SAVER, MEM_SAVER_PAUSE_TIMEOUT) && Flags.flagStatus(Settings, MEM_SAVER_PAUSE))
	{
		stopAutoFillCycle();
		mainMenuFunc();
		DEBUG_RTT(LOG_WARNING, "***MEM_SAVER_PAUSE_TIMEOUT OCCURRED***");
	}
	if (Flags.flagStatus(Settings, MEM_SAVER_PAUSE) == false && Flags.flagStatus(Status, LEVEL_SENSOR) != false
		&& Flags.flagStatus(Status, MEM_SAVER) && (Flags.flagStatus(Settings, MEM_SAVER) || Flags.flagStatus(Settings, FoD)))
	{
		invokeST3(false);
		invokeST5(false);
		//			Flags.setFlag(Settings, MEM_SAVER, false);
		roSaverST1();
	}
	else if(Flags.timeElapsed(LEVEL_SENSOR, FOD_AUTORUN_DELAY_PERIOD) &&
			Flags.flagStatus(Settings, FoD) == true &&
			Flags.flagStatus(Status, LEVEL_SENSOR) != true
			&& Flags.flagStatus(Status, MEM_SAVER) != true)
	{
		//				Disp->display("Starting Purification - System Filling.");
		//				Flags.setFlag(Settings, MEM_SAVER, true);
		//				Flags.setFlag(Status, PURE_CYCLE, true);
		//				DEBUG_RTT("W", "SAVE LOG TO FILE HERE.");			//perform logging to file here

		invokeST3(true);
		invokeST5(true);
	}
}

void controlPeripherals(void)
{
	memSaverST3();
	roSaverST1();
}

void controlSetup(void)
{
}
#endif