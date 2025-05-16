/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "pdsTdsControls.h"
#if LLPDS_TDS_MENU == 1
#include "TDS.hpp"
#include "menu_01.h"
#include "pdsTdsMenu.h"
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
	DEFAULT_TDS_TEMPERATURE,
	DEFAULT_DEL_TDS_THRESHOLD,
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
WarningFlags tempTdsWarning{.all_flags = 0xFFFF}; // all enabled by default

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
	if (isTDSConnected(TDS4) && Flags.flagStatus(Settings, PUMP_1))
	{
		menu.Display_Log("Input Stage - Delivery Water TDS: \"%d\" ppm", tds4.readTDS());
		Log.logToMaster("Input Stage - Delivery Water TDS: \"%d\" ppm", tds4.readTDS());
	}
	else
		menu.Display_Log("Input Stage - Delivery Water TDS: --- ppm");
}

void warningIndication(HexColor ledColor)
{
	DEBUG_RTT(LOG_INFO, "***warningIndication OCCURRED***");
	Buzzzer->setOutputPattern(2, 500, 500, 500);
	((RgbDriver *)WLED1)->setRgbColor(ledColor);
	WLED1->setOutputPattern(2, 500, 500, 500);
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
		tempTdsWarning.all_flags = 0xFFFF;
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

void invokeST7(bool status)
{
	if(Flags.flagStatus(Status, PUMP_1) != status)
	{
		Flags.setFlag(Status, PUMP_1, status);
	}
}

bool isWarnActive(SELECT_TDS bit)
{
	return (isTdsWarningEnable.all_flags & (1 << bit)) && (tempTdsWarning.all_flags & (1 << bit));
}

bool checkTdsTemperatureWarnings(void)
{
	int getCurTempValue = tds4.readTemp();
	if ((isTDSConnected(TDS4) > 0) && (Flags.flagStatus(Settings, DEL_TDS_TEMPERATURE)) && (setTDS.delTemperature > getCurTempValue))
	{
		if (isWarnActive(TDS4_TEMPERATURE))
			menu.Display_Log("Delivery Water Output Temperature too low \"%d\"*C- system at danger of freezing.", getCurTempValue);
		Log.logToMaster("Delivery Minimum Output Temperature threshold triggered. Threshold Temperature: %d celcius."
						"Actual Temperature: %d celcius. Fill Duration xx minutes.", setTDS.diTemperature, getCurTempValue);
		return TdsWarn.Tds4_Temp = isWarnActive(TDS4_TEMPERATURE);
	}
	return false;
}

bool checkTdsThresholdWarnings(void)
{
	if ((isTDSConnected(TDS4) > 0) && (Flags.flagStatus(Settings, DEL_TDS_THRESHOLD) == true) && (setTDS.delThreshold < tds4.readTDS()))
	{
		if (isWarnActive(TDS4_THRESHOLD))
			menu.Display_Log("Delivery Output TDS Too High (\"%d\" ppm) - Resin Change Required", tds4.readTDS());
			Log.logToMaster("Delivery Output TDS threshold triggered. Threshold TDS : %d ppm. Actual TDS : %d ppm."
							"Fill Duration xx minutes.",
							setTDS.delThreshold, tds4.readTDS());
			return TdsWarn.Tds4_Thresh = isWarnActive(TDS4_THRESHOLD);
	}
	return false;

}

void checkTdsShutdownWarnings(void)
{
	TdsWarn.Tds_Shutdown = false;
	if (isTDSConnected(TDS4))
	{
		if(Flags.flagStatus(Settings, DEL_TDS_THRESHOLD) && Flags.flagStatus(Settings, DEL_TDS_SHUTDOWN) && setTDS.delThreshold < tds4.readTDS())
		{
			menu.Display_Log("Delivery Output is paused - Delivery Output TDS too High", setTDS.delThreshold);
			TdsWarn.Tds_Shutdown = true;
		}
		if(Flags.flagStatus(Settings, DEL_TDS_TEMPERATURE) && Flags.flagStatus(Settings, DEL_TEMP_SHUTDOWN) && setTDS.delTemperature > tds4.readTemp())
		{
			menu.Display_Log("Delivery Output is paused - Delivery Output Temperature too High", setTDS.delTemperature);
			TdsWarn.Tds_Shutdown = true;
		}

		Log.logToMaster("Delivery Output TDS or Temperature shutdown triggered. Threshold TDS/Temperature: %d ppm / %d celcius."
						"Actual TDS/Temperature: %d ppm, xx celcius. Fill Duration xx minutes.",
						setTDS.delThreshold, setTDS.delTemperature, tds4.readTDS());
	}
	if (TdsWarn.Tds_Shutdown)
	{
		warningIndication(ORANGE);
		invokeST7(false);
		TdsWarn.Tds_Shutdown = false;
	}
}

void monitorTdsDel(void)
{
	Flags.update();
	static long lastMsgDisplayed = 0u;
	bool isShowTdsWarnMsg = false;
	bool isShowTempWarnMsg = false;

	if (HAL_GetTick() - lastMsgDisplayed < 2000)
		return;

	calCurrentTdsReading();
	if(Flags.flagStatus(Settings, PUMP_1))
	{
		if(Flags.flagStatus(Status, PUMP_1))
		{
			checkTdsShutdownWarnings();
		}
		isShowTdsWarnMsg = checkTdsThresholdWarnings();
		isShowTempWarnMsg = checkTdsTemperatureWarnings();
	}

	if (isShowTdsWarnMsg || isShowTempWarnMsg)
	{
		if (HAL_GetTick() - lastMsgDisplayed > 2000)
			warningIndication(RED);
		menu.Display_Log("Would you like to ignore this warning for this fill cycle?");
		menu.changeMenu(&pdsTds_haltMenu);

		isShowTdsWarnMsg = false;
		isShowTempWarnMsg = false;
	}
	
	lastMsgDisplayed = HAL_GetTick();
}

void controlPeripherals(void)
{
	if(!pdsTds_isMenuLabelMatch(currentM, pdsTds_intro) && !pdsTds_isMenuLabelMatch(currentM, pdsTds_mainMenu))
		monitorTdsDel();
}

void controlSetup(void)
{
	invokeST7(true); // just to test logic
}

#endif
