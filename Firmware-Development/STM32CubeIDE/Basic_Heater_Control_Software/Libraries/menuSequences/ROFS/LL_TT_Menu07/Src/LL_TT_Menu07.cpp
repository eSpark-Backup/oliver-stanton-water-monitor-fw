#include "LL_TT_Menu07.h"
#include "controls_LL_TT_07.h"
#include "masterLog.h"
#if LL_TT_MENU_07 == 1

// Helper function to check if the menu label matches the target threshold
bool remote_TT_isMenuLabelMatch(MenuItem *menu, const MenuItem &targetThreshold, uint8_t index)
{
	return strcmp(menu->Items[index].label, targetThreshold.Items[index].label) == 0;
}

bool remote_TT_isMenuMatch(MenuItem *menu1, const MenuItem &menu2, uint8_t index = 0)
{
	return menu1->Items[index].prev == menu2.Items[index].prev &&
		   menu1->Items[index].next == menu2.Items[index].next &&
		   menu1->Items[index].setup == menu2.Items[index].setup &&
		   menu1->Items[index].DoWork == menu2.Items[index].DoWork;
}

void remote_TT_introSetup07(void)
{
	setButtonStates({NONE}, true);
}

bool remote_TT_introPageSelection07(uint32_t argChoice, MenuItem* argMenu)
{
	if (ButtonE.period() >= LONG_PRESS)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}

void remote_TT_mainMenuSetup07(void)
{
	setButtonStates({BACK}, true);
}

bool remote_TT_mainMenuSelection07(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.period() >= LONG_PRESS)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}


#endif