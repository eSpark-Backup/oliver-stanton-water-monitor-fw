#include "menu_05.h"
#include "controls.h"


// Helper function to check if the menu label matches the target threshold
bool isMenuLabelMatch(MenuItem *menu, const MenuItem &targetThreshold)
{
	return strcmp(menu->Items[2].label, targetThreshold.Items[2].label) == 0;
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

	if (isMenuLabelMatch(argMenu, mainsTDSThreshold) == 0)
	{
		Flags.setFlag(Settings, MAINS_TDS_THRESHOLD, false);
		Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, false);
	}
	else if (isMenuLabelMatch(argMenu, roTDSThreshold))
	{
		Flags.setFlag(Settings, RO_TDS_THRESHOLD, false);
		Flags.setFlag(Settings, RO_TDS_TEMPERATURE, false);
	}
	else if (isMenuLabelMatch(argMenu, diTDSThreshold))
	{
		Flags.setFlag(Settings, DI_TDS_THRESHOLD, false);
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
	DEBUG_RTT(LOG_INFO, "TDS threshold value set to default ppm");

	if (strcmp(argMenu->Items[1].label, mainsTDSThreshold.Items[1].label) == 0)
	{
		Flags.setFlag(Settings, MAINS_TDS_THRESHOLD, true);
		setTDS.mainsThreshold = DEFAULT_MAINS_TDS_THRESHOLD;
		return true;
	}
	else if (strcmp(argMenu->Items[1].label, diTDSThreshold.Items[1].label) == 0)
	{
		Flags.setFlag(Settings, DI_TDS_THRESHOLD, true);
		setTDS.diThreshold = DEFAULT_DI_TDS_THRESHOLD;
		return true;
	}
	return false;
}

bool TDSThresholdAdjustedValue(bool mainsTdsMenu)
{
	uint8_t minThreshold = mainsTdsMenu ? MIN_MAINS_TDS_THRESHOLD : MIN_DI_TDS_THRESHOLD;
	uint8_t maxThreshold = mainsTdsMenu ? MAX_MAINS_TDS_THRESHOLD : MAX_DI_TDS_THRESHOLD;
	uint8_t diffThreshold = mainsTdsMenu ? DIFF_MAINS_TDS_THRESHOLD : DIFF_DI_TDS_THRESHOLD;

	static uint8_t threshVal = minThreshold;

	if (ButtonD.state() && threshVal > minThreshold)
	{
		threshVal -= diffThreshold;
	}
	else if (ButtonU.state() && threshVal < maxThreshold)
	{
		threshVal += diffThreshold;
	}

	Disp->display(threshVal);

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
			if (isMenuLabelMatch(argMenu, mainsTDSThreshold))
			{
				Disp->display("Select Threshold value from 50-600ppm");
				isThresholdAdjustable = true;
			}
			else if (isMenuLabelMatch(argMenu, diTDSThreshold))
			{
				Disp->display("Select Threshold value from 1-15ppm");
				isThresholdAdjustable = true;
			}
			backAndForthSetup_05();
			break;
		}
	}
	else if (isThresholdAdjustable)
	{
		return TDSThresholdAdjustedValue(isMenuLabelMatch(argMenu, mainsTDSThreshold));
	}

	return false;
}

bool TdsDefaultTemperatureValue(uint32_t argChoice, MenuItem *argMenu)
{
	DEBUG_RTT(LOG_INFO, "TDS Temperature value set to default 2*C");

	if (isMenuLabelMatch(argMenu, mainsTDSTemperature))
	{
		Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, true);
		setTDS.mainsTemperature = DEFAULT_TDS_TEMPERATURE;
	}
	else if (isMenuLabelMatch(argMenu, roTDSTemperature))
	{
		Flags.setFlag(Settings, RO_TDS_TEMPERATURE, true);
		setTDS.roTemperature = DEFAULT_TDS_TEMPERATURE;
	}
	else if (isMenuLabelMatch(argMenu, diTDSTemperature))
	{
		Flags.setFlag(Settings, DI_TDS_TEMPERATURE, true);
		setTDS.diTemperature = DEFAULT_TDS_TEMPERATURE;
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
	}
	else if (ButtonU.state() && tempVal < MAX_TDS_TEMPERATURE)
	{
		tempVal += DIFF_TDS_TEMPERATURE;
	}

	Disp->display(tempVal);
	if (ButtonE.state())
	{
		if (isMenuLabelMatch(argMenu, mainsTDSTemperature))
		{
			Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, true);
			setTDS.mainsTemperature = tempVal;
		}
		else if (isMenuLabelMatch(argMenu, roTDSTemperature))
		{
			Flags.setFlag(Settings, RO_TDS_TEMPERATURE, true);
			setTDS.roTemperature = tempVal;
		}
		else if (isMenuLabelMatch(argMenu, diTDSTemperature))
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
			return TdsSettingsOff(argChoice, argMenu);
		case 2:
			return TdsDefaultTemperatureValue(argChoice, argMenu);
		case 3:
			Disp->display("Select Temperature value from 0-10*C");
			isTemperatureAdjustable = true;
			backAndForthSetup_05();
			break;
		}
	}
	else if (isTemperatureAdjustable)
	{
		return TDSTemperatureAdjustedValue(argMenu);
	}

	return false;
}

bool TdsShutdown(uint32_t argChoice, MenuItem *argMenu)
{
	DEBUG_RTT(LOG_INFO, "TDS shutdown settings");
	bool isShutdownEnable = (argChoice == 2 && ButtonE.state());

	if (isMenuLabelMatch(argMenu, mainsTDSShutdown))
	{
		Flags.setFlag(Settings, MAINS_TDS_SHUTDOWN, isShutdownEnable);
	}
	else if (isMenuLabelMatch(argMenu, roTDSShutdown))
	{
		Flags.setFlag(Settings, RO_TDS_SHUTDOWN, isShutdownEnable);
	}
	else if (isMenuLabelMatch(argMenu, diTDSShutdown))
	{
		Flags.setFlag(Settings, DI_TDS_SHUTDOWN, isShutdownEnable);
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

	if (ButtonD.state() && threshVal > minThreshold)
	{
		threshVal -= diffThreshold;
	}
	else if (ButtonU.state() && threshVal < maxThreshold)
	{
		threshVal += diffThreshold;
	}

	Disp->display(threshVal);

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

bool RoTdsThreshold(uint32_t argChoice, MenuItem *argMenu)
{
	if (!ButtonE.state())	return false;

	if (argChoice == 1)
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
	else if (argChoice == 2)
	{
		if (isMenuLabelMatch(argMenu, roDynamicTDSThreshold))
		{
			return RoTdsThresholdAdj(true);
		}
		else if (isMenuLabelMatch(argMenu, roFixedTDSThreshold))
		{
			return RoTdsThresholdAdj(false);
		}
		return false;
	}
}

bool haltMenuLoop(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
		autoFlushEnable(true);
		invokeST4(true);
		menu.changeMenu(&mainMenu05);
	}
	else if (ButtonR.state())
	{
		menu.changeMenu(&mainMenu05);
	}
	else
	{
		return false;
	}
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