/*
 * controls_LL_TCHS.cpp
 *
 *  Created on: June 12, 2025
 *      Author: Abdul Ahad
 */

#include "controls_LL_TCHS.h"

#if LL_TCHS_MENU == 1
#include "TDS.hpp"
#include "LL_TCHS_Menu.h"
#include "masterLog.h"
#include "Battery.hpp"


#define SECONDS_TO_MS(x) 				((x) * 1000) 						//converts seconds to milliseconds
#define MIN_TO_MS(x) 					((x) * SECONDS_TO_MS(60))			//converts minutes to milliseconds
#define FOD_AUTORUN_DELAY_PERIOD		SECONDS_TO_MS(60)					//60secs
#define MEM_SAVER_PAUSE_TIMEOUT			MIN_TO_MS(15)						//15mins or 900secs
#define AUTO_FLUSH_TIMEOUT				MIN_TO_MS(20)						//20 mins or 1200secs
#define AUTO_FLUSH_FINISH_5M			MIN_TO_MS(05)
#define FINISH_5M						MIN_TO_MS(05)



uint32_t lastTick=0;
bool menuSwitched = false;

FrostPreventParams frostParam{
	PREHEATING_TIME,
	DEFAULT_FROST_PREVENT_TIME,
	DEFAULT_PREV_TEMP_THRESH_C,
	DEFAULT_TEMP_FORMAT
};

TempValues currTempVal{0,0,0,0};

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

selectST setST{
	DEFAULT_RESIN_SAVER_INTRVL
};

tempState perpStatus{
	.all_tempStateFlags = false};


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


void mainMenuFuncHeater()
{
	currentM = &heaterIntro;
	menu.displayMenu(&heaterIntro, heaterIntro.Items[0].num_items);
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
bool isWarnActive(SELECT_TDS bit)
{
	return (isTdsWarningEnable.all_flags & (1 << bit)) && (tempTdsWarning.all_flags & (1 << bit));
}

bool checkTdsTemperatureWarnings(void)
{
	currTempVal.tds1Temp = -5;
	currTempVal.tds2Temp = 30;
	currTempVal.tds4Temp = 30;
    // currTempVal.tds1Temp = tds1.readTemp();
	// currTempVal.tds2Temp = tds2.readTemp();
	// currTempVal.tds4Temp = tds4.readTemp();

	// if (isTDSConnected(TDS1) && Flags.flagStatus(Settings, MAINS_TDS_TEMPERATURE) && currTempVal.tds1Temp  <= frostParam.frostPreventTempThresh )
	// if (currTempVal.tds1Temp  <= frostParam.frostPreventTempThresh && !isWarnActive(TDS1_TEMPERATURE))
	if (currTempVal.tds1Temp  <= frostParam.frostPreventTempThresh &&  !TdsWarn.Tds1_Temp)
	{
		if (isWarnActive(TDS1_TEMPERATURE))
			menu.Display_Log("Mains Water Temperature too low \"%d\"*C- system at danger of freezing.", currTempVal.tds1Temp);
		Log.logToMaster("During Auto-Fill, Mains Minimum Temperature threshold triggered.Threshold Temperature: %d celsius."
						"Actual Temperature: %d celsius. Fill Duration xx minutes.", frostParam.frostPreventTempThresh, currTempVal.tds1Temp);
		return TdsWarn.Tds1_Temp = isWarnActive(TDS1_TEMPERATURE);
	}
	
	if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, RO_TDS_TEMPERATURE) && currTempVal.tds2Temp  <= frostParam.frostPreventTempThresh && TdsWarn.Tds2_Temp)
	{
		if (isWarnActive(TDS2_TEMPERATURE))
			menu.Display_Log("RO Membranes Temperature too low \"%d\"*C- system at danger of freezing.", currTempVal.tds2Temp);
		Log.logToMaster("During Auto-Fill, RO Minimum Temperature threshold triggered.Threshold Temperature: %d celsius."
						"Actual Temperature: %d celsius. Fill Duration xx minutes.", frostParam.frostPreventTempThresh, currTempVal.tds2Temp);
		return TdsWarn.Tds2_Temp = isWarnActive(TDS2_TEMPERATURE);
	}
#if LL_TCHS_MENU == 1

	// if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, DEL_TDS_TEMPERATURE) && currTempVal.tds4Temp  <= frostParam.frostPreventTempThresh)
	if (currTempVal.tds4Temp  <= frostParam.frostPreventTempThresh && TdsWarn.Tds4_Temp)
	{
		if (isWarnActive(TDS4_TEMPERATURE))
			menu.Display_Log("DEL Temperature too low \"%d\"*C- system at danger of freezing.", currTempVal.tds4Temp );
		Log.logToMaster("During Auto-Fill, DEL Minimum Temperature threshold triggered.Threshold Temperature: %d celsius."
						"Actual Temperature: %d celsius. Fill Duration xx minutes.", frostParam.frostPreventTempThresh, currTempVal.tds4Temp );
		return TdsWarn.Tds4_Temp = isWarnActive(TDS4_TEMPERATURE);
	}
#else
	currTempVal.tds3Temp = tds3.readTemp();
	if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, DI_TDS_TEMPERATURE) && currTempVal.tds3Temp  <= frostParam.frostPreventTempThresh &&TdsWarn.Tds3_Temp)
	{
		if (isWarnActive(TDS3_TEMPERATURE))
			menu.Display_Log("DI RESIN Temperature too low \"%d\"*C- system at danger of freezing.", currTempVal.tds3Temp);
		Log.logToMaster("During Auto-Fill, DI Minimum Temperature threshold triggered.Threshold Temperature: %d celsius."
						"Actual Temperature: %d celsius. Fill Duration xx minutes.", frostParam.frostPreventTempThresh, currTempVal.tds3Temp);
		return TdsWarn.Tds3_Temp = isWarnActive(TDS3_TEMPERATURE);
	}
#endif

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


void stopAutoFillCycle(void)
{
	Flags.setFlag(Settings, MEM_SAVER, false);
	Flags.setFlag(Settings, MEM_SAVER_PAUSE, false);
	invokeST3(false);
	invokeST5(false);
}

void checkTdsShutdownWarnings(void)
{
	TdsWarn.Tds_Shutdown = false;
	if (isTDSConnected(TDS1) && Flags.flagStatus(Settings, MAINS_TDS_THRESHOLD) && Flags.flagStatus(Settings, MAINS_TDS_SHUTDOWN) && setTDS.mainsThreshold < tds1.readTDS())
	{
		menu.Display_Log("Tank Full Purification Stopped - Mains Input TDS too High", setTDS.mainsThreshold);
		Log.logToMaster("During Auto-Fill, Mains TDS & Temperature shutdown triggered. Threshold TDS/Temperature: %d ppm / %d celsius."
						"Actual TDS/Temperature: %d ppm, xx celsius. Fill Duration xx minutes.",
						setTDS.mainsThreshold, setTDS.mainsTemperature, tds1.readTDS());
		TdsWarn.Tds_Shutdown = true;
	}
	if (Flags.flagStatus(Status, AUTO_FILL) && !Flags.flagStatus(Status, AUTO_FLUSH))
	{
		if (isTDSConnected(TDS2) && Flags.flagStatus(Settings, RO_TDS_THRESHOLD) && Flags.flagStatus(Settings, RO_TDS_SHUTDOWN) && (setTDS.roFixedThreshold < tds2.readTDS() || setTDS.roDynamicThreshold < roRejectionPc()))
		{
			menu.Display_Log("Tank Full Purification Stopped - RO Membrane TDS Output too High");
			Log.logToMaster("During Auto-Fill, RO TDS & Temperature shutdown triggered. Threshold TDS/Temperature: %d ppm / %d celsius."
							"Actual TDS/Temperature: %d ppm, xx celsius. Fill Duration xx minutes.",
							setTDS.roFixedThreshold, setTDS.roTemperature, tds2.readTDS());
			TdsWarn.Tds_Shutdown = true;
		}
		if (isTDSConnected(TDS3) && Flags.flagStatus(Settings, DI_TDS_THRESHOLD) && Flags.timeElapsed(AUTO_FLUSH, FINISH_5M) && Flags.flagStatus(Settings, DI_TDS_SHUTDOWN) && setTDS.diThreshold < tds3.readTDS())
		{
			menu.Display_Log("Tank Full Purification Stopped - DI RESIN too High");
			Log.logToMaster("During Auto-Fill, DI TDS & Temperature shutdown triggered. Threshold TDS/Temperature: %d ppm / %d celsius."
							"Actual TDS/Temperature: %d ppm, xx celsius. Fill Duration xx minutes.",
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
		invokeST4(false);
		DEBUG_RTT(LOG_WARNING, "******");
	}
	
	// if (Flags.flagStatus(Status, BATTERY_HIGH) || Flags.flagStatus(Status, BATTERY_CHARGING_DETECTED))	
	// {
		isShowWarnMsg = checkTdsTemperatureWarnings();
	// }	
	if (Flags.flagStatus(Status, AUTO_FILL) || Flags.flagStatus(Status, AUTO_FLUSH))
	{
		isShowWarnMsg = checkTdsThresholdWarnings();
		checkTdsShutdownWarnings();
	}
	
	
#if LL_TCHS_MENU == 1
	if(TdsWarn.Tds1_Temp || TdsWarn.Tds2_Temp || TdsWarn.Tds4_Temp)
	{

		DEBUG_RTT(LOG_WARNING, "***TDS PROBE TEMP EXCEEDED THRESHOLD***");
		if(Flags.flagStatus(Settings, FROST_PREVENT))
		{
			DEBUG_RTT(LOG_INFO, "***STARTING FROST PREVENTION SEQUENCE***");		
			Flags.setFlag(Status , FROST_PREVENT , true);
		}
		else
		{
			DEBUG_RTT(LOG_INFO, "***CANNOT START FROST PREVENTION SEQUENCE (SWITCHED OFF)***");	
			Flags.setFlag(Status , FROST_PREVENT , false);
			// frostParam.frostPreventTempThresh
			if(frostParam.tempFormat)
			{
				menu.Display_Log("Frost Prevention is Deactivated - System is at Risk of Freezing\n TDS1 TEMP = %d , TDS2 TEMP = %d ,TDS4 TEMP = %d" ,currTempVal.tds1Temp,currTempVal.tds2Temp,currTempVal.tds4Temp);
			}	
			else if(!frostParam.tempFormat)
			{
				menu.Display_Log("Frost Prevention is Deactivated - System is at Risk of Freezing\n TDS1 TEMP = %d , TDS2 TEMP = %d ,TDS4 TEMP = %d" ,
					frostParam.celsiusToFahrenheit(currTempVal.tds1Temp),frostParam.celsiusToFahrenheit(currTempVal.tds2Temp),frostParam.celsiusToFahrenheit(currTempVal.tds4Temp));

				}
				menu.Display_Log("Alert to Mobile App/Send Text to Alert User"); /*Alert sent on APP*/					
			menu.Display_Log("Do you wish to activate Frost Prevention?");
			menu.changeMenu(&frostPreventOff);
		}

	}
#else
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
#endif
}
void fillOpen()
{
	Disp->display("Tank Full - System not Filling");
	Buzzzer->setOutputPattern(1, 500, 1000);
	((RgbDriver*)WLED1)->setRgbColor(RED);
	WLED1->setOutputPattern(1, 1000, 1000);

	invokeST3(false);
	invokeST5(false);
	Log.logToMaster("Float Circuit Open - Unable to commence filling");
	mainMenuFuncHeater();
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

void invokeDT1(bool status)
{
	if(Flags.flagStatus(Status, MAIN_HEATER) != status)
	{
		Flags.setFlag(Status, MAIN_HEATER, status);
	}
}

void mainHeaterOn()
{
	Flags.update();

	if (Flags.flagStatus(Settings, MAIN_HEATER))
	{	
		// invokeDT1(false);
		// invokeST7(true);
		/*heater logic*/
		// DEBUG_RTT(LOG_INFO, " MAIN HEATER STATUS FLAG %d" , Flags.flagStatus(Status, MAIN_HEATER));
		if(Flags.flagStatus(Status, MAIN_HEATER) == false)
		{
			// DEBUG_RTT(LOG_INFO, "Main Heater Turned ON");
			bool heaterInterface= true;
			if(heaterInterface) /*turn on or initialize heater*/
			{
		        DEBUG_RTT(LOG_INFO, "Main Heater Turned ON");
				Log.logToMaster("Main Heater Turned ON");
				invokeDT1(true);
			}
			else
			{
				DEBUG_RTT(LOG_INFO, "Main Heater Failed to Turn ON");
				Log.logToMaster("Main Heater Failed to Turn ON");
			}
		}
		
		else if(Flags.flagStatus(Status, MAIN_HEATER) == true)
		{
			if(Flags.flagStatus(Status, PUMP_1) ==  true) //disconnected pump
			{
					/*RGB LED LOGIC*/
					Disp->display("(Flashing ! Icon) Heater ON Water not heating reconnect reel hose");
			}
			else if (Flags.flagStatus(Status, PUMP_1) == false && !menuSwitched) // connected pump
			{
				if (lastTick == 0) 
				{
    				lastTick = HAL_GetTick();  // start time in ms
				}

				if (HAL_GetTick() - lastTick >= frostParam.preHeatingTime) /* 10 min delay*/
				{
					/*RGB LED LOGIC*/		
					Disp->display("(Flame Icon) Heater ON");
				}
				else
				{
					Disp->display("(Flashing Flame Icon) Preheating");
				}
			}
		}
		
	}
	else if(Flags.flagStatus(Settings, MAIN_HEATER) == false)
	{
		if(Flags.flagStatus(Status, MAIN_HEATER) ==  true)
		{
			/* turn off heater*/
			DEBUG_RTT(LOG_INFO, "Main Heater powered off");
			invokeDT1(false);
			Log.logToMaster("Main Heater powered off");
		}
	}
}

void controlPeripherals(void)
{
	mainHeaterOn();
	monitorTdsCycle();
}


    //     if(!pumpState)
    //     {
    //         Disp->display("(Flashing ! Icon) Heater ON Water not heating reconnect reel hose");
    //     }
    //     else if(HAL_GetTick() - lastTick < delayTime)
    //     {
    //         lastTick = HAL_GetTick();
    //         Disp->display("(Flashing Flame Icon) Preheating");
    //     }
    //     else
    //     {
    //         Disp->display("(Flame Icon) Heater ON");
    //     }











#endif
