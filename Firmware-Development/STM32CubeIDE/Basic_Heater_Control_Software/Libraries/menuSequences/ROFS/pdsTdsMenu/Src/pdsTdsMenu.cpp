#include "pdsTdsMenu.h"
#include "controlsequence.h"
#include "masterLog.h"
#if LLPDS_TDS_MENU == 1

// Helper function to check if the menu label matches the target threshold
bool pdsTds_isMenuLabelMatch(MenuItem *menu, const MenuItem &targetThreshold, uint8_t index)
{
	return strcmp(menu->Items[index].label, targetThreshold.Items[index].label) == 0;
}

bool pdsTds_isMenuMatch(MenuItem *menu1, const MenuItem &menu2, uint8_t index = 0)
{
	return menu1->Items[index].prev == menu2.Items[index].prev &&
		   menu1->Items[index].next == menu2.Items[index].next &&
		   menu1->Items[index].setup == menu2.Items[index].setup &&
		   menu1->Items[index].DoWork == menu2.Items[index].DoWork;
}

void pdsTds_introSetup(void)
{
	setButtonStates({NONE}, true);
}

bool pdsTds_introPageSelection(uint32_t argChoice, MenuItem* argMenu)
{
	if (ButtonU.state() && argMenu->Items[choiceSelect].prev == nullptr)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}

void pdsTds_mainMenuSetup(void)
{
	setButtonStates({BACK}, true);
}

bool pdsTds_mainMenuSelection(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.period() > MINIMUM_PRESS && ButtonE.period() < SHORT_PRESS)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}

void pdsTds_backAndForthSetup(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

void pdsTds_navigationTrue(void)
{
	DEBUG_RTT(LOG_INFO, "navigationTrue:");
	setButtonStates({ENTER_OK, RODI}, false);
}

bool TdsThreshAdjVal(MenuItem *argMenu)
{
	uint8_t minThreshold = MIN_DEL_TDS_THRESHOLD;
	uint8_t maxThreshold = MAX_DEL_TDS_THRESHOLD;
	uint8_t diffThreshold = DIFF_DEL_TDS_THRESHOLD;

	static uint8_t threshVal = minThreshold;

	if (ButtonD.state() && threshVal > minThreshold)
	{
		threshVal -= diffThreshold;
		Disp->display(threshVal);
	}
	else if (ButtonU.state() && threshVal < maxThreshold)
	{
		threshVal += diffThreshold;
		Disp->display(threshVal);
	}

	if (ButtonE.state())
	{
		Flags.setFlag(Settings, DEL_TDS_THRESHOLD, true);
		setTDS.delThreshold = threshVal;
		DEBUG_RTT(LOG_WARNING, "Threshold value set to %d.", threshVal);
		threshVal = minThreshold;

		return true;
	}
	return false;
}

bool delTDSThreshold(uint32_t argChoice, MenuItem *argMenu)
{
	static bool isThresholdAdjustable = false;

	if (!isThresholdAdjustable && ButtonE.state())
	{
		switch (argChoice)
		{
			case 1:
			{
				Flags.setFlag(Settings, DEL_TDS_THRESHOLD, false);
			}
			case 2:
			{
				Flags.setFlag(Settings, DEL_TDS_THRESHOLD, true);
				setTDS.mainsThreshold = DEFAULT_DEL_TDS_THRESHOLD;
				DEBUG_RTT(LOG_INFO, "Delivery Output TDS threshold value set to %d ppm", setTDS.delThreshold);
			}
			case 3:
			{
				Disp->display("Select Threshold value from 0-50ppm");
				isThresholdAdjustable = true;
			}
			default:
				return false;

				pdsTds_backAndForthSetup();
				ButtonE.reset();
				break;
		}
	}
	else if (isThresholdAdjustable)
	{
		isThresholdAdjustable = !TdsThreshAdjVal(argMenu);
		return !isThresholdAdjustable; 
	}

	return false;
}

bool TDSTempAdjVal(MenuItem *argMenu)
{
	static uint8_t tempVal = MIN_TDS_TEMPERATURE;

	if (ButtonD.state() && tempVal > MIN_TDS_TEMPERATURE)
	{
		tempVal -= DIFF_TDS_TEMPERATURE;
		Disp->display(tempVal);
	}
	else if (ButtonU.state() && tempVal < MAX_TDS_TEMPERATURE)
	{
		tempVal += DIFF_TDS_TEMPERATURE;
		Disp->display(tempVal);
	}

	if (ButtonE.state())
	{
		Flags.setFlag(Settings, DI_TDS_TEMPERATURE, true);
		setTDS.diTemperature = tempVal;

		DEBUG_RTT(LOG_WARNING, "Temperature value set to %d.", tempVal);
		tempVal = MIN_TDS_TEMPERATURE;
		return true;
	}

	return false;
}

bool delTDSTemperature(uint32_t argChoice, MenuItem *argMenu)
{
	static bool isTemperatureAdjustable = false;

	if (!isTemperatureAdjustable && ButtonE.state())
	{
		switch (argChoice)
		{
			case 1:
				Flags.setFlag(Settings, DEL_TDS_TEMPERATURE, false);

			case 2:
			{
				Flags.setFlag(Settings, DEL_TDS_TEMPERATURE, true);
				setTDS.mainsTemperature = DEFAULT_TDS_TEMPERATURE;
				DEBUG_RTT(LOG_INFO, "Delivery Output temp value set to default %d*C", setTDS.delTemperature);
			}
			case 3:
			{
				Disp->display("Select Temperature value from 0-10*C");
				isTemperatureAdjustable = true;
				pdsTds_backAndForthSetup();
				ButtonE.reset();
				break;
			}
		}
	}
	else if (isTemperatureAdjustable)
	{
		isTemperatureAdjustable = !TDSTempAdjVal(argMenu);
		return !isTemperatureAdjustable;
	}

	return false;
}

bool delShutdown(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
	 	bool isEnable = argChoice == 2 ? true : false;

		if (pdsTds_isMenuMatch(argMenu, pdsTds_delTDSShutdown))
		{
			Flags.setFlag(Settings, DEL_TDS_SHUTDOWN, isEnable);
		}
		else if (pdsTds_isMenuMatch(argMenu, pdsTds_delTempShutdown))
		{
			Flags.setFlag(Settings, DEL_TEMP_SHUTDOWN, isEnable);
		}
		else if (pdsTds_isMenuMatch(argMenu, pdsTds_coldWeatherDynamics))
		{
			Flags.setFlag(Settings, DEL_COLD_DYNAMICS, isEnable);
		}
		DEBUG_RTT(LOG_INFO, "Delivery shutdown %d settings : %d", argChoice, isEnable);
	}
	else
	{
		return false;
	}
	return true;
}

bool delPump(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
		switch (argChoice)
		{
			case 1:
				Flags.setFlag(Settings, PUMP_1, false);
				break;

			case 2:
				Flags.setFlag(Settings, PUMP_1, true);
				break;

			case 3:
				DEBUG_RTT(LOG_WARNING, "Remote Control Disabled");
				break;
		}
		return true;
	}
	return false;
}

bool pdsTds_haltMenuLoop(uint32_t argChoice, MenuItem *argMenu)
{
	char warningTDS[30] = {0};
	if (ButtonE.state())
	{
		if(TdsWarn.Tds4_Thresh)
		{tempTdsWarning.Tds4_Thresh = false; strncpy(warningTDS,"Delivery Output TDS threshold", sizeof(warningTDS) - 1);}
		if(TdsWarn.Tds4_Temp)
		{tempTdsWarning.Tds4_Temp = false; strncpy(warningTDS,"Delivery Ouput Temperature", sizeof(warningTDS) - 1);}

		menu.changeMenu(&pdsTds_mainMenu);
		TdsWarn.all_flags = 0;
	}
	else
	{
		return false;
	}

	Log.logToMaster("User pressed enter key to override %s warning.", warningTDS);
	return true;
}

bool pdsTds_configureTdsWarning(uint32_t argChoice, MenuItem *argMenu)
{
	static uint8_t index = 0;
	if(pdsTds_isMenuMatch(argMenu, tdsWarningList))
	{
		index = argChoice - 1;
		return true;
	}
	else if (pdsTds_isMenuMatch(argMenu, SetWarnState) && ButtonE.state())
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

bool pdsTds_selectTdsWarningItem(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state()) return pdsTds_configureTdsWarning(argChoice, argMenu);
}

bool resetWarnLoop(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state() && argChoice == 2) 
	{
		tempTdsWarning.all_flags = 0xFFFF;
		if(Flags.flagStatus(Settings, PUMP_1))	invokeST7(true); // just to test logic
		return true;
	}
	return false;
}



#endif