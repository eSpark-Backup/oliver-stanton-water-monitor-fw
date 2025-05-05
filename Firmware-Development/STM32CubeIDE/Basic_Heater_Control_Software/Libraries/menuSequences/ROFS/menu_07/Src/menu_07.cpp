#include "menu_07.h"
#include "controlsequence.h"
#include "masterLog.h"
#if MENU_07 == 1

// Helper function to check if the menu label matches the target threshold
bool isMenuLabelMatch(MenuItem *menu, const MenuItem &targetThreshold, uint8_t index = 0)
{
	return strcmp(menu->Items[index].label, targetThreshold.Items[index].label) == 0;
}

bool isMenuMatch(MenuItem *menu1, const MenuItem &menu2, uint8_t index)
{
	return menu1->Items[index].prev == menu2.Items[index].prev &&
		   menu1->Items[index].next == menu2.Items[index].next &&
		   menu1->Items[index].setup == menu2.Items[index].setup &&
		   menu1->Items[index].DoWork == menu2.Items[index].DoWork;
}

void introSetup_07(void)
{
	setButtonStates({NONE}, true);
}

bool introPageSelection_07(uint32_t argChoice, MenuItem* argMenu)
{
	if (ButtonR.state() && argMenu->Items[choiceSelect].prev == nullptr)
	{
		menu.jumpToNextItem_ = true;
		return true;
	}
	return false;
}

void mainMenuSetup_07(void)
{
	setButtonStates({BACK}, true);
}

bool mainMenuSelection_07(uint32_t argChoice, MenuItem *argMenu)
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
		menu.changeMenu(&fodOptnsOff07);
		ButtonE.reset();
	}
	return false;
}

void fodMenuSetup_07(void)
{
	setButtonStates({RODI}, true);
}

void fodOffSetup_07(void)
{
	roSaverSetup_07();
}

void roSaverSetup_07()
{
	setButtonStates({BACK}, true);
}

void backAndForthSetup_07(void)
{
	setButtonStates({ENTER_OK, BACK}, true);
}

void navigationTrue(void)
{
	DEBUG_RTT(LOG_INFO, "navigationTrue:");
	setButtonStates({ENTER_OK, RODI}, false);
}

bool presCtrlOpt(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
	 	bool ispresSnsrEnable = argChoice == 2 ? true : false;
		Flags.setFlag(Settings, PRESSURE_SENSOR, ispresSnsrEnable);
		DEBUG_RTT(LOG_INFO, "Pressure Sensor Enable: %d", ispresSnsrEnable);
	}
	else
	{
		return false;
	}
	return true;
}

bool presSystemTypeOpt(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
	 	bool ispresSysTypeStatic = argChoice == 2 ? true : false;
	 	presSysType = (PRESSURE_SYSTEM_TYPE)ispresSysTypeStatic;
		DEBUG_RTT(LOG_INFO, "Pressure System Type Static: %d", presSysType);
	}
	else
	{
		return false;
	}
	return true;
}

bool boostedFillingOpt(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
	 	isboostedFillingEnable = argChoice == 2 ? true : false;
		DEBUG_RTT(LOG_INFO, "Pressure Boosted Filling Production Enable: %d", isboostedFillingEnable);
	}
	else
	{
		return false;
	}
	return true;
}

bool boostedFlushOpt(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state())
	{
	 	isboostedFlushEnable = argChoice == 2 ? true : false;
		DEBUG_RTT(LOG_INFO, "Pressure Boosted autoflush  Enable: %d", isboostedFlushEnable);
	}
	else
	{
		return false;
	}
	return true;
}

bool presAlertOpt(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonE.state() && argChoice > 0 && argChoice < 4)
	{
	 	presAlertType = (PRESSURE_ALERT_TYPE)argChoice;
		DEBUG_RTT(LOG_INFO, "Pressure Alert Type: %d", presAlertType);
	}
	else
	{
		return false;
	}
	return true;
}

bool calibrationAlgo()
{
	if (ButtonE.period() > MINIMUM_PRESS && ButtonE.period() < SHORT_PRESS)
	{
		// ST6 and ST7 remains 
		Disp->display("Pressure Sensor Calibrating - Checking Standby Pressure");
		invokeST1(false); invokeST2(false);	invokeST3(false); invokeST4(false); invokeST5(false);
		dt3_ps.standby = readSens(PRESSURE_SENSOR);
		
		Disp->display("Pressure Sensor Calibrating - Checking Filling - Non Boosted Autoflush Pressure");
		invokeST1(false); invokeST2(true);	invokeST3(true); invokeST4(true); invokeST5(false);
		dt3_ps.autoflushNoBoost = readSens(PRESSURE_SENSOR);
		
		Disp->display("Pressure Sensor Calibrating - Checking Filling - Boosted Autoflush Pressure");
		invokeST1(false); invokeST2(false);	invokeST3(true); invokeST4(true); invokeST5(false);
		dt3_ps.autoflushBoost = readSens(PRESSURE_SENSOR);
		
		Disp->display("Pressure Sensor Calibrating - Checking Non-Boosted Filling Pressure");
		invokeST1(false); invokeST2(false);	invokeST3(true); invokeST4(false); invokeST5(false);
		dt3_ps.fillingNoBoost = readSens(PRESSURE_SENSOR);
		
		Disp->display("Pressure Sensor Calibrating - Checking Boosted Filling Pressure");
		invokeST1(false); invokeST2(true);	invokeST3(true); invokeST4(false); invokeST5(false);
		dt3_ps.fillingBoost = readSens(PRESSURE_SENSOR);
		
		Disp->display("Pressure Sensor Calibration Complete\n");
		return true;
	}
	return false;
}

bool recalibratePressure(bool *recalibrate)
{
	
	static bool firstUse = true;
	static bool cycleCommenced = false;
	static long lastCycle = 0u;
	static MenuItem * previousMenu = nullptr;
	bool localCalibrate = *recalibrate;
	
	// if (HAL_GetTick() - lastCycle < 1000)
	// 	return false;
	
	if (!Flags.flagStatus(Status, LEVEL_SENSOR) && firstUse && !cycleCommenced) // closed
	{
		if(previousMenu != currentM) //callibrate mode
		{
			Disp->display("Pressure Sensor Calibration Required, press ENTER to start\n");
			cycleCommenced = true;
			previousMenu = currentM;
		}
	}
	else if(cycleCommenced)
	{
		if (ButtonE.period() > MINIMUM_PRESS && ButtonE.period() < SHORT_PRESS)
		{
			Disp->display("!!! ENSURE WATER INLET, WASTE OUTLET, and ELECTRIC MAINS HOOKUP IS CONNECTED AND TURNED ON !!!\n");
			*recalibrate = true;
			firstUse = false;
			cycleCommenced = false;
			ButtonE.reset();
		}
		else if (ButtonB.period() > MINIMUM_PRESS && ButtonB.period() < SHORT_PRESS)
		{
			*recalibrate = false;
			firstUse = true;
		}
	}
	// menu.Display_Log("*recalibrate: %d\n",*recalibrate);
	if(!cycleCommenced && !firstUse)
	{
		Disp->display("Press ENTER to start calibration\n");
		*recalibrate = calibrationAlgo();
	}
	else
	{
		return false;
	}
	return true;
}

bool pressureSnsrcalibration(uint32_t argChoice, MenuItem *argMenu)
{
	static bool calibratedOnce = false;
	if (ButtonE.state())
	{
		if (!calibratedOnce)
		{
			backAndForthSetup_07();
			recalibratePressure(&calibratedOnce);
			if(!calibratedOnce) ButtonE.reset();
		}
		else if (argChoice == 1)
		{
			menu.Display_Log("dts_ps_standby: %d\n"
							 "dts_ps_autoflush_boost: %d\n"
							 "dts_ps_autoflush_noboost: %d\n"
							 "dts_ps_filling_noboost: %d\n"
							 "dts_ps_filling_boost: %d\n",
							 dt3_ps.standby,
							 dt3_ps.autoflushBoost,
							 dt3_ps.autoflushNoBoost,
							 dt3_ps.fillingNoBoost,
							 dt3_ps.fillingBoost);
		}
		else if (argChoice == 2)
		{
			Disp->display("Are you sure you want to re-calibrate the pressure sensor?\n");
			calibratedOnce = false;
			ButtonE.reset();
		}
	}
	else
	{
		return false;
	}
	return true;
}

#endif