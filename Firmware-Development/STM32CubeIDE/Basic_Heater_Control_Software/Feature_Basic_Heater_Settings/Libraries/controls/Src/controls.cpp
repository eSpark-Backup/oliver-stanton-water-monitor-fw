/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "controls.h"
#include "TDS.hpp"

#define SECONDS_TO_MS(x) 				((x) * 1000) 						//converts seconds to milliseconds
#define MIN_TO_MS(x) 					((x) * SECONDS_TO_MS(60))			//converts minutes to milliseconds
#define FOD_AUTORUN_DELAY_PERIOD		SECONDS_TO_MS(60)					//60secs
#define MEM_SAVER_PAUSE_TIMEOUT			MIN_TO_MS(15)						//15mins or 900secs


short roSaverThreshValue = 0;
//short currentRoValue;


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

//--------------------------------Functions definitions-------------------------


//***************************TDS Sensor*******************************
float tdsSensor(uint32_t channel)
{
    switch (channel) {
    case ADC_CHANNEL_14: case ADC_CHANNEL_15:
		return tds2.readTDS();
		break;
    case ADC_CHANNEL_1: case ADC_CHANNEL_2:
		return tds4.readTDS();
		break;
	}
	return 0;
}
//*******************************************************************


//***********************Purefication Cycle**************************
bool fodOFF(uint32_t argChoice, MenuItem* argMenu)
{
	if (ButtonE.state())
	{
		if (Flags.flagStatus(Settings, FoD))
		{
			Flags.setFlag(Settings, FoD, false);
			DEBUG_RTT(LOG_WARNING, "Fill on Demand disabled");
		}
		if(ButtonE.period() > SHORT_PRESS && ButtonE.period() < LONG_LONG_PRESS)
		{
			Disp->display("System Purification stopped.");
			Flags.setFlag(Settings, MEM_SAVER, false);
			Flags.setFlag(Settings, MEM_SAVER_PAUSE, false);
			invokeST3(false);
			invokeST5(false);
			mainMenuFunc();
		}
		else if (ButtonE.period() < SINGLE_PRESS)
		{
			Flags.setFlag(Settings, MEM_SAVER_PAUSE, false);
			if(Flags.flagStatus(Status, LEVEL_SENSOR) == true)
			{
				Flags.setFlag(Settings, MEM_SAVER, false);
				fillOpen();
				// invokeST3(false); /// called twice
			}
			else if(!Flags.flagStatus(Settings, MEM_SAVER))
			{
//				Disp->display("Starting Purification - System Filling.");
//				Flags.setFlag(Settings, MEM_SAVER, true);
//				Flags.setFlag(Status, PURE_CYCLE, true);
				invokeST3(true);
				invokeST5(true);
			}
			else
			{
				Flags.setFlag(Settings, MEM_SAVER, false);
				Flags.setFlag(Settings, MEM_SAVER_PAUSE, true);
				Disp->display("System Purification paused.");
				invokeST3(false);
				invokeST5(false);
			}
		}
		return true;
	}
	return false;
}


bool BatLowMenuOption(uint32_t argChoice, MenuItem *argMenu)
{
	if (argChoice == 2 && ButtonE.state())
	{
		return BatLowShutdownOn(argChoice, argMenu);
	}
	else
		return false;
}

bool BatLowShutdown(uint32_t argChoice, MenuItem *argMenu)
{
	if (argChoice == 2 && ButtonE.state())
	{
		return BatLowShutdownOff(argChoice, argMenu);
	}
	else if (argChoice == 3 && ButtonE.state())
	{
		return BatLowShutdownOn(argChoice, argMenu);
	}
	else
		return false;
}

bool BatLowShutdownOn(uint32_t argChoice, MenuItem *argMenu)
{
	Disp->display("Battery LOW power shutdown Enabled.");
	Flags.setFlag(Settings, BATTERY_LOW_SHUTDON, true);
	return true;
}

bool BatLowShutdownOff(uint32_t argChoice, MenuItem* argMenu)
{
	Disp->display("Battery LOW power shutdown Disabled.");

	Flags.setFlag(Settings, LOG_TO_MASTER, true);
	Flags.setFlag(Settings, BATTERY_LOW_SHUTDON, false);
	return true;
}

bool BatPowerMonitor(uint32_t argChoice, MenuItem *argMenu)
{
	if (argChoice == 1 && ButtonE.state())
	{
		return BatPowerMonitorOff(argChoice, argMenu);
	}
	else if (argChoice == 2 && ButtonE.state())
	{
		return BatPowerMonitorOn(argChoice, argMenu);
	}
	else
		return false;
}

bool BatPowerMonitorOn(uint32_t argChoice, MenuItem *argMenu)
{
	Disp->display("Battery Power Monitoring Enabled.");
	Flags.setFlag(Settings, BATTERY_POWER_MONITOR, true);
	return true;
}

bool BatPowerMonitorOff(uint32_t argChoice, MenuItem* argMenu)
{
	Disp->display("Battery Power Monitoring Disabled.");
	Flags.setFlag(Settings, BATTERY_POWER_MONITOR, false);
	return true;
}

bool fodON(uint32_t argChoice, MenuItem* argMenu)
{
	if (argChoice == 2 && ButtonE.state())
	{
		DEBUG_RTT(LOG_ERROR, "fodON");

		Flags.setFlag(Settings, FoD, true);
		Flags.setFlag(Settings, MEM_SAVER, true);
		return true;
	}
	return false;
}
bool roSaverThreshAdj(uint32_t argChoice, MenuItem* argMenu)
{
	static uint8_t threshVal = 0;

	if (ButtonD.state())
	{
		if(threshVal > 0)	threshVal--;
		Disp->display(threshVal);
	}
	else if (ButtonU.state())
	{
		if(threshVal < 20)	threshVal++;
		Disp->display(threshVal);
	}
	else if (ButtonE.state())
	{
		roSaverThreshValue = threshVal;
		threshVal = 0;
		DEBUG_RTT(LOG_WARNING, "RO_Saver threshold value set to %d.", roSaverThreshValue);
		return true;
	}
	else
		return false;
}

bool setRoSaverThresh(uint32_t argChoice, MenuItem *argMenu)
{
	if (argChoice == 1 && ButtonE.state())
	{
		roSaverThreshDefault(argChoice, argMenu);
	}
	else
		return false;
}

bool roSaverThreshDefault(uint32_t argChoice, MenuItem *argMenu)
{
	roSaverThreshValue = 0;
	DEBUG_RTT(LOG_WARNING, "RO_Saver default threshold value set to %d.", roSaverThreshValue);
	return true;
}

bool roSaverOptions(uint32_t argChoice, MenuItem *argMenu)
{
	if (argChoice == 1 && ButtonE.state())
	{
		roSaverOn(argChoice, argMenu);
	}
	else if (argChoice == 2 && ButtonE.state())
	{
		roSaverOff(argChoice, argMenu);
	}
	else
		return false;
}

bool roSaverOn(uint32_t argChoice, MenuItem *argMenu)
{
	Flags.setFlag(Settings, RESIN_SAVER, true);
	DEBUG_RTT(LOG_WARNING, "RO_Saver setting Turned on.");
	return true;
}

bool roSaverOff(uint32_t argChoice, MenuItem * argMenu)
{
	Flags.setFlag(Settings, RESIN_SAVER, false);
	DEBUG_RTT(LOG_WARNING, "RO_Saver setting Turned off.");
	return true;
}

void mainMenuFunc()
{
	currentM = &mainMenu;
	menu.displayMenu(&mainMenu, mainMenu.Items[0].num_items);
}


void fillOpen()
{
	Disp->display("Tank Full - System not Filling");

	Buzzzer->setOutputPattern(1, 500, 1000);
	((RgbDriver*)WLED1)->setRgbColor(RED);
	WLED1->setOutputPattern(1, 1000, 1000);

	invokeST3(false);
	invokeST5(false);
	DEBUG_RTT(LOG_WARNING, "SAVE LOG TO FILE HERE.");			//perform logging to file here
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
		//perform logging function here
		DEBUG_RTT(LOG_WARNING, "SAVE LOG TO FILE HERE.");			//perform logging to file here

		Buzzzer->setOutputPattern(2, 500, 1100, 500);
		// WLED1->setOutputPattern(-1, 1000, 1000);					// -1 to run infinitely
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
		if (tdsSensor(ADC_CHANNEL_14) < 1.0 && tdsSensor(ADC_CHANNEL_15) < 1.0) // Assuming a pulldown resistor [not connected means 0 value]
		{
			Disp->display("TDS Sensor Disconnected - unable to run RO Saver cycle");
			Flags.setFlag(Status, PURE_CYCLE, false);
			Buzzzer->setOutputPattern(1, 500, 1000);
			((RgbDriver *)WLED1)->setRgbColor(RED);
			WLED1->setOutputPattern(1, 1000, 1000);
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
		if(tds2.readTDS() <= roSaverThreshValue)
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

void memSaverST3()
{
	Flags.update();
	if(Flags.timeElapsed(MEM_SAVER, MEM_SAVER_PAUSE_TIMEOUT) && Flags.flagStatus(Settings, MEM_SAVER_PAUSE))
	{
		Flags.setFlag(Settings, MEM_SAVER, false);
		Flags.setFlag(Settings, MEM_SAVER_PAUSE, false);
		invokeST3(false);
		invokeST5(false);
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
