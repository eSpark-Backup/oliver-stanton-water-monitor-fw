/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "tdsControls08.h"

#if LLTDS_MENU_08 == 1
#include "tdsMenu_08.h"
#include "TDS.hpp"
#include "menu_01.h"
#include "masterLog.h"

#define SECONDS_TO_MS(x) 				((x) * 1000) 						//converts seconds to milliseconds
#define MIN_TO_SEC(x) 					((x) * 60)							//converts minutes to seconds
#define MIN_TO_MS(x) 					((x) * SECONDS_TO_MS(60))			//converts minutes to milliseconds
#define FOD_AUTORUN_DELAY_PERIOD		SECONDS_TO_MS(60)					//60secs
#define MEM_SAVER_PAUSE_TIMEOUT			MIN_TO_MS(15)						//15mins or 900secs
#define AUTO_FLUSH_TIMEOUT				MIN_TO_MS(20)						//20 mins or 1200secs
#define FINISH_5M						MIN_TO_MS(05)
#define CUSTOM_INTERVAL_SEC(x)			MIN_TO_SEC(x)
#define CUSTOM_INTERVAL(x)				MIN_TO_MS(x)

wasteValues setWasteParam;

TdsValues setTDS{
	DEFAULT_MAINS_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE,
	DEFAULT_DYNAMIC_TDS_THRESHOLD,
	DEFAULT_FIXED_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE,
	DEFAULT_DI_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE
};

selectST setST{
	DEFAULT_RESIN_SAVER_INTRVL
};

tempState perpStatus{
	.all_tempStateFlags = false};

//***************************Buzzer*******************************
EventHandle *Buzzzer = new BuzzerDriver(BUZZER_GPIO_Port, BUZZER_Pin, IndBUZZER);
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
void mainMenuFunc02()
{
	currentM = &llTds_mainMenu08;
	menu.displayMenu(&llTds_mainMenu08, mainMenu.Items[0].num_items);
}

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
	((RgbDriver*)WLED1)->setRgbColor(RED);
	WLED1->setOutputPattern(1, 1000, 1000);

	invokeST3(false);
	invokeST5(false);
	if(flushTimer.activityStatus == DEACTIVATED && !Flags.flagStatus(Status, AUTO_FLUSH))
	{
		invokeST4(true);
		flushTimer.activityStatus = ACTIVATED;
		Disp->display("Flushing Membrane for 5 minutes");
	}
	Log.logToMaster("Float Circuit Open - Unable to commence filling");
	mainMenuFunc02();
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

void invokeST6(bool status)
{
	if (Flags.flagStatus(Status, PUMP_2) != status)
	{
		Flags.setFlag(Status, PUMP_2, status);
	}
}

void invokeST10(bool status)
{
	// BOOST_PRD_SIG and BOOST_AF_SIG responds to same actuator status but different settings
	if(Flags.flagStatus(Status, BOOST_PRD_SIG) != status)
	{
		Flags.setFlag(Status, BOOST_PRD_SIG, status);
		menu.Display_Log("ST10 Booster Signal: %s\n", status ? "Activated" : "Deactivated");
	}
}

bool roSaverST1()
{
	Flags.update();
	static long lastMsgDisplayed = 0u;
	if (Flags.flagStatus(Settings, RESIN_SAVER) && Flags.flagStatus(Status, PURE_CYCLE) && Flags.flagStatus(Status, AUTO_FILL)
		&& !Flags.flagStatus(Status, AUTO_FLUSH) && !Flags.flagStatus(Status, RESIN_SAVER) && !perpStatus.diResinSaver)
	{
		invokeST1(true);
		invokeST6(true);
		perpStatus.diResinSaver = true;
	}
	else if (Flags.flagStatus(Settings, RESIN_SAVER) && Flags.flagStatus(Status, PURE_CYCLE) && !Flags.flagStatus(Status, AUTO_FILL)
		&& Flags.getTime(AUTO_FLUSH, false) < 10 && !Flags.flagStatus(Status, RESIN_SAVER))
	{
		invokeST1(true);
		invokeST6(true);
	}
	else if (Flags.flagStatus(Status, RESIN_SAVER) && (Flags.checkOnTime(RESIN_SAVER, CUSTOM_INTERVAL(setST.diResinSvrIntrvl)) || !Flags.flagStatus(Settings, RESIN_SAVER)))
	{
		invokeST1(false);
		invokeST6(false);
	}
	else if (Flags.flagStatus(Status, RESIN_SAVER) && (HAL_GetTick() - lastMsgDisplayed > 10000))
	{
		lastMsgDisplayed = HAL_GetTick();
		menu.Display_Log("Purifying DI Saver Enabled: %d seconds to go", MIN_TO_SEC(setST.diResinSvrIntrvl) - Flags.getTime(RESIN_SAVER, true));
	}
}

void BoosterSignalControl(void)
{
	if ((Flags.flagStatus(Settings, BOOST_PRD_SIG) && Flags.flagStatus(Status, AUTO_FILL))
	|| (Flags.flagStatus(Settings, BOOST_AF_SIG) && Flags.flagStatus(Status, AUTO_FLUSH)))
	{
		invokeST10(true);
	}
	if (!Flags.flagStatus(Settings, BOOST_PRD_SIG))
	{
		if (Flags.flagStatus(Status, AUTO_FLUSH))
			invokeST10(false);
		else if (!Flags.flagStatus(Status, AUTO_FLUSH) && Flags.flagStatus(Status, AUTO_FILL))
			invokeST10(true);
	}
	else if(!Flags.flagStatus(Settings, BOOST_PRD_SIG))
	{
		invokeST10(false);
	}
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
		mainMenuFunc02();
		DEBUG_RTT(LOG_WARNING, "***MEM_SAVER_PAUSE_TIMEOUT OCCURRED***");
	}
	if (Flags.flagStatus(Settings, MEM_SAVER_PAUSE) == false && Flags.flagStatus(Status, LEVEL_SENSOR) != false
		&& Flags.flagStatus(Status, MEM_SAVER) && (Flags.flagStatus(Settings, MEM_SAVER) || Flags.flagStatus(Settings, FoD)))
	{
		invokeST3(false);
		invokeST5(false);
		//			Flags.setFlag(Settings, MEM_SAVER, false);
		// roSaverST1();
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

void cycleCount(uint32_t timeLeft)
{
	bool cycleSubsequent = flushTimer.cycleCount > 5;
	const char *cycle[] = {"First", "Second", "Third", "Fourth", "Fifth", "Subsequent (5+)"};
	menu.Display_Log("Purifying - %s Auto Flush Cycle - Flush time left: %d seconds", cycle[!cycleSubsequent ? flushTimer.cycleCount: 5], timeLeft);
}

void autoFlushST4(void)
{
	Flags.update();
	static long lastMsgDisplayed = 0u;

	if (Flags.flagStatus(Settings, AUTO_FLUSH) && !Flags.flagStatus(Status, LEVEL_SENSOR) && Flags.flagStatus(Status, PURE_CYCLE))
	{
		if(flushTimer.activityStatus == WAIT)
		{	
			if(Flags.getTime(AUTO_FLUSH, false) > 1 && Flags.getTime(AUTO_FLUSH, false) >= CUSTOM_INTERVAL_SEC(flushTimer.productionTimer))
			{
				flushTimer.activityStatus = READY;
				invokeST4(false);
			}
			else if((HAL_GetTick() - lastMsgDisplayed > MIN_TO_MS(1)))
			{
				uint32_t timeRemaining = flushTimer.productionTimer - (Flags.getTime(AUTO_FLUSH, false)/60);
				lastMsgDisplayed = HAL_GetTick();
				menu.Display_Log("Next Auto Flush Cycle in: %d minutes", timeRemaining);

			}
		}
		if (flushTimer.activityStatus != WAIT)
		{
			uint32_t flushOnTime = Flags.getTime(AUTO_FLUSH, true);
			uint32_t curCycleTime = (!flushTimer.cycleCount) ? flushTimer.startUpTimer : flushTimer.subsequentTimer;
			
			if(ButtonE.state() && ButtonE.period() < SINGLE_PRESS || flushTimer.activityStatus == READY)
			{
				if(!Flags.flagStatus(Status, AUTO_FLUSH))
				{
					invokeST4(true);
					flushTimer.activityStatus = ACTIVATED;
				}
				else if(Flags.flagStatus(Status, AUTO_FLUSH))
				{
					invokeST4(false);
					flushTimer.activityStatus = PAUSE;
					flushTimer.pausedTime =  ((flushTimer.pausedTime) ? flushTimer.pausedTime : CUSTOM_INTERVAL_SEC(curCycleTime)) - flushOnTime;
					Disp->display("System Flushing Paused");
				}
			}
			else if ((ButtonE.period() > SHORT_PRESS && ButtonE.period() < LONG_LONG_PRESS && Flags.flagStatus(Status, AUTO_FLUSH))
			|| (flushTimer.activityStatus == PAUSE && (Flags.getTime(AUTO_FLUSH, false) >= CUSTOM_INTERVAL_SEC(30))))
			{
				invokeST4(false);
				flushTimer.cycleCount = 0;
				flushTimer.pausedTime = 0;
				flushTimer.activityStatus = DEACTIVATED;
				Disp->display("System Flushing Deactivated");
			}
			Flags.update();
			if(flushTimer.activityStatus == ACTIVATED)
			{					
				flushOnTime = Flags.getTime(AUTO_FLUSH, true);
				curCycleTime = (!flushTimer.cycleCount) ? flushTimer.startUpTimer : flushTimer.subsequentTimer;

				if (flushOnTime >= ((flushTimer.pausedTime) ? flushTimer.pausedTime : CUSTOM_INTERVAL_SEC(curCycleTime)))
				{
					invokeST4(false);
					flushTimer.activityStatus = WAIT;
					flushTimer.pausedTime = 0;
					flushTimer.cycleCount++;
				}
				else if(flushTimer.activityStatus != PAUSE && (flushOnTime>1) && (HAL_GetTick() - lastMsgDisplayed > 10000))
				{
					uint32_t timeRemaining = ((flushTimer.pausedTime) ? flushTimer.pausedTime : CUSTOM_INTERVAL_SEC(curCycleTime)) - flushOnTime;
					lastMsgDisplayed = HAL_GetTick();
					cycleCount(timeRemaining);
				}
			}
		}
	}
	else if (Flags.flagStatus(Settings, AUTO_FLUSH) && Flags.flagStatus(Status, LEVEL_SENSOR))
	{
		if(Flags.flagStatus(Status, AUTO_FLUSH) && Flags.getTime(AUTO_FLUSH, true) >= FINISH_5M)
		{
			invokeST4(false);
			flushTimer.activityStatus = DEACTIVATED;
		}	
		flushTimer.cycleCount = 0;
		flushTimer.pausedTime = 0;
	}
	else if(!Flags.flagStatus(Settings, AUTO_FLUSH) && Flags.flagStatus(Status, AUTO_FLUSH))
	{
		invokeST4(false);
		flushTimer.activityStatus = DEACTIVATED;
		flushTimer.cycleCount = 0;
		flushTimer.pausedTime = 0;

	}	
}

bool isWarnActive(SELECT_TDS bit)
{
	return (isTdsWarningEnable.all_flags & (1 << bit)) && (tempTdsWarning.all_flags & (1 << bit));
}

bool checkTdsTemperatureWarnings(void)
{
    int getCurTempValue = tds1.readTemp();
	if (isTDSConnected(TDS1) && Flags.flagStatus(Settings, MAINS_TDS_TEMPERATURE) && setTDS.mainsTemperature < getCurTempValue)
	{
		if (isWarnActive(TDS1_TEMPERATURE))
			menu.Display_Log("Mains Water Temperature too low \"%d\"*C- system at danger of freezing.", getCurTempValue);
		Log.logToMaster("During Auto-Fill, Mains Minimum Temperature threshold triggered.Threshold Temperature: %d celcius."
						"Actual Temperature: %d celcius. Fill Duration xx minutes.", setTDS.mainsTemperature, getCurTempValue);
		return TdsWarn.Tds1_Temp = isWarnActive(TDS1_TEMPERATURE);
	}
	
	getCurTempValue = tds2.readTemp();
	if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, RO_TDS_TEMPERATURE) && setTDS.roTemperature < getCurTempValue)
	{
		if (isWarnActive(TDS2_TEMPERATURE))
			menu.Display_Log("RO Membranes Temperature too low \"%d\"*C- system at danger of freezing.", getCurTempValue);
		Log.logToMaster("During Auto-Fill, RO Minimum Temperature threshold triggered.Threshold Temperature: %d celcius."
						"Actual Temperature: %d celcius. Fill Duration xx minutes.", setTDS.roTemperature, getCurTempValue);
		return TdsWarn.Tds2_Temp = isWarnActive(TDS2_TEMPERATURE);
	}
	
	getCurTempValue = tds3.readTemp();
	if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, DI_TDS_TEMPERATURE) && setTDS.diTemperature < getCurTempValue)
	{
		if (isWarnActive(TDS3_TEMPERATURE))
			menu.Display_Log("DI RESIN Temperature too low \"%d\"*C- system at danger of freezing.", getCurTempValue);
		Log.logToMaster("During Auto-Fill, DI Minimum Temperature threshold triggered.Threshold Temperature: %d celcius."
						"Actual Temperature: %d celcius. Fill Duration xx minutes.", setTDS.diTemperature, getCurTempValue);
		return TdsWarn.Tds3_Temp = isWarnActive(TDS3_TEMPERATURE);
	}
}

bool checkTdsThresholdWarnings(void)
{
	if (isTDSConnected(TDS1) && Flags.flagStatus(Settings, MAINS_TDS_THRESHOLD) && setTDS.mainsThreshold < tds1.readTDS())
	{
		if (isWarnActive(TDS1_THRESHOLD))
			menu.Display_Log("Mains Input Too High (\"%d\" ppm) - System Inspection Required", tds1.readTDS());
		Log.logToMaster("During Auto-Fill, Mains TDS threshold triggered. Threshold TDS : %d ppm. Actual TDS : %d ppm."
						"Fill Duration xx minutes.",
						setTDS.mainsTemperature, tds1.readTDS());
		return TdsWarn.Tds1_Thresh = isWarnActive(TDS1_THRESHOLD);
	}
	if (Flags.flagStatus(Status, AUTO_FILL) && !Flags.flagStatus(Status, AUTO_FLUSH))
	{
		if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, RO_TDS_THRESHOLD))
		{
			if (Flags.timeElapsed(AUTO_FLUSH, FINISH_5M) && setTDS.roFixedThreshold < tds2.readTDS())
			{
				if (isWarnActive(TDS2_FIXED_THRESHOLD))
					menu.Display_Log("RO TDS Too High (\"%d\" ppm) - System Inspection Required", tds2.readTDS());
				Log.logToMaster("During Auto-Fill, RO Membrane Fixed TDS threshold triggered. Threshold TDS : %d ppm. Actual TDS : %d ppm."
								"Fill Duration xx minutes.",
								setTDS.roFixedThreshold, tds2.readTDS());
				return TdsWarn.Tds2_FixedThresh = isWarnActive(TDS2_FIXED_THRESHOLD);
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
			if (isWarnActive(TDS3_THRESHOLD))
				menu.Display_Log("DI RESIN TDS Too High (\"%d\" ppm) - Requires change", tds3.readTDS());
			Log.logToMaster("During Auto-Fill, DI RESIN TDS threshold triggered. Threshold TDS : %d ppm. Actual TDS : %d ppm."
							"Fill Duration xx minutes.",
							setTDS.diThreshold, tds3.readTDS());
			return TdsWarn.Tds3_Thresh = isWarnActive(TDS3_THRESHOLD);
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
		if (isTDSConnected(TDS3) && Flags.flagStatus(Settings, DI_TDS_THRESHOLD) && Flags.timeElapsed(AUTO_FLUSH, FINISH_5M) && Flags.flagStatus(Settings, DI_TDS_SHUTDOWN) && setTDS.diThreshold < tds3.readTDS())
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


	if (Flags.flagStatus(Status, BATTERY_HIGH) || Flags.flagStatus(Status, BATTERY_CHARGING_DETECTED))
		isShowWarnMsg = checkTdsTemperatureWarnings();

	if (Flags.flagStatus(Status, AUTO_FILL) || Flags.flagStatus(Status, AUTO_FLUSH))
	{
		isShowWarnMsg = checkTdsThresholdWarnings();
		checkTdsShutdownWarnings();
	}

	if (isShowWarnMsg && !TdsWarn.Tds2_DynamicThresh)
	{
		if (HAL_GetTick() - lastMsgDisplayed > 1000)
			warningIndication(RED);
		menu.Display_Log("Would you like to ignore this warning for this fill cycle?");
		if (TdsWarn.Tds2_FixedThresh && Flags.flagStatus(Settings, AUTO_FLUSH))
			menu.Display_Log("OR \nWould you like to a longer 20 minutes Auto-Flush?");
		lastMsgDisplayed = HAL_GetTick();
		menu.changeMenu(&haltMenu);
	}
}


void controlPeripherals(void)
{
	memSaverST3();
	// autoFlushST4();
	// roSaverST1();
	// monitorTdsCycle();
	// BoosterSignalControl();
}

#endif
