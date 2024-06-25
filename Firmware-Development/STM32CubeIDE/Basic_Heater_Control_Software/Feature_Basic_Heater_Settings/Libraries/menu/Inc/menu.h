/*
 * menu.h
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan Ahmed
 */

#pragma once

#include "string.h"
#include "mainApp.hpp"
#include "_DEBUG_.h"



struct MenuItem {
    const char* label;          		//Name of the menu Element
    bool (*DoWork)(uint32_t);     		//Function invoked when element is selected
    const MenuItem* next;       		//SubMenu which it points to
    const MenuItem* prev;       		//Parent Menu
    uint8_t num_childmenus;         	//No. of child menus
//    uint8_t num_parentmenuItems;		//No. of items in parent menu
    uint8_t num_items;					//No. of items in current menu
};

//-----------declare MenuItem Structs defined in menuList.h down below ----------------
extern struct MenuItem mainMenu[], fodMenu[], fodOptns[], roSaver[], fodOptnsOff[], roSaverOptns[], roSaverThresh[], roSaverThreshAdjust[]\
,Battery_Index[], BatMonitorMenu[], BatLowMenuOptns[],BatLowMenuOFF[],BatPowerConsumptionOptns[],BatPowerConsumptionOptnsON[],BatPowerConsumptionOptnsOFF[]\
,DeviceStandByScreen[],DeviceShutDown[];
//-------------------------------------------------------------------------------------

class Menu{
public:
	Menu(){
		DEBUG_RTT(LOG_WARNING, "Menu obj constructor called");
	}
	~Menu(){
		DEBUG_RTT(LOG_WARNING, "Menu obj destructor called");
	}
	void Display_Log(const char* log);
	void displayMenu(const MenuItem* menu, int elements);
	void navigateMenu(const char* optn, MenuItem* currentMenu);

//	void createMenu(const char** menu);
//	void arrowDown();
//	void arrowUp();
//	void displayArrow();
//	void next();
//	void back();
//	void printFlame();
//	void pumpConnection();
//	void flameIconCallback(TIM_HandleTypeDef* htim);
};
