/*
 * menu.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan Ahmed
 */

#include "menu.h"
#include "controls.h"
#include "Battery.hpp"



//const char* arrowEnumToString(arrowEnum value);

//#define HEATER_STATE_ADDRESS 0x0801F800
//#define FROST_PREV_STATE_ADDRESS 0x0801F810
//#define FROST_PUMP_STATE_ADDRESS 0x0801F820


//-------------------------------Function Definitions---------------------

MenuItem *currentM;
short choiceSelect = 0;
short g_csIndex = 0;

void Menu::menuSetup()
{
	setButtonStates({RODI, UP}, true);
}

void Menu::Display_Log(const char* log, ...)
{
    const int buffer_size = 1024;
    char buffer[buffer_size];

    va_list args;
    va_start(args, log);
    vsnprintf(buffer, buffer_size, log, args);
    va_end(args);
    Disp->display(buffer);
}

void Menu::changeMenu(MenuItem* menu)
{
	currentM = menu;
	displayMenu(currentM, currentM->Items[0].num_items);
}

void Menu::displayMenu(const MenuItem *menu, int elements)
{
	Disp->itemsToDisplay(elements);
	for (uint8_t count = 0; count < elements; count++)
	{
		Disp->display(menu->Items[count].label);
	}
}

void Menu::navigateMenu(MenuItem* currentMenu, bool checkReturn)
{

	if (Battery.getMonitor().isKeepDeviceInShutdown)
	{
		return;
	}

		numChild_ = currentMenu->Items[g_csIndex].num_childmenus;
		numItems_ = currentMenu->Items[g_csIndex].num_items;
		choice_ = choiceSelect;

		if (ButtonU.isButtonEnabled() && ButtonU.state())
		{
			DEBUG_LOG(LOG_INFO, "******* UP Button *******\n ButtonU.period : %d\n", ButtonU.period());

			if (choice_ == numItems_)
			{
				choice_ = 1;
			}
			else
			{
				choice_ += 1;
			}
			if (choice_ >= 1)
			{
				Disp->display(choice_);
			}

			choiceSelect = choice_;
			ButtonU.reset();
		}

		// when DOWN button is pressed
		if (ButtonD.isButtonEnabled() && ButtonD.state())
		{
			DEBUG_LOG(LOG_INFO, "******* DOWN Button *******\n ButtonD.period : %d\n", ButtonD.period());

			if (numItems_ > 1)
			{
				if (choice_ <= 1)
				{
					choice_ = numItems_;
				}
				else if (choice_ > 1)
				{
					choice_ -= 1;
				}
				Disp->display(choice_);
				choiceSelect = choice_;
			}
			ButtonD.reset();
		}

		// when BACK button is pressed
		if (ButtonB.isButtonEnabled() && ButtonB.state() && currentMenu->Items[g_csIndex].prev != nullptr)
		{
			DEBUG_LOG(LOG_INFO, "******* BACK Button *******\n ButtonB.period : %d\n", ButtonB.period());
			Battery.getMonitor().isBackButtonPressed = true;

			g_csIndex = 0;
			choiceSelect = 0;
			currentMenu = (MenuItem *)currentMenu->Items[g_csIndex].prev;
			displayMenu(currentMenu, currentMenu->Items[g_csIndex].num_items);
			currentM = currentMenu; //}
			currentMenu->oneTimeSetup();

			ButtonB.reset();
		}

		// when RODI button is pressed
		if (ButtonR.isButtonEnabled() && ButtonR.state() && currentMenu->Items[g_csIndex].prev == nullptr)
		{
			DEBUG_LOG(LOG_INFO, "******* RODI Button *******\n ButtonR.period : %d\n", ButtonR.period());

			if (strcmp(currentMenu->Items[g_csIndex].label, intro.Items[g_csIndex].label) == 0)
			{
				jumpToNextItem_ = true;
			}

			ButtonR.reset();
		}

		// when ENTER button is pressed
		if (ButtonE.isButtonEnabled() && ButtonE.state())
		{
			DEBUG_LOG(LOG_INFO, "******* ENTER Button *******\n ButtonE.period : %d\n", ButtonE.period());

			// Display_Log("++++++++++++++++++++++++++++++++++++ \r\n num_items %d and choice_ %d and choiceSelect %d\r\n", numItems_, choice_, choiceSelect);

			if (numItems_ > 1 && choice_ == 0)
			{
				Disp->display("Choose one of the options.");
			}
			else if (numItems_ > 0)
			{

				if (currentMenu->Items[choice_ == 0 ? 0 : choice_ - 1].next == nullptr)
				{
					Display_Log("nullptr ================");
					g_csIndex = 0;
					choiceSelect = 0;
					currentMenu = (MenuItem *)currentMenu->Items[g_csIndex].prev;
					displayMenu(currentMenu, currentMenu->Items[g_csIndex].num_items);
					currentM = currentMenu;
				}
				else
				{
					Display_Log("jump ================");

					jumpToNextItem_ = true;
					choiceSelect = 0;
				}
			}
			else{
			Display_Log(" ********************* enter else *********************");
		}
	}

	if (jumpToNextItem_)
	{
		numChild_ = currentMenu->Items[choice_ == 0 ? 0 : choice_ - 1].num_childmenus;
		currentMenu = (MenuItem *)currentMenu->Items[choice_ == 0 ? 0 : choice_ - 1].next;
		displayMenu(currentMenu, numChild_);
		currentM = currentMenu;

		jumpToNextItem_ = false;
	}
	if(choiceSelect>0)
		g_csIndex = choiceSelect - 1;
	else if(!choiceSelect)
		g_csIndex = 0;

	resetButtonStates();
	}


void setButtonStates(std::initializer_list<SWITCHES> buttons, bool enableSelected)
{
	ButtonE.enableButton(!enableSelected);
	ButtonB.enableButton(!enableSelected);
	ButtonU.enableButton(!enableSelected);
	ButtonD.enableButton(!enableSelected);
	ButtonR.enableButton(!enableSelected);

	bool hasValidButton = false;

	for (SWITCHES button : buttons)
	{
		switch (button)
		{
		case ENTER_OK:
			ButtonE.enableButton(enableSelected);
			hasValidButton = true;
			break;
		case BACK:
			ButtonB.enableButton(enableSelected);
			hasValidButton = true;
			break;
		case UP:
			ButtonU.enableButton(enableSelected);
			hasValidButton = true;
			break;
		case DOWN:
			ButtonD.enableButton(enableSelected);
			hasValidButton = true;
			break;
		case RODI:
			ButtonR.enableButton(enableSelected);
			hasValidButton = true;
			break;
		case NONE:
			hasValidButton = true;
			break;
		default:
			break; // Ignore any invalid or unknown enums
		}
	}

	if (!hasValidButton)
	{
		ButtonE.enableButton(true);
		ButtonB.enableButton(true);
		ButtonU.enableButton(true);
		ButtonD.enableButton(true);
		ButtonR.enableButton(true);
	}
}

void getButtonStates()
{
	ButtonE.getState(ENTER_OK);
	ButtonB.getState(BACK);
	ButtonU.getState(UP);
	ButtonD.getState(DOWN);
	ButtonR.getState(RODI);
}

void resetButtonStates()
{
	ButtonE.reset();
	ButtonB.reset();
	ButtonU.reset();
	ButtonD.reset();
	ButtonR.reset();
}

void defaultMenuSetup(void)
{
	setButtonStates({RODI}, false);
}

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
