#include "menu_05.h"
#include "controls.h"


void introSetup_05(void)
{
	setButtonStates({NONE}, true);
}

bool introPageSelection_05(uint32_t argChoice, MenuItem* argMenu)
{
	uint8_t numChild = argMenu->Items[choiceSelect].num_childmenus;
	uint8_t numItems = argMenu->Items[choiceSelect].num_items;
	uint8_t choice = choiceSelect;

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

void batIndexSetup_05(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

bool batIndexSelection_05(uint32_t argChoice, MenuItem *argMenu)
{
	uint8_t numChild = argMenu->Items[choiceSelect].num_childmenus;
	uint8_t numItems = argMenu->Items[choiceSelect].num_items;
	uint8_t choice = choiceSelect;
	return false;
}

void batConsumptionTableSetup_05(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}
