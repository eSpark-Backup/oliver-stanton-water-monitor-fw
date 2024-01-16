/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "controls.h"

#define FODAUTORUNDELAYPERIOD		60000				//60secs
#define MEMSAVERPAUSETIMEOUT		900000				//15mins or 900secs

short roSaverThreshValue = 0;
//short currentRoValue;

//***************************Buzzer*******************************
EventHandle Buzzzer = EventHandle(BUZZER_GPIO_Port, BUZZER_Pin, true);
EventHandle WLED1 = EventHandle(W_LED_CTRL_GPIO_Port, W_LED_CTRL_Pin, false);

//***************************TDS Sensor*******************************
GPIOConfig TDS1 = {TDS1_T2_GPIO_Port, TDS1_T1_GPIO_Port, ADC_CHANNEL_10, TDS1_T2_Pin, TDS1_T1_Pin, ADC_CHANNEL_11};
GPIOConfig TDS2 = {TDS2_T2_GPIO_Port, TDS2_T1_GPIO_Port, ADC_CHANNEL_14, TDS2_T2_Pin, TDS2_T1_Pin, ADC_CHANNEL_15};
GPIOConfig TDS3 = {TDS3_T2_GPIO_Port, TDS3_T1_GPIO_Port, ADC_CHANNEL_12, TDS3_T2_Pin, TDS3_T1_Pin, ADC_CHANNEL_13};
GPIOConfig TDS4 = {TDS4_T2_GPIO_Port, TDS4_T1_GPIO_Port, ADC_CHANNEL_1, TDS4_T2_Pin, TDS4_T1_Pin, ADC_CHANNEL_2};
TDS tds1 = TDS(TDS1);
TDS tds2 = TDS(TDS2);
TDS tds3 = TDS(TDS3);
TDS tds4 = TDS(TDS4);
//*******************************************************************
Menu menu;

//--------------------------------Functions definitions-------------------------


//***************************TDS Sensor*******************************
float tds_Sensor(uint32_t channel){
	if(channel == 14 || channel == 15)
		return tds2.readTDS();
	else if (channel == 1 || channel == 2)
		return tds4.readTDS();
	return 0;
}
//*******************************************************************


//***********************Purefication Cycle**************************
bool fodOFF(uint32_t){
	if(Flags.flagStatus(Settings, FoD)){
		Flags.setFlag(Settings, FoD, false);
		DEBUG_W("Fill on Demand disabled");
	}
	if(ButtonE.period() > SHORTPRESS && ButtonE.period() < LONGLONGPRESS){
		Disp->displayString("System Purification stopped.");
		Flags.setFlag(Settings, MEM_Saver, false);
		Flags.setFlag(Settings, Mem_Saver_Pause, false);
		invokeST3(false);
		if(!roSaverST1())						//call RO_Saver function here
			mainMenuFunc();
//		return true;
	}
	else if (ButtonE.period() < SINGLEPRESS){
			Flags.setFlag(Settings, Mem_Saver_Pause, false);
			if(Flags.flagStatus(Status, Level_Sensor) == true){
				Flags.setFlag(Settings, MEM_Saver, false);
				fillOpen();
				invokeST3(false);	}
			else if(!Flags.flagStatus(Settings, MEM_Saver)){
//				Disp->displayString("Starting Purification - System Filling.");
//				Flags.setFlag(Settings, MEM_Saver, true);
//				Flags.setFlag(Status, Pure_CYCLE, true);
				invokeST3(true);	}
			else{
				Flags.setFlag(Settings, MEM_Saver, false);
				Flags.setFlag(Settings, Mem_Saver_Pause, true);
				Disp->displayString("System Purification paused.");
				invokeST3(false);	}
	}
	return false;
}

bool fodON(uint32_t){
	Flags.setFlag(Settings, FoD, true);
	Flags.setFlag(Settings, MEM_Saver, true);
	return true;
}

bool roSaverThreshAdj(uint32_t c){
	roSaverThreshValue = c;
	DEBUG_W("RO_Saver thresh value set to %d.", roSaverThreshValue);
	return true;
}
bool roSaverThreshDefault(uint32_t c){
	roSaverThreshValue = 0;
	DEBUG_W("RO_Saver thresh value set to %d.", roSaverThreshValue);
	return true;
}
bool roSaverOn(uint32_t c){
	Flags.setFlag(Settings, RESIN_Saver, true);
	DEBUG_W("RO_Saver setting Turned on.");
	return true;
}
bool roSaverOff(uint32_t c){
	Flags.setFlag(Settings, RESIN_Saver, false);
	DEBUG_W("RO_Saver setting Turned off.");
	return true;
}

void mainMenuFunc(){
	currentM = (int*)(mainMenu);
	menu.displayMenu(mainMenu, mainMenu->num_items);
}
void fillOpen(){
	Disp->displayString("Tank Full - System not Filling");
	invokeST3(false);
	DEBUG_W("SAVE LOG TO FILE HERE.");			//perform logging to file here
	mainMenuFunc();
	//add buzzer/LED warning handler here
}

void invokeST3(bool status){
	if(Flags.flagStatus(Status, MEM_Saver) != status){
		Flags.setFlag(Status, MEM_Saver, status);
	}
	if(status)
	{
		Disp->displayString("Starting Purification - System Filling.");
		Flags.setFlag(Settings, MEM_Saver, status);
		Flags.setFlag(Status, Pure_CYCLE, status);
		//perform logging function here
		DEBUG_W("SAVE LOG TO FILE HERE.");			//perform logging to file here

		Buzzzer.Init(2, 500, 1100, 500);
		WLED1.Init(-1, 1000, 1000);				// -1 to run infinitely
	}
	else
	{
		Flags.setFlag(Settings, MEM_Saver, false);
		WLED1.turnOff();
	}
}

void invokeST1(bool status){
	if(Flags.flagStatus(Status, RESIN_Saver) != status){
		Flags.setFlag(Status, RESIN_Saver, status);
	}
}

void invokeST5(bool status){
	if(Flags.flagStatus(Status, AUTO_Fill) != status){
		Flags.setFlag(Status, AUTO_Fill, status);
	}
}

bool roSaverST1(){
	Flags.update();
	if(Flags.flagStatus(Status, Pure_CYCLE) && Flags.flagStatus(Settings, RESIN_Saver) && !Flags.flagStatus(Status, MEM_Saver) && !Flags.flagStatus(Status, AUTO_Fill)){
		Disp->displayString("Completing Purification Cycle - Dosing RO Membrane.");
		invokeST1(true);
		invokeST5(true);
		Flags.update();
		return true;
	}
	else if(Flags.flagStatus(Status, Pure_CYCLE) && Flags.flagStatus(Status, AUTO_Fill)){
		if(tds2.readTDS() < roSaverThreshValue){
		Disp->displayString("Purification Cycle Complete.");
		invokeST1(false);
		invokeST5(false);
		Flags.update();
		Flags.setFlag(Status, Pure_CYCLE, false);
		mainMenuFunc();
		return true;	}
	}
	else if(!Flags.flagStatus(Settings, RESIN_Saver) && Flags.flagStatus(Status, Pure_CYCLE) && Flags.flagStatus(Status, Level_Sensor)){
		Flags.setFlag(Status, Pure_CYCLE, false);
		Disp->displayString("Tank Full, System Purification Stopped.");
		mainMenuFunc();
		return true;
	}
//	else
		return false;
}

void memSaverST3(){
	Flags.update();
	if(Flags.timeElapsed(MEM_Saver, MEMSAVERPAUSETIMEOUT) && Flags.flagStatus(Settings, Mem_Saver_Pause)){
		Flags.setFlag(Settings, Mem_Saver_Pause, false);
		DEBUG_W("***MEMSAVERPAUSETIMEOUT OCCURRED***");
		if(!roSaverST1())
			mainMenuFunc();
	}
	if(Flags.flagStatus(Status, Level_Sensor) != false &&
	   Flags.flagStatus(Status, MEM_Saver) &&
	   (Flags.flagStatus(Settings, MEM_Saver) || Flags.flagStatus(Settings, FoD) == true))
	{
			invokeST3(false);
//			Flags.setFlag(Settings, MEM_Saver, false);
			roSaverST1();
	}
	else if(Flags.timeElapsed(Level_Sensor, FODAUTORUNDELAYPERIOD) &&
			Flags.flagStatus(Settings, FoD) == true &&
			Flags.flagStatus(Status, Level_Sensor) != true
			&& Flags.flagStatus(Status, MEM_Saver) != true)
	{
//				Disp->displayString("Starting Purification - System Filling.");
//				Flags.setFlag(Settings, MEM_Saver, true);
//				Flags.setFlag(Status, Pure_CYCLE, true);
//				DEBUG_W("SAVE LOG TO FILE HERE.");			//perform logging to file here

				invokeST3(true);
	}
}
