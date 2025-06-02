#include "LL_TT_Menu07.h"
#include "controls_LL_TT_07.h"
#include "masterLog.h"
#include "eventHandle.hpp"
#include "LED.h"

#if LL_TT_MENU_07 == 1

// Helper function to check if the menu label matches the target threshold
bool remote_TT_isMenuLabelMatch(MenuItem *menu, const MenuItem &targetThreshold, uint8_t index)
{
	return strcmp(menu->Items[index].label, targetThreshold.Items[index].label) == 0;
}

bool remote_TT_isMenuMatch(MenuItem *menu1, const MenuItem &menu2, uint8_t index)
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
		ButtonE.reset();
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
	static int pumpFlowTime = 0;
	static int lastPumpFlowTime = 0;
	static long prevSequence = 0u;
	static long timePassed = 0;
	static bool previousSequence = false;
	DEBUG_RTT(LOG_ERROR, "R: %d and U: %d", ButtonR.period(),ButtonU.period());
	
	if (ButtonDel.period() >= LONG_LONG_PRESS)
	{
		((RgbDriver*)WLED1)->setRgbColor(BLACK);
		WLED1->setOutputPattern(1, 0, 100);
		ButtonDel.reset();
	}
	else if (ButtonDel.period() >= MINIMUM_PRESS && ButtonDel.period() < SHORT_PRESS && pumpFlowTime == 1000)
	{
		pumpFlowTime = 0;
		((RgbDriver*)WLED1)->setRgbColor(BLACK);
		WLED1->setOutputPattern(1, 0, 100);
		Disp->display("Delivery Pump Pause by Remote Fob");
		ButtonDel.reset();
	}
	else if (ButtonE.period() > MINIMUM_PRESS && ButtonE.period() < SHORT_PRESS && pumpFlowTime == 0)
	{
		pumpFlowTime = 1000;
		((RgbDriver*)WLED1)->setRgbColor(MAGENTA);
		WLED1->setOutputPattern(-1, pumpFlowTime, pumpFlowTime);
		Disp->display("Delivery Pump Activated by Remote Fob");
		ButtonE.reset();
	}
	else if (ButtonR.period() > MINIMUM_PRESS && ButtonR.period() < SHORT_PRESS && pumpFlowTime == 1000)
	{
		pumpFlowTime = 500;
		((RgbDriver*)WLED1)->setRgbColor(MAGENTA);
		WLED1->setOutputPattern(-1, pumpFlowTime, pumpFlowTime);
		Disp->display("Delivery Pump Flow Speed Increased by Remote Fob");
		ButtonR.reset();
	}
	else if (ButtonU.period() > MINIMUM_PRESS && ButtonU.period() < SHORT_PRESS && (pumpFlowTime == 500 || pumpFlowTime == 1000))
	{
		pumpFlowTime = 2000;
		((RgbDriver*)WLED1)->setRgbColor(MAGENTA);
		WLED1->setOutputPattern(-1, pumpFlowTime, pumpFlowTime);
		Disp->display("Delivery Pump Flow Speed Decreased by Remote Fob");
		ButtonR.reset();
	}
	else if (ButtonB.period() > MINIMUM_PRESS && ButtonB.period() < SHORT_PRESS)
	{
		if(pumpFlowTime != 900)
		{
			lastPumpFlowTime = pumpFlowTime;
			pumpFlowTime = 900;
			((RgbDriver*)WLED1)->setRgbColor(ORANGE);
			WLED1->setOutputPattern(-1, pumpFlowTime, pumpFlowTime);
			Disp->display("Delivery Heater Activated by Remote");
		}
		else
		{
			pumpFlowTime = lastPumpFlowTime;
			lastPumpFlowTime = 0;
			previousSequence = true;
			Disp->display("Delivery Heater Dectivated by Remote");
		}
		ButtonB.reset();
	}
	else if ((ButtonR.period() >= LONG_PRESS && ButtonR.period() < LONG_LONG_PRESS) &&
			(ButtonU.period() >= LONG_PRESS && ButtonU.period() < LONG_LONG_PRESS))
	{
		((RgbDriver*)WLED1)->setRgbColor(BLUE);
		WLED1->setOutputPattern(1, 100, 5000);
		timePassed = 5000;
		Disp->display("To Display Tank Level Output on Receiver Controller (Not Connected Yet)");
		ButtonR.reset();
		ButtonU.reset();
	}
	else if ((ButtonR.period() >= LONG_PRESS && ButtonR.period() < LONG_LONG_PRESS) &&
			(ButtonE.period() >= LONG_PRESS && ButtonE.period() < LONG_LONG_PRESS))
	{
		((RgbDriver*)WLED1)->setRgbColor(GREEN);
		WLED1->setOutputPattern(1, 100, 5000);
		prevSequence = HAL_GetTick();
		timePassed = 5000;
		Disp->display("To Display Battery Level Output on Receiver Controller (Not Addressable LEDs)");
		ButtonR.reset();
		ButtonE.reset();
	}
	else if (ButtonR.period() >= LONG_LONG_PRESS && ButtonE.period() >= LONG_LONG_PRESS && ButtonU.period() >= LONG_LONG_PRESS)
	{
		((RgbDriver*)WLED1)->setRgbColor(GREEN);
		WLED1->setOutputPattern(-1, 500, 500);
		prevSequence = HAL_GetTick();
		timePassed = 5000;
		Disp->display("Do you wish to pair Remote Fob to this controller? (Not Reciever End Yet)");
		ButtonR.reset();
	}
	else
		return false;

	if(previousSequence == true || (!timePassed && (HAL_GetTick() - prevSequence > timePassed)))
	{
		DEBUG_RTT(LOG_INFO, "Previous Sequence");
		((RgbDriver*)WLED1)->setRgbColor(pumpFlowTime > 0 ? MAGENTA : BLACK);
		WLED1->setOutputPattern(pumpFlowTime>0 ? -1: 1, pumpFlowTime, pumpFlowTime);
		timePassed = 0;
		previousSequence = false;
	}
	return true;
}


#endif