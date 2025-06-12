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
    setButtonStates({ENTER_OK,UP,DOWN,BACK}, true);
}

void heaterOf()
{
    setButtonStates({BACK , DEL}, true);
}


bool heaterOfLoop(uint32_t argChoice, MenuItem *argMenu)
{
    if (ButtonDel.state())
	{
		Disp->display("(Flame Icon Crossed Out)");
        return true;
	}
    return false;
    
}

void deliveryHomeSetup()
{
    setButtonStates({ENTER_OK , DEL}, true);
}


bool deliveryHomeLoop(uint32_t argChoice, MenuItem *argMenu)
{
    bool pumpState = true; /*will be changed later with actual pump state*/
    uint32_t lastTick = 0;
    uint32_t delayTime = 5000; // Delay time in milliseconds

    if (ButtonE.state())
	{
		DEBUG_RTT(LOG_INFO, "Main Heater Power ON");
        if(!pumpState)
        {
            Disp->display("(Flashing ! Icon) Heater ON Water not heating reconnect reel hose");
        }
        // else if(HAL_GetTick() - lastTick < delayTime)
        // {
        //     lastTick = HAL_GetTick();
        //     Disp->display("(Flashing Flame Icon) Preheating");
        // }
        else
        {
            Disp->display("(Flame Icon) Heater ON");
        }
	}
	else if (ButtonDel.state())
	{
        DEBUG_RTT(LOG_INFO, "Main Heater Power OFF");
	}
 
}

void mainMenuFuncHeater()
{
	currentM = &heaterIntro;
	menu.displayMenu(&heaterIntro, heaterIntro.Items[0].num_items);
}









#endif