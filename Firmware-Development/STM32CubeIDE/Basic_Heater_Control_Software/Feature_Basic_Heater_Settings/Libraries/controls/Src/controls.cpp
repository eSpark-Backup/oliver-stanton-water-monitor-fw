/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "controls.h"
#include "TDS.hpp"
#include "menu_01.h"
#include "menu_05.h"

#define SECONDS_TO_MS(x) 				((x) * 1000) 						//converts seconds to milliseconds
#define MIN_TO_MS(x) 					((x) * SECONDS_TO_MS(60))			//converts minutes to milliseconds
#define FOD_AUTORUN_DELAY_PERIOD		SECONDS_TO_MS(60)					//60secs
#define MEM_SAVER_PAUSE_TIMEOUT			MIN_TO_MS(15)						//15mins or 900secs


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

//--------------------------------Functions definitions-------------------------

bool isTDSConnected(tdsSensor sensor)
{
	switch (sensor)
	{
	case TDS1:
		return tds1.readTDS() ? true : false;
	case TDS2:
		return tds2.readTDS() ? true : false;
	case TDS3:
		return tds3.readTDS() ? true : false;
	case TDS4:
		return tds4.readTDS() ? true : false;
	}
	return 0;
}

int roRejectionPc()
{
	int mainsPc = (setTDS.mainsThreshold * 100) / MAX_MAINS_TDS_THRESHOLD;
	int roPc = (setTDS.mainsThreshold * 100) / MAX_RO_TDS_THRESHOLD;

	int rejectionPercentage = mainsPc - roPc;

	return rejectionPercentage;
}

void warningIndication(HexColor ledColor)
{
	Buzzzer->setOutputPattern(3, 500, 500, 500, 500);
	((RgbDriver *)WLED1)->setRgbColor(ledColor);
	WLED1->setOutputPattern(3, 500, 500, 500, 500);
}

//***********************Purification Cycle**************************

void fillOpen()
{
	Disp->display("Tank Full - System not Filling");

	Buzzzer->setOutputPattern(1, 500, 1000);
	((RgbDriver*)WLED1)->setRgbColor(BLUE);
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
		if (isTDSConnected(TDS2)) // Assuming a pulldown resistor [not connected means 0 value]
		{
			Disp->display("TDS2 Sensor Disconnected - unable to run RO Saver cycle");
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

void checkTemperatureWarnings(bool &isTempWarn)
{
    if (Flags.flagStatus(Settings, MAINS_TDS_TEMPERATURE))
    {
        menu.Display_Log("Mains Water Temperature too low \"%d\"*C- system at danger of freezing.", setTDS.mainsTemperature);
        return;
    }
    if (Flags.flagStatus(Settings, RO_TDS_TEMPERATURE))
    {
        menu.Display_Log("RO Membranes Temperature too low \"%d\"*C- system at danger of freezing.", setTDS.roTemperature);
        return;
    }
    if (Flags.flagStatus(Settings, DI_TDS_TEMPERATURE))
    {
        menu.Display_Log("DI RESIN Temperature too low \"%d\"*C- system at danger of freezing.", setTDS.diTemperature);
        return;
    }
    
    isTempWarn = false;
}

void checkThresholdWarnings(bool &isThreshWarn)
{
	if (isTDSConnected(TDS1) && Flags.flagStatus(Settings, MAINS_TDS_THRESHOLD) && setTDS.mainsThreshold < tds1.readTDS())
	{
		menu.Display_Log("Mains Input Too High (\"%d\" ppm) - System Inspection Required", setTDS.mainsThreshold);
		return;
	}
	if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, RO_TDS_THRESHOLD))
	{
		if (setTDS.roFixedThreshold < tds2.readTDS())
		{
			menu.Display_Log("RO TDS Too High (\"%d\" ppm) - System Inspection Required", setTDS.roFixedThreshold);
			return;
		}
		if (setTDS.roDynamicThreshold < roRejectionPc())
		{
			warningIndication(RED);
			return;
		}
	}
	if (isTDSConnected(TDS3) && Flags.flagStatus(Settings, DI_TDS_THRESHOLD) && setTDS.diThreshold < tds3.readTDS())
	{
		menu.Display_Log("DI RESIN TDS Too High (\"%d\" ppm) - Requires change", setTDS.diThreshold);
		return;
	}

	isThreshWarn = false;
}
void monitorTdsCycle(void)
{
	bool isTempWarn = true;
	bool isThreshWarn = true;

	Flags.update();

	if (Flags.flagStatus(Status, BATTERY_HIGH) || Flags.flagStatus(Status, BATTERY_CHARGING_DETECTED))
		checkTemperatureWarnings(isTempWarn);
	else
		isTempWarn = false;

	if (Flags.flagStatus(Status, AUTO_FILL) && !Flags.flagStatus(Status, AUTO_FLUSH))
		checkThresholdWarnings(isThreshWarn);
	else
		isThreshWarn = false;


	if (isTempWarn || isThreshWarn)
	{
		warningIndication(RED);
	}
}