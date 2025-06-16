/*
 * LL_TCHS_Menu.cpp
 *
 *  Created on: June 12, 2025
 *      Author: Abdul Ahad
 */


#include "LL_TCHS_Menu.h"
#include "controlsequence.h"


#if LL_TCHS_MENU == 1



void heaterIntroSetup()
{
    setButtonStates({ENTER_OK}, true);
}

void heaterSettOptns()
{
	setButtonStates({ENTER_OK,UP,DOWN,BACK}, true);
}



void heaterTogg()
{
	lastTick=0;
	menuSwitched=true;
    setButtonStates({ENTER_OK,UP,DOWN,BACK}, true);
}


void heaterOf()
{
    setButtonStates({BACK}, true);
}


bool heaterOfLoop(uint32_t argChoice, MenuItem *argMenu)
{
    if (ButtonDel.state())
	{
		Disp->display("(Flame Icon Crossed Out)");
		menu.changeMenu(argMenu);
        return true;        
	}
    return false;
    
}

void deliveryHomeSetup()
{
	menuSwitched=false;
    setButtonStates({ BACK}, true);
}


bool deliveryHomeLoop(uint32_t argChoice, MenuItem *argMenu)
{           

    if (ButtonE.state())
	{
        // DEBUG_RTT(LOG_INFO, "Main Heater Power OFF");
	    Flags.setFlag(Settings, MAIN_HEATER, false);
	}
	else if (ButtonDel.state())
	{   
		// DEBUG_RTT(LOG_INFO, "Main Heater Power ON");
	    Flags.setFlag(Settings, MAIN_HEATER, true);
		menu.changeMenu(argMenu);
	}
    else{return false;}

    return true;
    
}

void mainMenuFuncHeater()
{
	currentM = &heaterIntro;
	menu.displayMenu(&heaterIntro, heaterIntro.Items[0].num_items);
}









#endif