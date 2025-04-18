#include "tdsMenu_08.h"
#include "controlsequence.h"
#include "masterLog.h"
#include "flagHandle.h"

autoFlushTimers flushTimer;

#if LLTDS_MENU_08 == 1

// Helper function to check if the menu label matches the target threshold
bool llTds_isMenuLabelMatch08(MenuItem *menu, const MenuItem &targetThreshold, uint8_t index = 0)
{
	return strcmp(menu->Items[index].label, targetThreshold.Items[index].label) == 0;
}

bool llTds_isMenuMatch08(MenuItem *menu1, const MenuItem &menu2, uint8_t index = 0)
{
	return menu1->Items[index].prev == menu2.Items[index].prev &&
		   menu1->Items[index].next == menu2.Items[index].next &&
		   menu1->Items[index].setup == menu2.Items[index].setup &&
		   menu1->Items[index].DoWork == menu2.Items[index].DoWork;
}

void llTds_introSetup_08(void)
{
	setButtonStates({NONE}, true);
}

bool llTds_introPageSelection_08(uint32_t argChoice, MenuItem* argMenu)
{
	if (ButtonR.state() && argMenu->Items[choiceSelect].prev == nullptr)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}

void llTds_mainMenuSetup_08(void)
{
	setButtonStates({BACK}, true);
}

bool llTds_mainMenuSelection_08(uint32_t argChoice, MenuItem *argMenu)
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
			&& !Flags.flagStatus(Status, RESIN_SAVER) && !Flags.flagStatus(Status, AUTO_FILL))
	{
		// menu.changeMenu(&fodOptnsOff08);
		ButtonE.reset();
	}
	return false;
}

void llTds_fodMenuSetup_08(void)
{
	setButtonStates({RODI}, false);
}

bool fodOFF_08(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
		if (Flags.flagStatus(Settings, FoD))
		{
			Flags.setFlag(Settings, FoD, false);
			DEBUG_RTT(LOG_WARNING, "Fill on Demand disabled");
		}
		if (ButtonE.period() > SHORT_PRESS && ButtonE.period() < LONG_LONG_PRESS)
		{
			Disp->display("System Purification stopped.");
			Flags.setFlag(Settings, MEM_SAVER, false);
			Flags.setFlag(Settings, MEM_SAVER_PAUSE, false);
			invokeST3(false);
			invokeST5(false);
		}
		else if (ButtonE.period() < SINGLE_PRESS)
		{
			Flags.setFlag(Settings, MEM_SAVER_PAUSE, false);
			if (Flags.flagStatus(Status, LEVEL_SENSOR) == true)
			{
				Flags.setFlag(Settings, MEM_SAVER, false);
				fillOpen();
			}
			else if (!Flags.flagStatus(Settings, MEM_SAVER))
			{
				invokeST3(true);
				invokeST5(true);
				// calCurrentTdsReading();
				perpStatus.diResinSaver = false;
			}
			else
			{
				Flags.setFlag(Settings, MEM_SAVER, false);
				Flags.setFlag(Settings, MEM_SAVER_PAUSE, true);
				Disp->display("System Purification paused.");
				invokeST3(false);
				invokeST5(false);

			}
		}
		return true;
	}
	return false;
}

void llTds_backAndForthSetup_08(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

void llTds_selectMustSetup_08(void)
{
	setButtonStates({ENTER_OK}, true);
}

void llTds_navigationTrue(void)
{
	DEBUG_RTT(LOG_INFO, "navigationTrue:");
	setButtonStates({ENTER_OK, RODI}, false);
}

bool llTdsWasteSelOpt(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
	 	bool isWasteRatioEnable = argChoice == 2 ? true : false;
		Flags.setFlag(Settings, WASTE_RATIO, isWasteRatioEnable);
		DEBUG_RTT(LOG_INFO, "Waste Ratio Settings Enable: %d",isWasteRatioEnable);
	}
	else
	{
		return false;
	}
	return true;
}

bool setDynamicWasteRatio(uint32_t argChoice, MenuItem *argMenu)
{
	uint8_t minWastePer = MIN_WASTE_PER;
	uint8_t maxWastePer = MAX_WASTE_PER;
	uint8_t diffWastePer = DIFF_WASTE_PER;

	static uint8_t wasteVal = minWastePer;

	if (ButtonD.state() && wasteVal > minWastePer)
	{
		wasteVal -= diffWastePer;
		Disp->display(wasteVal);
	}
	else if (ButtonU.state() && wasteVal < maxWastePer)
	{
		wasteVal += diffWastePer;
		Disp->display(wasteVal);
	}

	if (ButtonE.state())
	{
		Flags.setFlag(Settings, WASTE_RATIO, true);
		setWasteParam.wastePer = wasteVal;
		setWasteParam.purePer = MAX_WASTE_PER - wasteVal;
		DEBUG_RTT(LOG_WARNING, "Set %d Pure / %d Waste", setWasteParam.purePer, setWasteParam.wastePer);
		wasteVal = minWastePer;
		
		return true;
	}
	return false;
}

bool setWasteRatio(uint32_t argChoice, MenuItem *argMenu)
{
	int wasteValue = MIN_WASTE_PER;
	if (ButtonE.state() && argChoice == 2)
	{
		wasteValue = MIN_WASTE_PER;
	}
	else if (ButtonE.state() && argChoice == 3)
	{
		wasteValue = MIN_WASTE_PER + 10;
	}
	else
	{
		return false;
	}
	
	Flags.setFlag(Settings, WASTE_RATIO, true);
	setWasteParam.wastePer = wasteValue;
	setWasteParam.purePer = MAX_WASTE_PER - wasteValue;
	DEBUG_RTT(LOG_WARNING, "Set %d Pure / %d Waste", setWasteParam.purePer, setWasteParam.wastePer);
	return true;
}

bool autoFlushOptions08(uint32_t argChoice, MenuItem *argMenu)
{
	if (argChoice < 3 && ButtonE.state())
	{
		autoFlushEnable08(argChoice == 1 ? false : true);
	}
	else
		return false;
}

bool autoFlushEnable08(bool activateST4)
{
	Flags.setFlag(Settings, AUTO_FLUSH, activateST4);
	DEBUG_RTT(LOG_WARNING, "AUTO_FLUSH setting Turned %s.", activateST4 ? "On" : "Off");
	return true;
}

bool configureTdsWarning(uint32_t argChoice, MenuItem *argMenu)
{
	static uint8_t index = 0;
	if(llTds_isMenuMatch08(argMenu, tdsWarningList))
	{
		index = argChoice - 1;
		return true;
	}
	else if (llTds_isMenuMatch08(argMenu, SetWarnState) && ButtonE.state())
	{
		bool bitSet = false;

		if (argChoice == 1)			bitSet = false;
		else if (argChoice == 2)	bitSet = true;
		else						return false;

		if (bitSet)		isTdsWarningEnable.all_flags |= (1 << index); // Set the bit
		else			isTdsWarningEnable.all_flags &= ~(1 << index); // Clear the bit

		DEBUG_RTT(LOG_INFO, "Index : %d and bit: %d and warn %d", index, bitSet, isTdsWarningEnable.all_flags);
		index = 0;
		return true;
	}
	else
		return false;
}

bool selectTdsWarningItem(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state()) configureTdsWarning(argChoice, argMenu);
}

#endif
