#include "menu_02.h"
#include "controlsequence.h"
#include "masterLog.h"
#include "flagHandle.h"

#if MENU_02 == 1

#define MAX_TIME 60
#define MAX_PROD_TIME 150
#define DEFAULT_PROD_TIME 50
#define DEFAULT_TIME 10
#define MIN_TIME 05
#define DIFF_TIME 05

autoFlushTimers flushTimer;

// Helper function to check if the menu label matches the target threshold
bool isMenuLabelMatch02(MenuItem *menu, const MenuItem &targetThreshold, uint8_t index = 0)
{
	return strcmp(menu->Items[index].label, targetThreshold.Items[index].label) == 0;
}

bool isMenuMatch02(MenuItem *menu1, const MenuItem &menu2, uint8_t index = 0)
{
	return menu1->Items[index].prev == menu2.Items[index].prev &&
		   menu1->Items[index].next == menu2.Items[index].next &&
		   menu1->Items[index].setup == menu2.Items[index].setup &&
		   menu1->Items[index].DoWork == menu2.Items[index].DoWork;
}

void introSetup_02(void)
{
	setButtonStates({NONE}, true);
}

bool introPageSelection_02(uint32_t argChoice, MenuItem* argMenu)
{
	if (ButtonR.state() && argMenu->Items[choiceSelect].prev == nullptr)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}

void mainMenuSetup_02(void)
{
	setButtonStates({BACK}, true);
}

bool mainMenuSelection_02(uint32_t argChoice, MenuItem *argMenu)
{
	uint8_t numItems = argMenu->Items[choiceSelect].num_items;

	if ((ButtonE.period() > LONG_PRESS || ButtonD.period() > LONG_PRESS)
		&& (ButtonE.period() < LONG_LONG_PRESS || ButtonD.period() < LONG_LONG_PRESS))
	{
		argMenu = (MenuItem *)argMenu->Items[choiceSelect].next;
		numItems = argMenu->Items[choiceSelect].num_items;
		menu.displayMenu(argMenu, numItems);
		currentM = argMenu;

		ButtonE.reset();
		ButtonD.reset();
		return true;
	}

	else if (ButtonE.period() > MINIMUM_PRESS && ButtonE.period() < SHORT_PRESS
			&& !Flags.flagStatus(Status, RESIN_SAVER) && !Flags.flagStatus(Status, AUTO_FILL))
	{
		menu.changeMenu(&fodOptnsOff02);
		ButtonE.reset();
	}
	return false;
}

void fodMenuSetup_02(void)
{
	setButtonStates({RODI}, true);
}

void fodOffSetup_02(void)
{
	roSaverSetup_02();
}

void roSaverSetup_02()
{
	setButtonStates({BACK}, true);
}

bool fodOFF_02(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
		if (Flags.flagStatus(Settings, FoD))
		{
			Flags.setFlag(Settings, FoD, false);
			DEBUG_RTT(LOG_WARNING, "Fill on Demand disabled");
		}
		if (ButtonE.period() > SHORT_PRESS && ButtonE.period() < LONG_LONG_PRESS)
		{
			Disp->display("System Purification stopped.");
			Flags.setFlag(Settings, MEM_SAVER, false);
			Flags.setFlag(Settings, MEM_SAVER_PAUSE, false);
			invokeST3(false);
			invokeST5(false);
//			mainMenuFunc02();
		}
		else if (ButtonE.period() < SINGLE_PRESS)
		{
			Flags.setFlag(Settings, MEM_SAVER_PAUSE, false);
			if (Flags.flagStatus(Status, LEVEL_SENSOR) == true)
			{
				Flags.setFlag(Settings, MEM_SAVER, false);
				fillOpen();
			}
			else if (!Flags.flagStatus(Settings, MEM_SAVER))
			{
				invokeST3(true);
				invokeST5(true);
				// calCurrentTdsReading();
				perpStatus.diResinSaver = false;
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

void backAndForthSetup_02(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

void selectMustSetup_02(void)
{
	setButtonStates({ENTER_OK}, true);
}

bool TdsSettingsOff(uint32_t argChoice, MenuItem *argMenu)
{
	DEBUG_RTT(LOG_INFO, "TDS settings disabled");

	if (isMenuLabelMatch02(argMenu, mainsTDSThreshold, 1))
	{
		Flags.setFlag(Settings, MAINS_TDS_THRESHOLD, false);
	}
	else if (isMenuLabelMatch02(argMenu, roTDSThreshold, 1))
	{
		Flags.setFlag(Settings, RO_TDS_THRESHOLD, false);
	}
	else if (isMenuLabelMatch02(argMenu, diTDSThreshold, 1))
	{
		Flags.setFlag(Settings, DI_TDS_THRESHOLD, false);
	}
	else
	{
		return false;
	}

	return true;
}

bool tempSettingsOff(uint32_t argChoice, MenuItem *argMenu)
{
	DEBUG_RTT(LOG_INFO, "Temp settings disabled");

	if (isMenuMatch02(argMenu, mainsTDSThreshold))
	{
		Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, false);
	}
	else if (isMenuMatch02(argMenu, roTDSThreshold))
	{
		Flags.setFlag(Settings, RO_TDS_TEMPERATURE, false);
	}
	else if (isMenuMatch02(argMenu, diTDSThreshold))
	{
		Flags.setFlag(Settings, DI_TDS_TEMPERATURE, false);
	}
	else
	{
		return false;
	}

	return true;
}

bool TdsDefaultThresholdValue(uint32_t argChoice, MenuItem *argMenu)
{
	if (strcmp(argMenu->Items[1].label, mainsTDSThreshold.Items[1].label) == 0)
	{
		Flags.setFlag(Settings, MAINS_TDS_THRESHOLD, true);
		setTDS.mainsThreshold = DEFAULT_MAINS_TDS_THRESHOLD;
		DEBUG_RTT(LOG_INFO, "Mains TDS threshold value set to %d ppm", setTDS.mainsThreshold);
	}
	else if (strcmp(argMenu->Items[1].label, diTDSThreshold.Items[1].label) == 0)
	{
		Flags.setFlag(Settings, DI_TDS_THRESHOLD, true);
		setTDS.diThreshold = DEFAULT_DI_TDS_THRESHOLD;
		DEBUG_RTT(LOG_INFO, "DI TDS threshold value set to default ppm", setTDS.diThreshold);
	}
	else
		return false;

	return true;
}

void navigationTrue(void)
{
	DEBUG_RTT(LOG_INFO, "navigationTrue:");
	setButtonStates({ENTER_OK, RODI}, false);
}

bool TDSThresholdAdjustedValue(MenuItem *argMenu)
{
	static bool mainsTdsMenu = isMenuLabelMatch02(argMenu, mainsTDSThreshold,2);

	uint8_t minThreshold = mainsTdsMenu ? MIN_MAINS_TDS_THRESHOLD : MIN_DI_TDS_THRESHOLD;
	uint8_t maxThreshold = mainsTdsMenu ? MAX_MAINS_TDS_THRESHOLD : MAX_DI_TDS_THRESHOLD;
	uint8_t diffThreshold = mainsTdsMenu ? DIFF_MAINS_TDS_THRESHOLD : DIFF_DI_TDS_THRESHOLD;

	static uint8_t threshVal = minThreshold;

	if (ButtonD.state() && threshVal > minThreshold)
	{
		threshVal -= diffThreshold;
		Disp->display(threshVal);
	}
	else if (ButtonU.state() && threshVal < maxThreshold)
	{
		threshVal += diffThreshold;
		Disp->display(threshVal);
	}


	if (ButtonE.state())
	{
		Flags.setFlag(Settings, mainsTdsMenu ? MAINS_TDS_THRESHOLD : DI_TDS_THRESHOLD, true);
		if (mainsTdsMenu)
		{
			setTDS.mainsThreshold = threshVal;
		}
		else
		{
			setTDS.diThreshold = threshVal;
		}

		DEBUG_RTT(LOG_WARNING, "Threshold value set to %d.", threshVal);
		threshVal = minThreshold;
		mainsTdsMenu = false;

		return true;
	}
	return false;
}

bool TDSThresholdLoop(uint32_t argChoice, MenuItem *argMenu)
{
	static bool isThresholdAdjustable = false;

	if (!isThresholdAdjustable && ButtonE.state())
	{
		switch (argChoice)
		{
		case 1:
			return TdsSettingsOff(argChoice, argMenu);
		case 2:
			return TdsDefaultThresholdValue(argChoice, argMenu);
		case 3:
			if (isMenuLabelMatch02(argMenu, mainsTDSThreshold, 2))
			{
				Disp->display("Select Threshold value from 50-600ppm");
				isThresholdAdjustable = true;
			}
			else if (isMenuLabelMatch02(argMenu, diTDSThreshold, 2))
			{
				Disp->display("Select Threshold value from 1-15ppm");
				isThresholdAdjustable = true;
			}
			else
				return false;

			backAndForthSetup_02();
			ButtonE.reset();
			break;
		}
	}
	else if (isThresholdAdjustable)
	{
		isThresholdAdjustable = !TDSThresholdAdjustedValue(argMenu);
		return !isThresholdAdjustable; 
	}

	return false;
}

bool TdsDefaultTemperatureValue(uint32_t argChoice, MenuItem *argMenu)
{

	if (isMenuMatch02(argMenu, mainsTDSTemperature))
	{
		Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, true);
		setTDS.mainsTemperature = DEFAULT_TDS_TEMPERATURE;
		DEBUG_RTT(LOG_INFO, "Mains TDS temp value set to default %d*C", setTDS.mainsTemperature);
	}
	else if (isMenuMatch02(argMenu, roTDSTemperature))
	{
		Flags.setFlag(Settings, RO_TDS_TEMPERATURE, true);
		setTDS.roTemperature = DEFAULT_TDS_TEMPERATURE;
		DEBUG_RTT(LOG_INFO, "RO TDS temp value set to default %d*C", setTDS.roTemperature);
	}
	else if (isMenuMatch02(argMenu, diTDSTemperature))
	{
		Flags.setFlag(Settings, DI_TDS_TEMPERATURE, true);
		setTDS.diTemperature = DEFAULT_TDS_TEMPERATURE;
		DEBUG_RTT(LOG_INFO, "DI TDS temp value set to default %d*C", setTDS.diTemperature);
	}
	else
	{
		return false;
	}
	return true;
}

bool TDSTemperatureAdjustedValue(MenuItem *argMenu)
{
	static uint8_t tempVal = MIN_TDS_TEMPERATURE;

	if (ButtonD.state() && tempVal > MIN_TDS_TEMPERATURE)
	{
		tempVal -= DIFF_TDS_TEMPERATURE;
		Disp->display(tempVal);
	}
	else if (ButtonU.state() && tempVal < MAX_TDS_TEMPERATURE)
	{
		tempVal += DIFF_TDS_TEMPERATURE;
		Disp->display(tempVal);
	}

	if (ButtonE.state())
	{
		if (isMenuLabelMatch02(argMenu, mainsTDSTemperature, 1))
		{
			Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, true);
			setTDS.mainsTemperature = tempVal;
		}
		else if (isMenuLabelMatch02(argMenu, roTDSTemperature, 1))
		{
			Flags.setFlag(Settings, RO_TDS_TEMPERATURE, true);
			setTDS.roTemperature = tempVal;
		}
		else if (isMenuLabelMatch02(argMenu, diTDSTemperature, 1))
		{
			Flags.setFlag(Settings, DI_TDS_TEMPERATURE, true);
			setTDS.diTemperature = tempVal;
		}
		else
		{
			return false;
		}

		DEBUG_RTT(LOG_WARNING, "Temperature value set to %d.", tempVal);
		tempVal = MIN_TDS_TEMPERATURE;
		return true;
	}
}

bool TDSTemperatureLoop(uint32_t argChoice, MenuItem *argMenu)
{
	static bool isTemperatureAdjustable = false;

	if (!isTemperatureAdjustable && ButtonE.state())
	{
		switch (argChoice)
		{
		case 1:
			return tempSettingsOff(argChoice, argMenu);
		case 2:
			return TdsDefaultTemperatureValue(argChoice, argMenu);
		case 3:
			Disp->display("Select Temperature value from 0-10*C");
			isTemperatureAdjustable = true;
			backAndForthSetup_02();
			ButtonE.reset();
			break;
		}
	}
	else if (isTemperatureAdjustable)
	{
		isTemperatureAdjustable = !TDSTemperatureAdjustedValue(argMenu);
		return !isTemperatureAdjustable;
	}

	return false;
}

bool TdsShutdown(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
	 	bool isShutdownEnable = argChoice == 2 ? true : false;

		if (isMenuMatch02(argMenu, mainsTDSShutdown))
		{
			Flags.setFlag(Settings, MAINS_TDS_SHUTDOWN, isShutdownEnable);
		}
		else if (isMenuMatch02(argMenu, roTDSShutdown))
		{
			Flags.setFlag(Settings, RO_TDS_SHUTDOWN, isShutdownEnable);
		}
		else if (isMenuMatch02(argMenu, diTDSShutdown))
		{
			Flags.setFlag(Settings, DI_TDS_SHUTDOWN, isShutdownEnable);
		}
		DEBUG_RTT(LOG_INFO, "TDS shutdown settings : %d",isShutdownEnable);
	}
	else
	{
		return false;
	}
	return true;
}

bool RoTdsThresholdAdj(bool dynamicTdsMenu)
{
	uint8_t minThreshold = dynamicTdsMenu ? MIN_DYNAMIC_TDS_THRESHOLD : MIN_FIXED_TDS_THRESHOLD;
	uint8_t maxThreshold = MAX_RO_TDS_THRESHOLD;
	uint8_t diffThreshold = DIFF_RO_TDS_THRESHOLD;

	static uint8_t threshVal = minThreshold;
	static bool lastMenu = dynamicTdsMenu;

	if (lastMenu != dynamicTdsMenu)
	{
		lastMenu = dynamicTdsMenu;
		threshVal = minThreshold;
	}

	if (ButtonD.state() && threshVal > minThreshold)
	{
		threshVal -= diffThreshold;
		Disp->display(threshVal);
	}
	else if (ButtonU.state() && threshVal < maxThreshold)
	{
		threshVal += diffThreshold;
		Disp->display(threshVal);
	}

	if (ButtonE.state())
	{
		Flags.setFlag(Settings, RO_TDS_THRESHOLD, true);
		if (dynamicTdsMenu)
		{
			setTDS.mainsThreshold = threshVal;
		}
		else
		{
			setTDS.diThreshold = threshVal;
		}

		DEBUG_RTT(LOG_WARNING, "Ro Threshold value set to %d.", threshVal);
		threshVal = minThreshold;

		return true;
	}
	return false;
}

bool setRoTdsThreshold(uint32_t argChoice, MenuItem *argMenu)
{
	static bool isRoTdsAdjustable = false;
	if (!ButtonE.state() && !isRoTdsAdjustable)	return false;

	if (argChoice == 1 && !isRoTdsAdjustable)
	{
		if (isMenuLabelMatch02(argMenu, roDynamicTDSThreshold))
		{
			setTDS.roDynamicThreshold = DEFAULT_DYNAMIC_TDS_THRESHOLD;
		}
		else if (isMenuLabelMatch02(argMenu, roFixedTDSThreshold))
		{
			setTDS.roFixedThreshold = DEFAULT_FIXED_TDS_THRESHOLD;
		}
		else
		{
			return false;
		}
		Flags.setFlag(Settings, RO_TDS_THRESHOLD, true);
		return true;
	}
	else if (argChoice == 2 && !isRoTdsAdjustable)
	{
		isRoTdsAdjustable = true;
		backAndForthSetup_02();
		ButtonE.reset();
		if (isMenuLabelMatch02(argMenu, roDynamicTDSThreshold))
			Disp->display("Select RO Dynamic value from 50-99%");
		else if (isMenuLabelMatch02(argMenu, roFixedTDSThreshold))
			Disp->display("Select RO Fixed value from 1-100ppm");
	}
	else if (isRoTdsAdjustable)
	{
		if (isMenuLabelMatch02(argMenu, roDynamicTDSThreshold))
		{
			isRoTdsAdjustable = !RoTdsThresholdAdj(true);
		}
		else if (isMenuLabelMatch02(argMenu, roFixedTDSThreshold))
		{
			isRoTdsAdjustable = !RoTdsThresholdAdj(false);
		}
		else
			return false;

		return !isRoTdsAdjustable;
	}

	return false;
}

bool haltMenuLoop(uint32_t argChoice, MenuItem *argMenu)
{
	char warningTDS[30] = {0};
	if (TdsWarn.Tds2_FixedThresh && ButtonU.state() && Flags.flagStatus(Settings, AUTO_FLUSH))
	{
		invokeST4(true);
		menu.changeMenu(&mainMenu02);
		Log.logToMaster("User pressed UP key to override RO TDS threshold warning and Start Auto Flush.");
		return TdsWarn.all_flags = 0;
	}
	else if (ButtonE.state())
	{
		if(TdsWarn.Tds1_Thresh) 
		{tempTdsWarning.Tds1_Thresh = false; strncpy(warningTDS,"Mains TDS threshold", sizeof(warningTDS) - 1);}
		if(TdsWarn.Tds1_Temp)
		{tempTdsWarning.Tds1_Temp = false; strncpy(warningTDS,"Mains Temperature threshold", sizeof(warningTDS) - 1);}
		if(TdsWarn.Tds2_DynamicThresh)
		{tempTdsWarning.Tds2_DynamicThresh = false; strncpy(warningTDS,"RO rejection", sizeof(warningTDS) - 1);}
		if(TdsWarn.Tds2_FixedThresh)
		{tempTdsWarning.Tds2_FixedThresh = false; strncpy(warningTDS,"RO TDS threshold", sizeof(warningTDS) - 1);}
		if(TdsWarn.Tds2_Temp)
		{tempTdsWarning.Tds2_Temp = false; strncpy(warningTDS,"RO Temperature threshold", sizeof(warningTDS) - 1);}
		if(TdsWarn.Tds3_Thresh)
		{tempTdsWarning.Tds3_Thresh = false; strncpy(warningTDS,"DI RESIN TDS threshold", sizeof(warningTDS) - 1);}
		if(TdsWarn.Tds3_Temp)
		{tempTdsWarning.Tds3_Temp = false; strncpy(warningTDS,"DI Temperature threshold", sizeof(warningTDS) - 1);}

		menu.changeMenu(&mainMenu02);
		TdsWarn.all_flags = 0;
	}
	else
	{
		return false;
	}

	Log.logToMaster("User pressed enter key to override %s warning.", warningTDS);
	return true;
}

bool autoFlushOptions(uint32_t argChoice, MenuItem *argMenu)
{
	if (argChoice < 3 && ButtonE.state())
	{
		autoFlushEnable(argChoice == 1 ? false : true);
	}
	else
		return false;
}

bool autoFlushEnable(bool activateST4)
{
	Flags.setFlag(Settings, AUTO_FLUSH, activateST4);
	DEBUG_RTT(LOG_WARNING, "AUTO_FLUSH setting Turned %s.", activateST4 ? "On" : "Off");
	return true;
}


bool configureTdsWarning(uint32_t argChoice, MenuItem *argMenu)
{
	static uint8_t index = 0;
	if(isMenuMatch02(argMenu, tdsWarningList))
	{
		index = argChoice - 1;
		return true;
	}
	else if (isMenuMatch02(argMenu, SetWarnState) && ButtonE.state())
	{
		bool bitSet = false;

		if (argChoice == 1)			bitSet = false;
		else if (argChoice == 2)	bitSet = true;
		else						return false;

		if (bitSet)		isTdsWarningEnable.all_flags |= (1 << index); // Set the bit
		else			isTdsWarningEnable.all_flags &= ~(1 << index); // Clear the bit

		DEBUG_RTT(LOG_INFO, "Index : %d and bit: %d and warn %d", index, bitSet, isTdsWarningEnable.all_flags);
		index = 0;
		return true;
	}
	else
		return false;
}

bool selectTdsWarningItem(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state()) configureTdsWarning(argChoice, argMenu);
}

bool diSaverEnable(bool activateST1)
{
	Flags.setFlag(Settings, RESIN_SAVER, activateST1);
	DEBUG_RTT(LOG_WARNING, "RESIN_SAVER setting Turned %s.", activateST1 ? "On" : "Off");
	return true;
}

bool diResinSaverAdjValue(void)
{
	static uint8_t resinSaverIntervalVal = 0;

	if (ButtonD.state())
	{
		if (resinSaverIntervalVal > 0)
			resinSaverIntervalVal--;
		Disp->display(resinSaverIntervalVal);
	}
	else if (ButtonU.state())
	{
		if (resinSaverIntervalVal < 10)
			resinSaverIntervalVal++;
		Disp->display(resinSaverIntervalVal);
	}
	else if (ButtonE.state())
	{
		setST.diResinSvrIntrvl = resinSaverIntervalVal;
		diSaverEnable(true);
		resinSaverIntervalVal = 0;
		DEBUG_RTT(LOG_WARNING, "Resin Saver Interval set to %d.", setST.diResinSvrIntrvl);
		return true;
	}
	else
		return false;
}

bool diSaverOptions(uint32_t argChoice, MenuItem *argMenu)
{
	static bool adjResinSvr = false;
	
	if(adjResinSvr)
	{
		adjResinSvr = !diResinSaverAdjValue();
	}
	else if (ButtonE.state() && argChoice == 1)
	{
		setST.diResinSvrIntrvl = DEFAULT_RESIN_SAVER_INTRVL;
		diSaverEnable(true);
		DEBUG_RTT(LOG_WARNING, "Resin Saver Interval set to %d.", setST.diResinSvrIntrvl);
	}
	else if (ButtonE.state() && argChoice == 2)
	{
		adjResinSvr = true;
		Disp->display("\nSelect DI Resin Saver Time Interval: 1 to 10 minutes");
		backAndForthSetup_02();
		ButtonE.reset();
	}
	else
		return false;


	return !adjResinSvr;
}

bool diSaverOnOff(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state() && argChoice < 3)
	{
		diSaverEnable(argChoice == 2 ? true : false);
		return true;
	}
	else
		return false;
}



// // Helper function to check if the menu label matches the target menu label
// bool isMenuLabelMatch(MenuItem *menu, const MenuItem &targetCountTime, uint8_t childMenu = 0)
// {
//     return strcmp(menu->Items[childMenu].label, targetCountTime.Items[childMenu].label) == 0;
// }

bool autoFlushTimerAdjValue(bool prodTimerMode)
{

	uint8_t minCountTime = flushTimer.selectTimer == PRODUCTION_TIMER ? DEFAULT_PROD_TIME : MIN_TIME;
	uint8_t maxCountTime = flushTimer.selectTimer == PRODUCTION_TIMER ? MAX_PROD_TIME : MAX_TIME;

	static uint8_t timerVal = minCountTime;
	static bool lastMenu = prodTimerMode;

	if (lastMenu != prodTimerMode)
	{
		lastMenu = prodTimerMode;
		timerVal = minCountTime;
	}

	if (ButtonD.state() && timerVal > minCountTime)
	{
		timerVal -= DIFF_TIME;
		Disp->display(timerVal);
	}
	else if (ButtonU.state() && timerVal < maxCountTime)
	{
		timerVal += DIFF_TIME;
		Disp->display(timerVal);
	}

	if (ButtonE.state())
	{
		if (flushTimer.selectTimer == STARTUP_TIMER)
		{
			flushTimer.startUpTimer = timerVal;
		}
		else if (flushTimer.selectTimer == PRODUCTION_TIMER)
		{
			flushTimer.productionTimer = timerVal;
		}
		else if (flushTimer.selectTimer == SUBSEQUENT_TIMER)
		{
			flushTimer.subsequentTimer = timerVal;
		}
		else
		{
			// Flags.setFlag(Settings, AUTO_FLUSH, false);
			return false;
		}

		DEBUG_RTT(LOG_WARNING, "Timer value set to %d.", timerVal);
		Flags.setFlag(Settings, AUTO_FLUSH, true);
		timerVal = MIN_TIME;
		flushTimer.selectTimer = NO_TIMER;
		return true;
	}
}

bool autoFlushTimerLoop(uint32_t argChoice, MenuItem *argMenu)
{
	static bool autoFlushTimer = false;

	if (!autoFlushTimer && ButtonE.state())
	{
		switch (argChoice)
		{
		case 1:
			Disp->display("Default time, 10 minutes is set");
			if(isMenuLabelMatch02(argMenu, initialFlushTimer))
				flushTimer.startUpTimer = DEFAULT_TIME;
			else if(isMenuLabelMatch02(argMenu, subsequentFlushTimer))
				flushTimer.subsequentTimer = DEFAULT_TIME;
			flushTimer.selectTimer = NO_TIMER;
				return true;
		
		case 2:
			Disp->display("Set time from 05 till 60 minutes:");
			if(isMenuLabelMatch02(argMenu, initialFlushTimer))
				flushTimer.selectTimer = STARTUP_TIMER;
			else if(isMenuLabelMatch02(argMenu, subsequentFlushTimer))
				flushTimer.selectTimer = SUBSEQUENT_TIMER;
			autoFlushTimer = true;
			ButtonE.reset();
			selectMustSetup_02();
			break;
		}
	}
	else if (autoFlushTimer)
	{
		autoFlushTimer = !autoFlushTimerAdjValue(false);
	}

	return !autoFlushTimer;
}

bool productionTimerLoop(uint32_t argChoice, MenuItem *argMenu)
{
	static bool productionTimer = false;

	if (!productionTimer && ButtonE.state())
	{
		switch (argChoice)
		{
		case 1:
			Disp->display("Default time, 50 minutes is set");
			flushTimer.productionTimer = DEFAULT_PROD_TIME;
			flushTimer.selectTimer = NO_TIMER;
			return true;
			
			case 2:
			Disp->display("Set time from 50 till 150 minutes");
			flushTimer.selectTimer = PRODUCTION_TIMER;
			productionTimer = true;
			ButtonE.reset();
			selectMustSetup_02();
			break;
		}
	}
	else if (productionTimer)
	{
		productionTimer = !autoFlushTimerAdjValue(true);
	}

	return !productionTimer;
}

#endif
