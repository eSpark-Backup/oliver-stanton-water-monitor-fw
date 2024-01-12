/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "controls.h"

#define FoD_autoRun_delay_Period		60000				//60secs
#define MEM_Saver_Pause_Timeout			900000				//15mins or 900secs

short ro_saver_thresh_value = 0;
short current_ro_value;

//***************************Buzzer*******************************
eventHandle Buzzzer = eventHandle(BUZZER_GPIO_Port, BUZZER_Pin, true);
eventHandle WLED1 = eventHandle(W_LED_CTRL_GPIO_Port, W_LED_CTRL_Pin, false);

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


//--------------------------------Functions definitions-------------------------


//***************************TDS Sensor*******************************
float tds_Sensor(uint32_t channel){
	if(channel == 14 || channel == 15)
		return tds2.readTDS();
	else if (channel == 1 || channel == 2)
		return tds4.readTDS();
}
//*******************************************************************


//***********************Purefication Cycle**************************
bool fodOFF(uint32_t){
	if(flags.flagStatus(Settings, FoD)){
		flags.setFlag(Settings, FoD, false);
		DEBUG_W("Fill on Demand disabled");
	}
	if(buttonStateE.period > short_press && buttonStateE.period < long_long_press){
		disp->displayString("System Purification stopped.");
		flags.setFlag(Settings, MEM_Saver, false);
		flags.setFlag(Settings, Mem_Saver_Pause, false);
		invoke_ST3(false);
		if(!RO_Saver_ST1())						//call RO_Saver function here
			mainMenufunc();
//		return true;
	}
	else if (buttonStateE.period < single_press){
			flags.setFlag(Settings, Mem_Saver_Pause, false);
			if(flags.flagStatus(Status, Level_Sensor) == true){
				flags.setFlag(Settings, MEM_Saver, false);
				fillOpen();
				invoke_ST3(false);	}
			else if(!flags.flagStatus(Settings, MEM_Saver)){
				flags.setFlag(Settings, MEM_Saver, true);
				disp->displayString("Starting Purification - System Filling.");
				invoke_ST3(true);	}
			else{
				flags.setFlag(Settings, MEM_Saver, false);
				flags.setFlag(Settings, Mem_Saver_Pause, true);
				disp->displayString("System Purification paused.");
				invoke_ST3(false);	}
	}
	return false;
}

bool fodON(uint32_t){
	flags.setFlag(Settings, FoD, true);
	flags.setFlag(Settings, MEM_Saver, true);
	return true;
}

bool ro_saver_thresh_adj(uint32_t c){
	ro_saver_thresh_value = c;
	DEBUG_W("RO_Saver thresh value set to %d.", ro_saver_thresh_value);
	return true;
}
bool ro_saver_thresh_default(uint32_t c){
	ro_saver_thresh_value = 0;
	DEBUG_W("RO_Saver thresh value set to %d.", ro_saver_thresh_value);
	return true;
}
bool ro_saver_On(uint32_t c){
	flags.setFlag(Settings, RESIN_Saver, true);
	DEBUG_W("RO_Saver setting Turned on.");
	return true;
}
bool ro_saver_Off(uint32_t c){
	flags.setFlag(Settings, RESIN_Saver, false);
	DEBUG_W("RO_Saver setting Turned off.");
	return true;
}

void mainMenufunc(){
	currentM = (int*)(mainMenu);
	menu.displayMenu(mainMenu, mainMenu->num_items);
}
void fillOpen(){
	disp->displayString("Tank Full - System not Filling");
	invoke_ST3(false);
	mainMenufunc();
	//add buzzer/LED warning handler here
}

void invoke_ST3(bool status){
	if(flags.flagStatus(Status, MEM_Saver) != status){
		flags.setFlag(Status, MEM_Saver, status);
	}
	if(status)
	{
		Buzzzer.Init(2, 500, 1100, 500);
		WLED1.Init(-1, 1000, 1000);				// -1 to run infinitely
	}
	else
		WLED1.TurnOff();
}

void invoke_ST1(bool status){
	if(flags.flagStatus(Status, RESIN_Saver) != status){
		flags.setFlag(Status, RESIN_Saver, status);
	}
}

void invoke_ST5(bool status){
	if(flags.flagStatus(Status, AUTO_Fill) != status){
		flags.setFlag(Status, AUTO_Fill, status);
	}
}

bool RO_Saver_ST1(){
	flags.update();
//	DEBUG_W("RO_Saver_ST1() called.");
//	DEBUG_W("flags.flagStatus(Settings, RESIN_Saver) %d", flags.flagStatus(Settings, RESIN_Saver));
//	DEBUG_W("flags.flagStatus(Settings, MEM_Saver) %d", flags.flagStatus(Settings, MEM_Saver));

	if(flags.flagStatus(Status, Pure_CYCLE) && flags.flagStatus(Settings, RESIN_Saver) && !flags.flagStatus(Status, MEM_Saver) && !flags.flagStatus(Status, AUTO_Fill)){
		disp->displayString("Completing Purification Cycle - Dosing RO Membrane.");
		invoke_ST1(true);
		invoke_ST5(true);
		flags.update();
		return true;
	}
	else if(flags.flagStatus(Status, Pure_CYCLE) && flags.flagStatus(Status, AUTO_Fill)){
		if(tds2.readTDS() < ro_saver_thresh_value){
		disp->displayString("Purification Cycle Complete.");
		invoke_ST1(false);
		invoke_ST5(false);
		flags.update();
		flags.setFlag(Status, Pure_CYCLE, false);
		mainMenufunc();
		return true;	}
	}
	else if(!flags.flagStatus(Settings, RESIN_Saver) && flags.flagStatus(Status, Pure_CYCLE) && flags.flagStatus(Status, Level_Sensor)){
		flags.setFlag(Status, Pure_CYCLE, false);
		disp->displayString("Tank Full, System Purification Stopped.");
		mainMenufunc();
		return true;
	}
	else
		return false;
}

void Mem_Saver_ST3(){
	flags.update();
	if(flags.timeElapsed(MEM_Saver, MEM_Saver_Pause_Timeout) && flags.flagStatus(Settings, Mem_Saver_Pause)){
		flags.setFlag(Settings, Mem_Saver_Pause, false);
		DEBUG_W("***TIMEOUT OCCURRED***");
		if(!RO_Saver_ST1())
			mainMenufunc();
	}
	if(flags.flagStatus(Status, Level_Sensor) != false &&
	   flags.flagStatus(Status, MEM_Saver) &&
	   (flags.flagStatus(Settings, MEM_Saver) || flags.flagStatus(Settings, FoD) == true))
	{
			invoke_ST3(false);
			RO_Saver_ST1();
	}
	else if(flags.timeElapsed(Level_Sensor, FoD_autoRun_delay_Period) &&
			flags.flagStatus(Settings, FoD) == true &&
			flags.flagStatus(Status, Level_Sensor) != true
			&& flags.flagStatus(Status, MEM_Saver) != true)
	{
				disp->displayString("Starting Purification - System Filling.");
				flags.setFlag(Settings, MEM_Saver, true);
				flags.setFlag(Status, Pure_CYCLE, true);

				invoke_ST3(true);
	}
}

//void Buzzer_BUZZ(uint16_t x){
////	TIM1->CCR1 = 50;
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//	HAL_Delay(x);
//	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
//}


