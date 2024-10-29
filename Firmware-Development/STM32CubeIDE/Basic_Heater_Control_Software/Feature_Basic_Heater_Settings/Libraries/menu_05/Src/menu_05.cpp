#include "menu_05.h"
#include "controls.h"


#define MAX_MAINS_TDS_THRESHOLD		600
#define DEFAULT_MAINS_TDS_THRESHOLD 500
#define MIN_MAINS_TDS_THRESHOLD 	50
#define DIFF_MAINS_TDS_THRESHOLD	20
#define MAX_DI_TDS_THRESHOLD 		15
#define DEFAULT_DI_TDS_THRESHOLD 	10
#define MIN_DI_TDS_THRESHOLD 		01
#define DIFF_DI_TDS_THRESHOLD 		01

#define MAX_TDS_TEMPERATURE		10
#define DEFAULT_TDS_TEMPERATURE	02
#define MIN_TDS_TEMPERATURE		00
#define DIFF_TDS_TEMPERATURE	01


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

	if (strcmp(argMenu->Items[0].label, mainsTDSThreshold.Items[0].label) == 0)
	{
		Flags.setFlag(Settings, MAINS_TDS_THRESHOLD, false);
		Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, false);
	}
	else if (strcmp(argMenu->Items[0].label, roTDSThreshold.Items[0].label) == 0)
	{
		Flags.setFlag(Settings, RO_TDS_THRESHOLD, false);
		Flags.setFlag(Settings, RO_TDS_TEMPERATURE, false);
	}
	else if (strcmp(argMenu->Items[0].label, diTDSThreshold.Items[0].label) == 0)
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
	if (argChoice == 1 && ButtonE.state()) TdsSettingsOff(argChoice, argMenu);
	else if (argChoice == 2 && ButtonE.state()) TdsDefaultThresholdValue(argChoice, argMenu);
	else if (argChoice == 3 && ButtonE.state())
	{
		if (strcmp(argMenu->Items[2].label, mainsTDSThreshold.Items[2].label) == 0)
		{
			return TDSThresholdAdjustedValue(true);
		}
		else if (strcmp(argMenu->Items[2].label, diTDSThreshold.Items[2].label) == 0)
		{
			return TDSThresholdAdjustedValue(false);
		}
		return false;
	}
}

bool TdsDefaultTemperatureValue(uint32_t argChoice, MenuItem *argMenu)
{
	DEBUG_RTT(LOG_INFO, "TDS Temperature value set to default 2*C");

	if (strcmp(argMenu->Items[1].label, mainsTDSTemperature.Items[1].label) == 0)
	{
		Flags.setFlag(Settings, MAINS_TDS_TEMPERATURE, true);
		setTDS.mainsTemperature = DEFAULT_TDS_TEMPERATURE;
		return true;
	}
	else if (strcmp(argMenu->Items[1].label, diTDSTemperature.Items[1].label) == 0)
	{
		Flags.setFlag(Settings, DI_TDS_TEMPERATURE, true);
		setTDS.diTemperature = DEFAULT_TDS_TEMPERATURE;
		return true;
	}
	return false;
}

bool TDSTemperatureAdjustedValue(bool mainsTdsMenu)
{
	static uint8_t threshVal = MIN_TDS_TEMPERATURE;

	if (ButtonD.state() && threshVal > MIN_TDS_TEMPERATURE)
	{
		threshVal -= DIFF_TDS_TEMPERATURE;
	}
	else if (ButtonU.state() && threshVal < MAX_TDS_TEMPERATURE)
	{
		threshVal += DIFF_TDS_TEMPERATURE;
	}

	Disp->display(threshVal);

	if (ButtonE.state())
	{
		Flags.setFlag(Settings, mainsTdsMenu ? MAINS_TDS_TEMPERATURE : DI_TDS_TEMPERATURE, true);
		if (mainsTdsMenu)
		{
			setTDS.mainsTemperature = threshVal;
		}
		else
		{
			setTDS.diTemperature = threshVal;
		}

		DEBUG_RTT(LOG_WARNING, "Temperature value set to %d.", threshVal);
		threshVal = MIN_TDS_TEMPERATURE;

		return true;
	}
	return false;
}

bool TDSTemperatureLoop(uint32_t argChoice, MenuItem *argMenu)
{
	if (argChoice == 1 && ButtonE.state())
		TdsSettingsOff(argChoice, argMenu);
	else if (argChoice == 2 && ButtonE.state())
		TdsDefaultTemperatureValue(argChoice, argMenu);
	else if (argChoice == 3 && ButtonE.state())
	{
		if (strcmp(argMenu->Items[2].label, mainsTDSTemperature.Items[2].label) == 0)
		{
			return TDSTemperatureAdjustedValue(true);
		}
		else if (strcmp(argMenu->Items[2].label, diTDSTemperature.Items[2].label) == 0)
		{
			return TDSTemperatureAdjustedValue(false);
		}
		return false;
	}
}

bool TdsShutdown(uint32_t argChoice, MenuItem *argMenu)
{
	DEBUG_RTT(LOG_INFO, "TDS shutdown settings");
	bool isShutdownEnable = (argChoice == 2 && ButtonE.state());

	if (strcmp(argMenu->Items[0].label, mainsTDSThreshold.Items[0].label) == 0)
	{
		Flags.setFlag(Settings, MAINS_TDS_SHUTDOWN, isShutdownEnable);
	}
	else if (strcmp(argMenu->Items[0].label, roTDSThreshold.Items[0].label) == 0)
	{
		Flags.setFlag(Settings, RO_TDS_SHUTDOWN, isShutdownEnable);
	}
	else if (strcmp(argMenu->Items[0].label, diTDSThreshold.Items[0].label) == 0)
	{
		Flags.setFlag(Settings, DI_TDS_SHUTDOWN, isShutdownEnable);
	}
	else
	{
		return false;
	}

	return true;
}