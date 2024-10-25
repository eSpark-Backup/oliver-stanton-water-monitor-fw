#include "menu_01.h"
#include "controls.h"


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
