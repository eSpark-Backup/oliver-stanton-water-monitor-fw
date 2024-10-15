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

int *currentM;
short choiceSelect = 0;


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

void Menu::displayMenu(const MenuItem* menu, int elements)
{
	Disp->itemsToDisplay(elements);
    for(uint8_t count = 0; count < elements; count++)
    {
    	Disp->display(menu[count].label);
    }
}

void Menu::navigateMenu(const char* optn, MenuItem* currentMenu, bool checkReturn)
{
//	uint8_t num_parent = currentMenu->num_parentmenuItems;
	if(Battery.getMonitor().isKeepDeviceInShutdown == true)
	{
		return;
	}
	numChild_ = currentMenu->num_childmenus;
	numItems_ = currentMenu->num_items;
	choice_ = choiceSelect ;				//when referencing value of "choice_" in currentMenu we will use "choice_-1" since arrays start from 0

	if ((strcmp(optn, "up") == 0) && (strcmp(currentMenu->label, intro->label) != 0))
	{
		if(choice_==numItems_)
		{
			choice_=1;
		}
		else
		{
			choice_ += 1;

		}
		if(choice_>=1){
			Disp->display(choice_);
		}

		choiceSelect = choice_;
	}

	//when DOWN button is pressed
	if(strcmp(optn, "down") == 0)
	{
		if (numItems_ > 1)
		{
			if(choice_<=1)
			{
				choice_=numItems_;
			}
			else if (choice_>1)
			{
				choice_ -= 1;
			}
			Disp->display(choice_);
			choiceSelect = choice_;
		}
		

	}

	//when BACK button is pressed
	if(strcmp(optn, "back") == 0 && currentMenu->prev != nullptr)
	{
		choiceSelect = 1;
		currentMenu = (MenuItem*)currentMenu->prev;
		displayMenu(currentMenu, currentMenu->num_items);
		currentM = (int*)currentMenu;	//}
		currentMenu->setup();
	}
	
	//when RODI button is pressed
	if(strcmp(optn, "rodi") == 0 && currentMenu->prev == nullptr)
	{
		if(strcmp(currentMenu->label,intro->label)==0)
		{
			jumpToNextItem_ = true;
		}
	}

	//when ENTER button is pressed
	if((strcmp(optn, "enter") == 0))
	{
			if(numItems_>1 && choice_ == 0)
			{
				Disp->display("Choose one of the options.");
			}
			else if(numItems_>1 && choice_ != 0)
			{
//				displayMenu(currentMenu[choice_-1], numItems_);
//				if(currentMenu[choice_-1].next == nullptr && currentMenu->DoWork!= nullptr){
//					currentMenu[choice_-1].DoWork();
//				}
//				numChild_ = currentMenu[choice_-1].num_childmenus;
				if(numChild_ == 0)
				{
					//Do nothing for this condition, program will crash if we dont do this
					//used for condition when you need to make a choice out of a range but number of items are 0
					if (currentMenu[0].DoWork!= nullptr)
					{
						if(currentMenu[0].DoWork(choice_, currentMenu) != true){}
						else
						{
							currentMenu = (MenuItem*)currentMenu->prev;
							displayMenu(currentMenu, currentMenu->num_items);
							currentM = (int*)currentMenu;
						}
					}
				}
				else
				{
					if (currentMenu[choice_-1].DoWork!= nullptr)
					{
						currentMenu[choice_-1].DoWork(choice_, currentMenu);			//function returning a false means that we dont need to return back to previous menu
//						else{
//							currentMenu = (MenuItem*)currentMenu->prev;
//							displayMenu(currentMenu, currentMenu->num_items);
//							currentM = (int*)currentMenu;		}
					}
					if(currentMenu[choice_-1].next != nullptr)
					{

//						Disp->display("It should come here");
//						Disp->display(currentMenu[choice_-1].label);

						if(strcmp(currentMenu[choice_-1].label,"On")==0) choiceSelect = 1;
						numChild_ = currentMenu[choice_-1].num_childmenus;
//						Disp->display(numChild_);
						currentMenu = (MenuItem*)currentMenu[choice_-1].next;
//						Disp->display("It should come here too");
//						Disp->display(currentMenu->label);
						if(strcmp(currentMenu->label,BatLowMenuOFF[0].label)==0)
						{
							Disp->display("Switching this off will invalidate the warranty on your battery.");
						}
//						sprintf(buffer,"num_childmenus : %d\r\n Next Menu label %s", numChild_, currentMenu->label);
//						Disp->display(buffer);
						displayMenu(currentMenu, numChild_);
						currentM = (int*)currentMenu;
						if(numChild_==1)choiceSelect=1;
						if(numChild_ >2)choiceSelect++;
						//return;
					}
					else if ((strcmp(currentMenu->label, mainMenu->label) != 0))
					{
//						Disp->display("It shouldn't come here");
						currentMenu = (MenuItem*)currentMenu->prev;
						displayMenu(currentMenu, currentMenu->num_items);
						currentM = (int*)currentMenu;
					}
				}
				if(currentMenu[choice_-1].next == nullptr)
					{
						Disp->display(currentMenu[choice_-1].label);
						choiceSelect = 0;
					}
			}
			else if (numItems_ == 1 && currentMenu[0].DoWork != nullptr 
			&& (strcmp(currentMenu->label, Battery_Index->label) != 0) || (strcmp(currentMenu->label, mainMenu->label) != 0)) // we need to enable this for conditions when a person doesnt have to make a choice in menu, i.e. choice_ = undefined and above if condition is never satisfied
			{
				if(currentMenu[0].DoWork(0, currentMenu) != true){}
//					return;							}
				else if(currentMenu[choice_-1].next != nullptr)
				{
//					Disp->display("It should come here");
					numChild_ = currentMenu[choice_-1].num_childmenus;
					currentMenu = (MenuItem*)currentMenu[choice_-1].next;
//					sprintf(buffer,"num_childmenus : %d\r\n Next Menu label %s", numChild_, currentMenu->label);
//																		Disp->display(buffer);
					displayMenu(currentMenu, numChild_);
					currentM = (int*)currentMenu;
				}
				else
				{
					currentMenu = (MenuItem*)currentMenu->prev;
					displayMenu(currentMenu, currentMenu->num_items);
					currentM = (int*)currentMenu;
				}
			}

			else
			{
				// Disp->display(1);
			}
	}

	if(jumpToNextItem_ == true)
	{
		currentMenu = (MenuItem*)currentMenu->next;
		numItems_ = currentMenu->num_items;
		displayMenu(currentMenu, numItems_ );
		currentM = (int*)currentMenu;

		jumpToNextItem_ = false;
	}
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

void introSetup(void)
{
	setButtonStates({NONE}, false);
}

bool introPageSelection(uint32_t argChoice, MenuItem* argMenu)
{
	uint8_t numChild = argMenu->num_childmenus;
	uint8_t numItems = argMenu->num_items;
	uint8_t choice = choiceSelect;

	if(ButtonU.state() && (numItems==1 && ((choice == 0) || (strcmp(argMenu->label,intro->label)==0))))
	{
		argMenu = (MenuItem *)Battery_Index;
		numItems = argMenu->num_items;
		menu.displayMenu(argMenu,  numItems);
		currentM = (int*)argMenu;
		return true;
	}
	return false;
}


void mainMenuSetup(void)
{
	setButtonStates({RODI}, false);
}

bool mainMenuSelection(uint32_t argChoice, MenuItem *argMenu)
{
	uint8_t numItems = argMenu->num_items;

	if ((ButtonE.period() > LONG_PRESS || ButtonD.period() > LONG_PRESS) 
		&& (ButtonE.period() < LONG_LONG_PRESS || ButtonD.period() < LONG_LONG_PRESS))
	{
		argMenu = (MenuItem *)argMenu->next;
		numItems = argMenu->num_items;
		menu.displayMenu(argMenu, numItems);
		currentM = (int *)argMenu;
		return true;
	}
	return false;
}


void batIndexSetup(void)
{
	DEBUG_LOG(LOG_ERROR, "batIndexSetup");
	setButtonStates({RODI}, false);
}

bool batIndexSelection(uint32_t argChoice, MenuItem *argMenu)
{
	uint8_t numChild = argMenu->num_childmenus;
	uint8_t numItems = argMenu->num_items;
	uint8_t choice = choiceSelect;

	if (ButtonE.state())
	{
		argMenu = (MenuItem *)argMenu->next;
		numItems = argMenu->num_items;
		menu.displayMenu(argMenu, numItems);
		currentM = (int *)argMenu;
		return true;
	}
	return false;
}