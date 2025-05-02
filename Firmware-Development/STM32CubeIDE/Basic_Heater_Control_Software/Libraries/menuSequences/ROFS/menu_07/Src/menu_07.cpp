#include "menu_07.h"
#include "controlsequence.h"
#include "masterLog.h"
#if MENU_07 == 1

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

void introSetup_07(void)
{
	setButtonStates({NONE}, true);
}

bool introPageSelection_07(uint32_t argChoice, MenuItem* argMenu)
{
	if (ButtonR.state() && argMenu->Items[choiceSelect].prev == nullptr)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}

void mainMenuSetup_07(void)
{
	setButtonStates({BACK}, true);
}

bool mainMenuSelection_07(uint32_t argChoice, MenuItem *argMenu)
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
		menu.changeMenu(&fodOptnsOff07);
		ButtonE.reset();
	}
	return false;
}

void fodMenuSetup_07(void)
{
	setButtonStates({RODI}, true);
}

void fodOffSetup_07(void)
{
	roSaverSetup_07();
}

void roSaverSetup_07()
{
	setButtonStates({BACK}, true);
}

void backAndForthSetup_07(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

void navigationTrue(void)
{
	DEBUG_RTT(LOG_INFO, "navigationTrue:");
	setButtonStates({ENTER_OK, RODI}, false);
}

bool presCtrlOpt(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
	 	bool ispresSnsrEnable = argChoice == 2 ? true : false;
		Flags.setFlag(Settings, PRESSURE_SENSOR, ispresSnsrEnable);
		DEBUG_RTT(LOG_INFO, "Pressure Sensor Enable: %d", ispresSnsrEnable);
	}
	else
	{
		return false;
	}
	return true;
}

bool presSystemTypeOpt(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
	 	bool ispresSysTypeStatic = argChoice == 2 ? true : false;
	 	presSysType = (PRESSURE_SYSTEM_TYPE)ispresSysTypeStatic;
		DEBUG_RTT(LOG_INFO, "Pressure System Type Static: %d", presSysType);
	}
	else
	{
		return false;
	}
	return true;
}


#endif