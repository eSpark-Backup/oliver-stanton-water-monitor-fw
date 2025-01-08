#include "menu_05.h"
#include "controlsequence.h"
#include "masterLog.h"

// Helper function to check if the menu label matches the target threshold
bool isMenuLabelMatch(MenuItem *menu, const MenuItem &targetThreshold, uint8_t index = 0)
{
	return strcmp(menu->Items[index].label, targetThreshold.Items[index].label) == 0;
}

bool isMenuMatch(MenuItem *menu1, const MenuItem &menu2, uint8_t index = 0)
{
	return menu1->Items[index].prev == menu2.Items[index].prev &&
		   menu1->Items[index].next == menu2.Items[index].next &&
		   menu1->Items[index].setup == menu2.Items[index].setup &&
		   menu1->Items[index].DoWork == menu2.Items[index].DoWork;
}

void introSetup_05(void)
{
	setButtonStates({NONE}, true);
}

bool introPageSelection_05(uint32_t argChoice, MenuItem* argMenu)
{
	if (ButtonR.state() && argMenu->Items[choiceSelect].prev == nullptr)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}

void mainMenuSetup_05(void)
{
	setButtonStates({BACK}, true);
}

bool mainMenuSelection_05(uint32_t argChoice, MenuItem *argMenu)
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
			&& !Flags.flagStatus(Status, RESIN_SAVER))
	{
		menu.changeMenu(&fodOptnsOff);
		ButtonE.reset();
	}
	return false;
}

void fodMenuSetup_05(void)
{
	setButtonStates({RODI}, true);
}

void fodOffSetup_05(void)
{
	roSaverSetup_05();
}

void roSaverSetup_05()
{
	setButtonStates({BACK}, true);
}

void backAndForthSetup_05(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

bool TdsSettingsOff(uint32_t argChoice, MenuItem *argMenu)
{
	DEBUG_RTT(LOG_INFO, "TDS settings disabled");

	if (isMenuLabelMatch(argMenu, mainsTDSThreshold, 1))
	{
		Flags.setFlag(Settings, MAINS_TDS_THRESHOLD, false);
	}
	else if (isMenuLabelMatch(argMenu, roTDSThreshold, 1))
	{
		Flags.setFlag(Settings, RO_TDS_THRESHOLD, false);
	}
	else if (isMenuLabelMatch(argMenu, diTDSThreshold, 1))
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

	if (isMenuMatch(argMenu, mainsTDSThreshold))
	{
		Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, false);
	}
	else if (isMenuMatch(argMenu, roTDSThreshold))
	{
		Flags.setFlag(Settings, RO_TDS_TEMPERATURE, false);
	}
	else if (isMenuMatch(argMenu, diTDSThreshold))
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
	static bool mainsTdsMenu = isMenuLabelMatch(argMenu, mainsTDSThreshold,2);

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
			if (isMenuLabelMatch(argMenu, mainsTDSThreshold, 2))
			{
				Disp->display("Select Threshold value from 50-600ppm");
				isThresholdAdjustable = true;
			}
			else if (isMenuLabelMatch(argMenu, diTDSThreshold, 2))
			{
				Disp->display("Select Threshold value from 1-15ppm");
				isThresholdAdjustable = true;
			}
			else
				return false;

			backAndForthSetup_05();
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

	if (isMenuMatch(argMenu, mainsTDSTemperature))
	{
		Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, true);
		setTDS.mainsTemperature = DEFAULT_TDS_TEMPERATURE;
		DEBUG_RTT(LOG_INFO, "Mains TDS temp value set to default %d*C", setTDS.mainsTemperature);
	}
	else if (isMenuMatch(argMenu, roTDSTemperature))
	{
		Flags.setFlag(Settings, RO_TDS_TEMPERATURE, true);
		setTDS.roTemperature = DEFAULT_TDS_TEMPERATURE;
		DEBUG_RTT(LOG_INFO, "RO TDS temp value set to default %d*C", setTDS.roTemperature);
	}
	else if (isMenuMatch(argMenu, diTDSTemperature))
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
		if (isMenuLabelMatch(argMenu, mainsTDSTemperature, 1))
		{
			Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, true);
			setTDS.mainsTemperature = tempVal;
		}
		else if (isMenuLabelMatch(argMenu, roTDSTemperature, 1))
		{
			Flags.setFlag(Settings, RO_TDS_TEMPERATURE, true);
			setTDS.roTemperature = tempVal;
		}
		else if (isMenuLabelMatch(argMenu, diTDSTemperature, 1))
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
			backAndForthSetup_05();
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

		if (isMenuMatch(argMenu, mainsTDSShutdown))
		{
			Flags.setFlag(Settings, MAINS_TDS_SHUTDOWN, isShutdownEnable);
		}
		else if (isMenuMatch(argMenu, roTDSShutdown))
		{
			Flags.setFlag(Settings, RO_TDS_SHUTDOWN, isShutdownEnable);
		}
		else if (isMenuMatch(argMenu, diTDSShutdown))
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
		if (isMenuLabelMatch(argMenu, roDynamicTDSThreshold))
		{
			setTDS.roDynamicThreshold = DEFAULT_DYNAMIC_TDS_THRESHOLD;
		}
		else if (isMenuLabelMatch(argMenu, roFixedTDSThreshold))
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
		backAndForthSetup_05();
		ButtonE.reset();
		if (isMenuLabelMatch(argMenu, roDynamicTDSThreshold))
			Disp->display("Select RO Dynamic value from 50-99%");
		else if (isMenuLabelMatch(argMenu, roFixedTDSThreshold))
			Disp->display("Select RO Fixed value from 1-100ppm");
	}
	else if (isRoTdsAdjustable)
	{
		if (isMenuLabelMatch(argMenu, roDynamicTDSThreshold))
		{
			isRoTdsAdjustable = !RoTdsThresholdAdj(true);
		}
		else if (isMenuLabelMatch(argMenu, roFixedTDSThreshold))
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
	if (TdsWarn.Tds2_FixedThresh && ButtonU.state())
	{
		autoFlushEnable(true);
		invokeST4(true);
		menu.changeMenu(&mainMenu05);
		Log.logToMaster("User pressed UP key to override RO TDS threshold warning and Start Auto Flush.");
		return TdsWarn.all_flags = 0;
	}
	else if (ButtonE.state())
	{
		if(TdsWarn.Tds1_Thresh) strncpy(warningTDS,"Mains TDS threshold", sizeof(warningTDS) - 1);
		if(TdsWarn.Tds1_Temp)	strncpy(warningTDS,"Mains Temperature threshold", sizeof(warningTDS) - 1);
		if(TdsWarn.Tds2_DynamicThresh) strncpy(warningTDS,"RO rejection", sizeof(warningTDS) - 1);
		if(TdsWarn.Tds2_FixedThresh) strncpy(warningTDS,"RO TDS threshold", sizeof(warningTDS) - 1);
		if(TdsWarn.Tds2_Temp) strncpy(warningTDS,"RO Temperature threshold", sizeof(warningTDS) - 1);
		if(TdsWarn.Tds3_Thresh) strncpy(warningTDS,"DI RESIN TDS threshold", sizeof(warningTDS) - 1);
		if(TdsWarn.Tds3_Temp) strncpy(warningTDS,"DI Temperature threshold", sizeof(warningTDS) - 1);

		menu.changeMenu(&mainMenu05);
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