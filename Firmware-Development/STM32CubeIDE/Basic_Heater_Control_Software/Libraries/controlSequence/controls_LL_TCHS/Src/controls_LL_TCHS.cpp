/*
 * controls_LL_TCHS.cpp
 *
 *  Created on: June 12, 2025
 *      Author: Abdul Ahad
 */

#include "controls_LL_TCHS.h"

#if LL_TCHS_MENU == 1
#include "TDS.hpp"
#include "LL_TCHS_Menu.h"
#include "masterLog.h"
#include "Battery.hpp"


#define SECONDS_TO_MS(x) 				((x) * 1000) 						//converts seconds to milliseconds
#define MIN_TO_MS(x) 					((x) * SECONDS_TO_MS(60))			//converts minutes to milliseconds
#define FOD_AUTORUN_DELAY_PERIOD		SECONDS_TO_MS(60)					//60secs
#define MEM_SAVER_PAUSE_TIMEOUT			MIN_TO_MS(15)						//15mins or 900secs
#define AUTO_FLUSH_TIMEOUT				MIN_TO_MS(20)						//20 mins or 1200secs
#define AUTO_FLUSH_FINISH_5M			MIN_TO_MS(05)

TdsValues setTDS{
	DEFAULT_MAINS_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE,
	DEFAULT_DYNAMIC_TDS_THRESHOLD,
	DEFAULT_FIXED_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE,
	DEFAULT_DI_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE,
	DEFAULT_DEL_TDS_THRESHOLD,
	DEFAULT_TDS_TEMPERATURE
};


//***************************Buzzer*******************************
EventHandle* Buzzzer = new BuzzerDriver(BUZZER_GPIO_Port, BUZZER_Pin, IndBUZZER);
EventHandle* WLED1 = new RgbDriver(R_CNTRL_GPIO_Port, R_CNTRL_Pin, G_CNTRL_Pin, B_CNTRL_Pin, IndRGB);

//***************************TDS Sensor*******************************
GPIOConfig tds1PinConfig  = {TDS1_T2_GPIO_Port, TDS1_T1_GPIO_Port, ADC_CHANNEL_10, TDS1_T2_Pin, TDS1_T1_Pin, ADC_CHANNEL_11};
GPIOConfig tds2PinConfig  = {TDS2_T2_GPIO_Port, TDS2_T1_GPIO_Port, ADC_CHANNEL_14, TDS2_T2_Pin, TDS2_T1_Pin, ADC_CHANNEL_15};
GPIOConfig tds3PinConfig  = {TDS3_T2_GPIO_Port, TDS3_T1_GPIO_Port, ADC_CHANNEL_12, TDS3_T2_Pin, TDS3_T1_Pin, ADC_CHANNEL_13};
GPIOConfig tds4PinConfig  = {TDS4_T2_GPIO_Port, TDS4_T1_GPIO_Port, ADC_CHANNEL_1, TDS4_T2_Pin, TDS4_T1_Pin, ADC_CHANNEL_2};
TDS tds1 = TDS(tds1PinConfig);
TDS tds2 = TDS(tds2PinConfig);
TDS tds3 = TDS(tds3PinConfig);
TDS tds4 = TDS(tds4PinConfig);
//*******************************************************************
Menu menu;
WarningFlags TdsWarn = {.all_flags = 0};			  // all disabled by default
WarningFlags isTdsWarningEnable{.all_flags = 0xFFFF}; // all enabled by default
WarningFlags tempTdsWarning{.all_flags = 0xFFFF}; // all enabled by default

uint32_t lastTick = 0;
uint32_t delayTime = 5000; // Delay time in milliseconds


void fillOpen()
{
	Disp->display("Tank Full - System not Filling");
	Buzzzer->setOutputPattern(1, 500, 1000);
	((RgbDriver*)WLED1)->setRgbColor(RED);
	WLED1->setOutputPattern(1, 1000, 1000);

	invokeST3(false);
	invokeST5(false);
	Log.logToMaster("Float Circuit Open - Unable to commence filling");
	mainMenuFuncHeater();
	//add buzzer/LED warning handler here
}

void invokeST3(bool status)
{
	if(Flags.flagStatus(Status, MEM_SAVER) != status)
	{
		Flags.setFlag(Status, MEM_SAVER, status);
	}
	if(status)
	{
		Disp->display("Starting Purification - System Filling.");
		Flags.setFlag(Settings, MEM_SAVER, status);
		Flags.setFlag(Status, PURE_CYCLE, status);
		Log.logToMaster("Float Circuit Closed - System filling commenced");
		tempTdsWarning.all_flags = 0xFFFF;
		Buzzzer->setOutputPattern(2, 500, 500, 500);
		((RgbDriver *)WLED1)->setRgbColor(BLUE);
		WLED1->setOutputPattern(2, 500, 500, 500);					// -1 to run infinitely
	}
	else
	{
		Flags.setFlag(Settings, MEM_SAVER, false);
		// WLED1->turnOff();
	}
}

void invokeST1(bool status)
{
	if(Flags.flagStatus(Status, RESIN_SAVER) != status)
	{
		Flags.setFlag(Status, RESIN_SAVER, status);
	}
}

void invokeST4(bool status)
{
	if (Flags.flagStatus(Status, AUTO_FLUSH) != status)
	{
		Flags.setFlag(Status, AUTO_FLUSH, status);
	}
}

void invokeST5(bool status)
{
	if(Flags.flagStatus(Status, AUTO_FILL) != status)
	{
		Flags.setFlag(Status, AUTO_FILL, status);
	}
}

void invokeST7(bool status)
{
	if(Flags.flagStatus(Status, PUMP_1) != status)
	{
		Flags.setFlag(Status, PUMP_1, status);
	}
}


void mainHeaterOn()
{

	Flags.update();

	if (Flags.flagStatus(Status, MAIN_HEATER) ==  true)
	{
		if(Flags.flagStatus(Status, PUMP1) ==  true)
		{
			Disp->display("(Flashing ! Icon) Heater ON Water not heating reconnect reel hose");
			Flags.setFlag(Status, MAIN_HEATER, false);
		}
		else if(Flags.flagStatus(Status, PUMP1) ==  false)
		{
			if(HAL_GetTick() - lastTick < delayTime) // delay define on the start of file
			{
			    lastTick = HAL_GetTick();
			    Disp->display("(Flashing Flame Icon) Preheating");
			}
			else
			{
				Disp->display("(Flame Icon) Heater ON");
			}
		}
	}

}

void controlPeripherals(void)
{
	mainHeaterOn();
}


    //     if(!pumpState)
    //     {
    //         Disp->display("(Flashing ! Icon) Heater ON Water not heating reconnect reel hose");
    //     }
    //     else if(HAL_GetTick() - lastTick < delayTime)
    //     {
    //         lastTick = HAL_GetTick();
    //         Disp->display("(Flashing Flame Icon) Preheating");
    //     }
    //     else
    //     {
    //         Disp->display("(Flame Icon) Heater ON");
    //     }











#endif
