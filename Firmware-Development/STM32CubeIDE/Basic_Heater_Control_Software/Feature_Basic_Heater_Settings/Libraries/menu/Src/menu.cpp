/*
 * menu.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan Ahmed
 */

#include "menu.h"
#include "controls.h"



//const char* arrowEnumToString(arrowEnum value);

//#define HEATER_STATE_ADDRESS 0x0801F800
//#define FROST_PREV_STATE_ADDRESS 0x0801F810
//#define FROST_PUMP_STATE_ADDRESS 0x0801F820


//-------------------------------Function Definitions---------------------

int *currentM;
short choiceSelect = 0;

void Menu::Display_Log(const char* log)
{
	Disp->display(log);
}

void Menu::displayMenu(const MenuItem* menu, int elements)
{
	Disp->itemsToDisplay(elements);
    for(uint8_t count = 0; count < elements; count++)
    {
    	Disp->display(menu[count].label);
    }
}

void Menu::navigateMenu(const char* optn, MenuItem* currentMenu)
{
//	uint8_t num_parent = currentMenu->num_parentmenuItems;
	uint8_t num_childs = currentMenu->num_childmenus;
	uint8_t num_items = currentMenu->num_items;
	uint8_t c = choiceSelect ;				//when referencing value of "c" in currentMenu we will use "c-1" since arrays start from 0
//	DEBUG_I("Inside navigateMenu()");
	//when UP button is pressed
	char buffer[100];
	sprintf(buffer,"num_items : %d\nc : %d", num_items, c);
	Disp->display(buffer);
	if(strcmp(optn, "up") == 0 )
	{
		if(num_items==1 && ((c == 0) || (strcmp(currentMenu->label,mainMenu[0].label)==0)))
					{
			currentMenu = (MenuItem*)Battery_Index;
						num_items = currentMenu->num_items;
						displayMenu(currentMenu,  num_items);
						currentM = (int*)currentMenu;
					}
		if(c==num_items)
		{
			c=1;
		}
		else
		{
			c += 1;

		}
		if(c>=1){
			Disp->display(c);
		}

		choiceSelect = c;
	}
		//when DOWN button is pressed
		if(strcmp(optn, "down") == 0 && num_items > 1)
		{
			if(c<=1)
			{
				c=num_items;
			}
			else if (c>1)
			{
				c -= 1;
			}
			Disp->display(c);
			choiceSelect = c;
		}
	//when BACK button is pressed
	if(strcmp(optn, "back") == 0 && currentMenu->prev != nullptr)
	{
		choiceSelect = 1;
		currentMenu = (MenuItem*)currentMenu->prev;
		displayMenu(currentMenu, currentMenu->num_items);
		currentM = (int*)currentMenu;	//}
	}
	//when ENTER button is pressed
	if(strcmp(optn, "enter") == 0)
	{
			if(num_items>1 && c == 0)
			{
				Disp->display("Choose one of the options.");
			}
			else if(num_items>1 && c != 0)
			{
//				displayMenu(currentMenu[c-1], num_items);
//				if(currentMenu[c-1].next == nullptr && currentMenu->DoWork!= nullptr){
//					currentMenu[c-1].DoWork();
//				}
//				num_childs = currentMenu[c-1].num_childmenus;
				if(num_childs == 0)
				{
					//Do nothing for this condition, program will crash if we dont do this
					//used for condition when you need to make a choice out of a range but number of items are 0
					if (currentMenu[0].DoWork!= nullptr)
					{
						if(currentMenu[0].DoWork(c) != true){}
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
					if (currentMenu[c-1].DoWork!= nullptr)
					{
						currentMenu[c-1].DoWork(c);			//function returning a false means that we dont need to return back to previous menu
//						else{
//							currentMenu = (MenuItem*)currentMenu->prev;
//							displayMenu(currentMenu, currentMenu->num_items);
//							currentM = (int*)currentMenu;		}
					}
					if(currentMenu[c-1].next != nullptr)
					{

						Disp->display("It should come here");
						Disp->display(currentMenu[c-1].label);

						if(strcmp(currentMenu[c-1].label,"On")==0) choiceSelect = 1;
						num_childs = currentMenu[c-1].num_childmenus;

						currentMenu = (MenuItem*)currentMenu[c-1].next;
						sprintf(buffer,"num_childmenus : %d\r\n", num_childs);
													Disp->display(buffer);
						displayMenu(currentMenu, num_childs);
						currentM = (int*)currentMenu;
						if(num_childs >2)choiceSelect++;
						//return;
					}
					else
					{
						Disp->display("It shouldn't come here");
						currentMenu = (MenuItem*)currentMenu->prev;
						displayMenu(currentMenu, currentMenu->num_items);
						currentM = (int*)currentMenu;
					}
				}
				if(currentMenu[c-1].next == nullptr)
					{
						Disp->display(currentMenu[c-1].label);
						choiceSelect = 0;
					}
			}
			else if (num_items==1 && currentMenu[0].DoWork!= nullptr)				//we need to enable this for conditions when a person doesnt have to make a choice in menu, i.e. c = undefined and above if condition is never satisfied
			{
				if(currentMenu[0].DoWork(0) != true){}
//					return;							}
				else if(currentMenu[c-1].next != nullptr)
				{
					Disp->display("It should come here");
					num_childs = currentMenu[c-1].num_childmenus;
					currentMenu = (MenuItem*)currentMenu[c-1].next;
					sprintf(buffer,"num_childmenus : %d\r\n", num_childs);
																		Disp->display(buffer);
					displayMenu(currentMenu, num_childs);
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
				Disp->display("It should come here first");
//				displayMenu(currentMenu, num_items);
				currentMenu = (MenuItem*)currentMenu->next;
				num_items = currentMenu->num_items;
				displayMenu(currentMenu, num_items);
				currentM = (int*)currentMenu;

			}
			Disp->display(1);
	}
}
