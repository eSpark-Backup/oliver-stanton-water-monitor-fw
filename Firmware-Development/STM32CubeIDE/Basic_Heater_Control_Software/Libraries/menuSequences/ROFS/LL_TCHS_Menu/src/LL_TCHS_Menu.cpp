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
	frostParam.tempFormat=true;	/*Celsius by Default*/
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
    	DEBUG_RTT(LOG_INFO, "Heater OFF");	
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
void frostPreventToggleSetup()
{
    setButtonStates({ENTER_OK,UP,DOWN,BACK}, true);
}

void frostPreventOffSetup()
{
    Flags.setFlag(Settings, FROST_PREVENT ,false);
	setButtonStates({ BACK}, true);
}

bool frostPreventOffLoop(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonDel.state())
	{   
		Disp->display("(Frost Icon Crossed Out)");
    	DEBUG_RTT(LOG_INFO, "Heater OFF");	
		menu.changeMenu(argMenu);
        return true; 
	}
	// else if(ButtonE.state())
	// {
	// 	DEBUG_RTT(LOG_INFO, "***FROST PREVENTION ON***");	
	// 	Flags.setFlag(Settings , FROST_PREVENT , true);
	// 	menu.changeMenu(&frostPreventOn);
	// }
	// else if(ButtonB.state())
	// {
	// 	DEBUG_RTT(LOG_INFO, "***WARNING IGNORED***");	
	// 	menu.changeMenu(&heaterIntro);
	// }
    else{return false;}
	
    return true;
}

void frostPreventOnSetup()
{
    Flags.setFlag(Settings, FROST_PREVENT ,true);
    setButtonStates({ENTER_OK,UP,DOWN,BACK}, true);
}

void frostPreventTimerSetup()
{
	Disp->display((frostParam.frostPreventTime/1000)/60,"Current Timer (minutes):");
	setButtonStates({BACK}, true);
	
}

bool frostPreventTimerLoop(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonU.state())
	{
		if(frostParam.frostPreventTime < FROST_PREVENT_MAX_TIME)
		{
			frostParam.frostPreventTime=frostParam.frostPreventTime + FROST_PREVENT_TIME_STEP; /*increase by 1 min*/
			Disp->display((frostParam.frostPreventTime/1000)/60,"Current Timer (minutes):");
			DEBUG_RTT(LOG_INFO, "Timer Increased by 1 minute");
		}
	}
	else if (ButtonD.state())
	{   
		if(frostParam.frostPreventTime > DEFAULT_FROST_PREVENT_TIME)
		{
			frostParam.frostPreventTime= frostParam.frostPreventTime - FROST_PREVENT_TIME_STEP; /*increase by 1 min*/
			Disp->display((frostParam.frostPreventTime/1000)/60,"Current Timer (minutes):");
			DEBUG_RTT(LOG_INFO, "Timer Decreased by 1 minute");
		}
		DEBUG_RTT(LOG_INFO, "Timer Decreased by 1 minute");
	}
    else{return false;}

    return true;

}

void frostPreventTempThreshSetup()
{
	if(frostParam.tempFormat)
	{
		Disp->display(frostParam.frostPreventTempThresh,"Frost Prevent Temperature Threshold (Celsius): ");
	}
	else if(!frostParam.tempFormat)
	{
		Disp->display(frostParam.frostPreventTempThresh,"Frost Prevent Temperature Threshold (Fahrenheit): ");
	}
    setButtonStates({ ENTER_OK , BACK}, true);
}

bool frostPreventTempThreshLoop(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonU.state() && frostParam.tempFormat)
	{
		if(frostParam.frostPreventTempThresh < FROST_PREV_MAX_TEMP_THRESH_C)
		{
			frostParam.frostPreventTempThresh = frostParam.frostPreventTempThresh + FROST_PREV_TEMP_THRESH_STEP; /*increase by 1 min*/
			Disp->display(frostParam.frostPreventTempThresh,"Frost Prevent Temperature Threshold (Celsius): ");
			DEBUG_RTT(LOG_INFO, "Frost Prevent Temperature Threshold Increased by 1 Degree");
		}
	}
	else if (ButtonD.state() && frostParam.tempFormat)
	{   
		if(frostParam.frostPreventTempThresh > DEFAULT_PREV_TEMP_THRESH_C)
		{
			frostParam.frostPreventTempThresh = frostParam.frostPreventTempThresh - FROST_PREV_TEMP_THRESH_STEP; /*increase by 1 min*/
			Disp->display(frostParam.frostPreventTempThresh,"Frost Prevent Temperature Threshold (Celsius): ");
			DEBUG_RTT(LOG_INFO, "Frost Prevent Temperature Threshold Decreased by 1 Degree");
		}
	}
	else if (ButtonU.state() && !frostParam.tempFormat)
	{   
		if(frostParam.frostPreventTempThresh < FROST_PREV_MAX_TEMP_THRESH_F)
		{
			frostParam.frostPreventTempThresh = frostParam.frostPreventTempThresh + FROST_PREV_TEMP_THRESH_STEP; /*increase by 1 min*/
			Disp->display(frostParam.frostPreventTempThresh,"Frost Prevent Temperature Threshold (Fahrenheit): ");
			DEBUG_RTT(LOG_INFO, "Frost Prevent Temperature Threshold Decreased by 1 Degree");
		}
	}
	else if (ButtonD.state() && !frostParam.tempFormat)
	{   
		if(frostParam.frostPreventTempThresh > DEFAULT_PREV_TEMP_THRESH_F)
		{
			frostParam.frostPreventTempThresh = frostParam.frostPreventTempThresh - FROST_PREV_TEMP_THRESH_STEP; /*increase by 1 min*/
			Disp->display(frostParam.frostPreventTempThresh,"Frost Prevent Temperature Threshold (Fahrenheit): ");
			DEBUG_RTT(LOG_INFO, "Frost Prevent Temperature Threshold Decreased by 1 Degree");
		}
	}
    else{return false;}

    return true;	
}

void TempFormatSetup()
{
    setButtonStates({ENTER_OK,UP,DOWN,BACK}, true);
}

void celsiusSetup()
{
	frostParam.frostPreventTempThresh = DEFAULT_PREV_TEMP_THRESH_C;
	frostParam.tempFormat = true;
	Disp->display("Temperature Format Set to Celsius");
}

void fahrenheitSetup()
{	
	frostParam.frostPreventTempThresh = DEFAULT_PREV_TEMP_THRESH_F;
	frostParam.tempFormat = false;
	Disp->display("Temperature Format Set to Fahrenheit");
}


void frostPreventPumpCircuSpeedSetup()
{
    setButtonStates({  BACK}, true);
	Disp->display(frostParam.frostPreventPumpCircuSpeed,"Frost Prevent Pump Circultaion Speed :  ");
}

bool frostPreventPumpCircuSpeedLoop(uint32_t argChoice, MenuItem *argMenu)
{
	if (ButtonU.state())
	{
		if(frostParam.frostPreventPumpCircuSpeed < FROST_PREVENT_PUMP_SPEED_MAX)
		{
			frostParam.frostPreventPumpCircuSpeed = frostParam.frostPreventPumpCircuSpeed + FROST_PREVENT_PUMP_SPEED_STEP; /*increase by 1 min*/
			Disp->display(frostParam.frostPreventPumpCircuSpeed,"Frost Prevent Pump Circultaion Speed : ");
			DEBUG_RTT(LOG_INFO, "Frost Prevent Pump Circultaion Speed Increased by 5");
		}
	}
	else if (ButtonD.state())
	{   
		if(frostParam.frostPreventPumpCircuSpeed > DEFAULT_FROST_PREVENT_PUMP_SPEED)
		{
			frostParam.frostPreventPumpCircuSpeed = frostParam.frostPreventPumpCircuSpeed - FROST_PREVENT_PUMP_SPEED_STEP; /*increase by 1 min*/
			Disp->display(frostParam.frostPreventPumpCircuSpeed,"Frost Prevent Pump Circultaion Speed : ");
			DEBUG_RTT(LOG_INFO, "Frost Prevent Pump Circultaion Speed Decreased by 5");
		}
	}
	else{return false;}

    return true;

}











#endif
