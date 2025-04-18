#include "menu_01.h"
#include "controlsequence.h"


void introSetup(void)
{
	setButtonStates({NONE}, true);
}

bool introPageSelection(uint32_t argChoice, MenuItem* argMenu)
{
	uint8_t numChild = argMenu->Items[choiceSelect].num_childmenus;
	uint8_t numItems = argMenu->Items[choiceSelect].num_items;
	uint8_t choice = choiceSelect;

	if (ButtonU.state() && argMenu->Items[choiceSelect].prev == nullptr)
	{
		currentM = &Battery_Index;
		menu.displayMenu(&Battery_Index, Battery_Index.Items[0].num_items);
		return true;
	}
	else if (ButtonR.state() && argMenu->Items[choiceSelect].prev == nullptr)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}

void mainMenuSetup(void)
{
	setButtonStates({BACK}, true);
}

bool mainMenuSelection(uint32_t argChoice, MenuItem *argMenu)
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

void fodMenuSetup(void)
{
	setButtonStates({RODI}, true);
}

void fodOffSetup(void)
{
	roSaverSetup();
}

void roSaverSetup()
{
	setButtonStates({BACK}, true);
}

void backAndForthSetup(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

void batIndexSetup(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

bool batIndexSelection(uint32_t argChoice, MenuItem *argMenu)
{
	uint8_t numChild = argMenu->Items[choiceSelect].num_childmenus;
	uint8_t numItems = argMenu->Items[choiceSelect].num_items;
	uint8_t choice = choiceSelect;
	return false;
}

void batConsumptionTableSetup(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

bool fodOFF(uint32_t argChoice, MenuItem *argMenu)
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
			mainMenuFunc();
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
				calCurrentTdsReading();
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

bool BatLowShutdownOff(uint32_t argChoice, MenuItem *argMenu)
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

bool BatPowerMonitorOff(uint32_t argChoice, MenuItem *argMenu)
{
	Disp->display("Battery Power Monitoring Disabled.");
	Flags.setFlag(Settings, BATTERY_POWER_MONITOR, false);
	return true;
}

bool fodON(uint32_t argChoice, MenuItem *argMenu)
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
bool roSaverThreshAdj(uint32_t argChoice, MenuItem *argMenu)
{
	static uint8_t threshVal = 0;

	if (ButtonD.state())
	{
		if (threshVal > 0)
			threshVal--;
		Disp->display(threshVal);
	}
	else if (ButtonU.state())
	{
		if (threshVal < 20)
			threshVal++;
		Disp->display(threshVal);
	}
	else if (ButtonE.state())
	{
		setTDS.roFixedThreshold = threshVal;
		threshVal = 0;
		DEBUG_RTT(LOG_WARNING, "RO_Saver threshold value set to %d.", setTDS.roFixedThreshold);
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
	setTDS.roFixedThreshold = 0;
	DEBUG_RTT(LOG_WARNING, "RO_Saver default threshold value set to %d.", setTDS.roFixedThreshold);
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

bool roSaverOff(uint32_t argChoice, MenuItem *argMenu)
{
	Flags.setFlag(Settings, RESIN_SAVER, false);
	DEBUG_RTT(LOG_WARNING, "RO_Saver setting Turned off.");
	return true;
}

void mainMenuFunc()
{
	currentM = &llTds_mainMenu08;
	menu.displayMenu(&llTds_mainMenu08, mainMenu.Items[0].num_items);
}
