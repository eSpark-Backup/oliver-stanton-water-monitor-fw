/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "controls05.h"
#if MENU_05 == 1
#include "TDS.hpp"
#include "menu_01.h"
#include "menu_05.h"
#include "masterLog.h"

#define SECONDS_TO_MS(x) 				((x) * 1000) 						//converts seconds to milliseconds
#define MIN_TO_MS(x) 					((x) * SECONDS_TO_MS(60))			//converts minutes to milliseconds
#define FOD_AUTORUN_DELAY_PERIOD		SECONDS_TO_MS(60)					//60secs
#define MEM_SAVER_PAUSE_TIMEOUT			MIN_TO_MS(15)						//15mins or 900secs
#define AUTO_FLUSH_TIMEOUT				MIN_TO_MS(20)						//20 mins or 1200secs
#define AUTO_FLUSH_FINISH_5M			MIN_TO_MS(05)

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
WarningFlags TdsWarn = {.all_flags = 0};			  // all disabled by default
WarningFlags isTdsWarningEnable{.all_flags = 0xFFFF}; // all enabled by default

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

bool ifTdsIsDisconnected()
{
	for(int tdsCount = 0; tdsCount < TDS4; tdsCount++) // tds4 not active in sequence
	{

		switch (tdsCount)
		{
		case TDS1:
			if (!tds1.readTDS())
			{
				Disp->display("TDS1 Sensor Disconnected - unable to run Mains cycle");
				return false;
			}
			else
				return true;
		case TDS2:
			if (!tds2.readTDS())
			{
				Disp->display("TDS2 Sensor Disconnected - unable to run RO Saver cycle");
				return false;
			}
			else
				return true;
		case TDS3:
			if (!tds3.readTDS())
			{
				Disp->display("TDS3 Sensor Disconnected - unable to run DI cycle");
				return false;
			}
			else
				return true;
		case TDS4:
			if (!tds4.readTDS())
			{
				Disp->display("TDS4 Sensor Disconnected - unable to run TDS4 cycle");
				return false;
			}
			else
				return true;
		}
	}
	return 0;
}

void calCurrentTdsReading(void)
{
	if (isTDSConnected(TDS1) && Flags.flagStatus(Status, AUTO_FILL))
	{
		menu.Display_Log("Input Stage - Mains Water TDS: \"%d\" ppm", tds1.readTDS());
		Log.logToMaster("Input Stage - Mains Water TDS: \"%d\" ppm", tds1.readTDS());
	}
	else
		menu.Display_Log("Input Stage - Mains Water TDS: --- ppm");

	if (isTDSConnected(TDS2) && Flags.flagStatus(Status, AUTO_FILL))
	{
		menu.Display_Log("Mid Stage - RO Output TDS: \"%d\" ppm", tds2.readTDS());
		Log.logToMaster("Mid Stage - RO Output TDS: \"%d\" ppm", tds2.readTDS());
	}
	else
		menu.Display_Log("Mid Stage - RO Output TDS: --- ppm");

	if (isTDSConnected(TDS3) && Flags.flagStatus(Status, AUTO_FILL))
	{
		menu.Display_Log("Final Stage - DI Output TDS: \"%d\" ppm\n", tds3.readTDS());
		Log.logToMaster("Final Stage - DI Output TDS: \"%d\" ppm", tds3.readTDS());
	}
	else
		menu.Display_Log("Final Stage - DI Output TDS: --- ppm");
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
	DEBUG_RTT(LOG_INFO, "***warningIndication OCCURRED***");
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

bool checkTdsTemperatureWarnings(void)
{
    int getCurTempValue = tds1.readTemp();
	if (isTDSConnected(TDS1) && Flags.flagStatus(Settings, MAINS_TDS_TEMPERATURE) && setTDS.mainsTemperature < getCurTempValue)
	{
		if (isTdsWarningEnable.Tds1_Temp)
			menu.Display_Log("Mains Water Temperature too low \"%d\"*C- system at danger of freezing.", getCurTempValue);
		Log.logToMaster("During Auto-Fill, Mains Minimum Temperature threshold triggered.Threshold Temperature: %d celcius."
						"Actual Temperature: %d celcius. Fill Duration xx minutes.", setTDS.mainsTemperature, getCurTempValue);
		return TdsWarn.Tds1_Temp = isTdsWarningEnable.Tds1_Temp;
	}
	
	getCurTempValue = tds2.readTemp();
	if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, RO_TDS_TEMPERATURE) && setTDS.roTemperature < getCurTempValue)
	{
		if (isTdsWarningEnable.Tds2_Temp)
			menu.Display_Log("RO Membranes Temperature too low \"%d\"*C- system at danger of freezing.", getCurTempValue);
		Log.logToMaster("During Auto-Fill, RO Minimum Temperature threshold triggered.Threshold Temperature: %d celcius."
						"Actual Temperature: %d celcius. Fill Duration xx minutes.", setTDS.roTemperature, getCurTempValue);
		return TdsWarn.Tds2_Temp = isTdsWarningEnable.Tds2_Temp;
	}
	
	getCurTempValue = tds3.readTemp();
	if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, DI_TDS_TEMPERATURE) && setTDS.diTemperature < getCurTempValue)
	{
		if (isTdsWarningEnable.Tds3_Temp)
			menu.Display_Log("DI RESIN Temperature too low \"%d\"*C- system at danger of freezing.", getCurTempValue);
		Log.logToMaster("During Auto-Fill, DI Minimum Temperature threshold triggered.Threshold Temperature: %d celcius."
						"Actual Temperature: %d celcius. Fill Duration xx minutes.", setTDS.diTemperature, getCurTempValue);
		return TdsWarn.Tds3_Temp = isTdsWarningEnable.Tds3_Temp;
	}
}

bool checkTdsThresholdWarnings(void)
{
	if (isTDSConnected(TDS1) && Flags.flagStatus(Settings, MAINS_TDS_THRESHOLD) && setTDS.mainsThreshold < tds1.readTDS())
	{
		if (isTdsWarningEnable.Tds1_Thresh)
			menu.Display_Log("Mains Input Too High (\"%d\" ppm) - System Inspection Required", tds1.readTDS());
		Log.logToMaster("During Auto-Fill, Mains TDS threshold triggered. Threshold TDS : %d ppm. Actual TDS : %d ppm."
						"Fill Duration xx minutes.",
						setTDS.mainsTemperature, tds1.readTDS());
		return TdsWarn.Tds1_Thresh = isTdsWarningEnable.Tds1_Thresh;
	}
	if (Flags.flagStatus(Status, AUTO_FILL) && !Flags.flagStatus(Status, AUTO_FLUSH))
	{
		if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, RO_TDS_THRESHOLD))
		{
			if (Flags.timeElapsed(AUTO_FLUSH, AUTO_FLUSH_FINISH_5M) && setTDS.roFixedThreshold < tds2.readTDS())
			{
				if (isTdsWarningEnable.Tds2_FixedThresh)
					menu.Display_Log("RO TDS Too High (\"%d\" ppm) - System Inspection Required", tds2.readTDS());
				Log.logToMaster("During Auto-Fill, RO Membrane Fixed TDS threshold triggered. Threshold TDS : %d ppm. Actual TDS : %d ppm."
								"Fill Duration xx minutes.",
								setTDS.roFixedThreshold, tds2.readTDS());
				return TdsWarn.Tds2_FixedThresh = isTdsWarningEnable.Tds2_FixedThresh;
			}
			if (setTDS.roDynamicThreshold < roRejectionPc())
			{
				warningIndication(RED);
				Log.logToMaster("During Auto-Fill, RO Membrane Dynamic TDS rejection percentage triggered. Threshold TDS : %d %. Actual TDS : %d %."
								"Fill Duration xx minutes.",
								setTDS.roDynamicThreshold, roRejectionPc());
				return TdsWarn.Tds2_DynamicThresh = true;
			}
		}
		if (isTDSConnected(TDS3) && Flags.flagStatus(Settings, DI_TDS_THRESHOLD) && setTDS.diThreshold < tds3.readTDS())
		{
			if (isTdsWarningEnable.Tds3_Thresh)
				menu.Display_Log("DI RESIN TDS Too High (\"%d\" ppm) - Requires change", tds3.readTDS());
			Log.logToMaster("During Auto-Fill, DI RESIN TDS threshold triggered. Threshold TDS : %d ppm. Actual TDS : %d ppm."
							"Fill Duration xx minutes.",
							setTDS.diThreshold, tds3.readTDS());
			return TdsWarn.Tds3_Thresh = isTdsWarningEnable.Tds3_Thresh;
		}
	}
}

void checkTdsShutdownWarnings(void)
{
	TdsWarn.Tds_Shutdown = false;
	if (isTDSConnected(TDS1) && Flags.flagStatus(Settings, MAINS_TDS_THRESHOLD) && Flags.flagStatus(Settings, MAINS_TDS_SHUTDOWN) && setTDS.mainsThreshold < tds1.readTDS())
	{
		menu.Display_Log("Tank Full Purification Stopped - Mains Input TDS too High", setTDS.mainsThreshold);
		Log.logToMaster("During Auto-Fill, Mains TDS & Temperature shutdown triggered. Threshold TDS/Temperature: %d ppm / %d celcius."
						"Actual TDS/Temperature: %d ppm, xx celcius. Fill Duration xx minutes.",
						setTDS.mainsThreshold, setTDS.mainsTemperature, tds1.readTDS());
		TdsWarn.Tds_Shutdown = true;
	}
	if (Flags.flagStatus(Status, AUTO_FILL) && !Flags.flagStatus(Status, AUTO_FLUSH))
	{
		if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, RO_TDS_THRESHOLD) && Flags.flagStatus(Settings, RO_TDS_SHUTDOWN) && (setTDS.roFixedThreshold < tds2.readTDS() || setTDS.roDynamicThreshold < roRejectionPc()))
		{
			menu.Display_Log("Tank Full Purification Stopped - RO Membrane TDS Output too High");
			Log.logToMaster("During Auto-Fill, RO TDS & Temperature shutdown triggered. Threshold TDS/Temperature: %d ppm / %d celcius."
							"Actual TDS/Temperature: %d ppm, xx celcius. Fill Duration xx minutes.",
							setTDS.roFixedThreshold, setTDS.roTemperature, tds2.readTDS());
			TdsWarn.Tds_Shutdown = true;
		}
		if (isTDSConnected(TDS3) && Flags.flagStatus(Settings, DI_TDS_THRESHOLD) && Flags.timeElapsed(AUTO_FLUSH, AUTO_FLUSH_FINISH_5M) && Flags.flagStatus(Settings, DI_TDS_SHUTDOWN) && setTDS.diThreshold < tds3.readTDS())
		{
			menu.Display_Log("Tank Full Purification Stopped - DI RESIN too High");
			Log.logToMaster("During Auto-Fill, DI TDS & Temperature shutdown triggered. Threshold TDS/Temperature: %d ppm / %d celcius."
							"Actual TDS/Temperature: %d ppm, xx celcius. Fill Duration xx minutes.",
							setTDS.diThreshold, setTDS.diTemperature, tds3.readTDS());
			TdsWarn.Tds_Shutdown = true;
		}
	}
	if (TdsWarn.Tds_Shutdown)
	{
		warningIndication(ORANGE);
		stopAutoFillCycle();
		TdsWarn.Tds_Shutdown = false;
	}
}

void monitorTdsCycle(void)
{
	Flags.update();
	static long lastMsgDisplayed = 0u;
	bool isShowWarnMsg = false;

	if (HAL_GetTick() - lastMsgDisplayed < 1000)
		return;

	if (Flags.checkOnTime(AUTO_FLUSH, AUTO_FLUSH_TIMEOUT) && Flags.flagStatus(Settings, AUTO_FLUSH))
	{
		autoFlushEnable(false);
		invokeST4(false);
		DEBUG_RTT(LOG_WARNING, "***AUTO_FLUSH_TIMEOUT OCCURRED***");
	}

	if (Flags.flagStatus(Status, BATTERY_HIGH) || Flags.flagStatus(Status, BATTERY_CHARGING_DETECTED))
		isShowWarnMsg = checkTdsTemperatureWarnings();

	if (Flags.flagStatus(Status, AUTO_FILL) || Flags.flagStatus(Status, AUTO_FLUSH))
	{
		isShowWarnMsg = checkTdsThresholdWarnings();
		checkTdsShutdownWarnings();
	}

	if (isShowWarnMsg && !TdsWarn.Tds2_DynamicThresh)
	{
		DEBUG_RTT(LOG_WARNING, "isShowWarnMsg %d TdsWarn.Tds2_DynamicThresh%d", isShowWarnMsg,TdsWarn.Tds2_DynamicThresh);
		if (HAL_GetTick() - lastMsgDisplayed > 1000)
			warningIndication(RED);
		menu.Display_Log("Would you like to ignore this warning for this fill cycle?");
		if (TdsWarn.Tds2_FixedThresh)
			menu.Display_Log("OR \nWould you like to a longer 20 minutes Auto-Flush?");
		lastMsgDisplayed = HAL_GetTick();
		menu.changeMenu(&haltMenu);
	}
}


void controlPeripherals(void)
{
	memSaverST3();
	roSaverST1();
	monitorTdsCycle();
}

#endif