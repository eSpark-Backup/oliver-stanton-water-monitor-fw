/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "controls.h"

#define FoD_autoRun_delay_Period		6000				//60secs
#define MEM_Saver_Pause_Timeout			20000				//15mins or 900secs

short ro_saver_thresh_value = 0;
short current_ro_value = 50;

//--------------------------------Functions definitions-------------------------

bool fodOFF(uint32_t){
//    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	if(flags.flagStatus(Settings, FoD)){
		flags.setFlag(Settings, FoD, false);
#ifdef _DEBUG_
	dispdebug->displayVar(flags.flagStatus(Settings, FoD), "FoD setting value set to: ");
#endif
	}
	if(buttonStateE.period > short_press && buttonStateE.period < long_long_press){
		disp->displayString("System Purification stopped.");
		//using inverse logic for Settings.MEM_Saver
		flags.setFlag(Settings, MEM_Saver, false);
		flags.setFlag(Settings, Mem_Saver_Pause, false);
		invoke_ST3(false);
//		flags.setFlag(Settings, RESIN_Saver, true);			//call RO_Saver function here
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
#ifdef _DEBUG_
	disp->displayVar(flags.flagStatus(Settings, MEM_Saver), "flags.flagStatus(Settings, MEM_Saver) : ");
#endif
	return false;
}

bool fodON(uint32_t){
	flags.setFlag(Settings, FoD, true);
	flags.setFlag(Settings, MEM_Saver, true);
#ifdef _DEBUG_
	dispdebug->displayVar(flags.flagStatus(Settings, FoD), "FoD setting value set to: ");
#endif
	return true;
}

bool ro_saver_thresh_adj(uint32_t c){
	ro_saver_thresh_value = c;
#ifdef _DEBUG_
	dispdebug->displayVar(ro_saver_thresh_value, "ro_saver_thresh_value set to: ");
#endif
	return true;
}
bool ro_saver_thresh_default(uint32_t c){
	ro_saver_thresh_value = 0;
#ifdef _DEBUG_
	dispdebug->displayVar(ro_saver_thresh_value, "ro_saver_thresh_value set to: ");
#endif
	return true;
}
bool ro_saver_On(uint32_t c){
	flags.setFlag(Settings, RESIN_Saver, true);
#ifdef _DEBUG_
	dispdebug->displayVar(flags.flagStatus(Settings, RESIN_Saver), "RO Saver Turned on. value set to: ");
#endif
	return true;
}
bool ro_saver_Off(uint32_t c){
	flags.setFlag(Settings, RESIN_Saver, false);
#ifdef _DEBUG_
	dispdebug->displayVar(flags.flagStatus(Settings, RESIN_Saver), "RO Saver Turned off. value set to: ");
#endif
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
		if(flags.flagStatus(Settings, MEM_Saver) == true){
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin, GPIO_PIN_SET);
#ifdef _DEBUG_
			dispdebug->displayVar(flags.flagStatus(Settings, MEM_Saver), "Mem_Saver_ST3 turned on. (Settings, MEM_Saver): ");
#endif
		}
		else if(flags.flagStatus(Settings, MEM_Saver) == false){
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin, GPIO_PIN_RESET);
#ifdef _DEBUG_
			dispdebug->displayVar(flags.flagStatus(Settings, MEM_Saver), "Mem_Saver_ST3 turned off. (Settings, MEM_Saver): ");
#endif
		}
	}
//	currentM = (int*)(mainMenu);
//	Buzzer_BUZZ(500);
//	HAL_Delay(50);
//	Buzzer_BUZZ(100);
}

void invoke_ST1(bool status){
	if(flags.flagStatus(Status, RESIN_Saver) != status){
		if(flags.flagStatus(Settings, RESIN_Saver) == true){
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(RO_SAVER_GPIO_Port, RO_SAVER_Pin, GPIO_PIN_SET);
#ifdef _DEBUG_
			dispdebug->displayVar(flags.flagStatus(Settings, MEM_Saver), "RESIN_Saver_ST1 turned on. (Settings, MEM_Saver): ");
#endif
		}
		else if(flags.flagStatus(Settings, RESIN_Saver) == false){
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(RO_SAVER_GPIO_Port, RO_SAVER_Pin, GPIO_PIN_RESET);
#ifdef _DEBUG_
			dispdebug->displayVar(flags.flagStatus(Settings, MEM_Saver), "RESIN_Saver_ST1 turned off. (Settings, MEM_Saver): ");
#endif
		}
	}
}

void invoke_ST5(bool status){
	if(flags.flagStatus(Status, RESIN_Saver) != status){
		if(flags.flagStatus(Settings, RESIN_Saver) == true){
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AUTO_FILL_GPIO_Port, AUTO_FILL_Pin, GPIO_PIN_SET);
#ifdef _DEBUG_
			dispdebug->displayVar(flags.flagStatus(Settings, MEM_Saver), "Mem_Saver_ST3 turned on. (Settings, MEM_Saver): ");
#endif
		}
		else if(flags.flagStatus(Settings, RESIN_Saver) == false){
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AUTO_FILL_GPIO_Port, AUTO_FILL_Pin, GPIO_PIN_RESET);
#ifdef _DEBUG_
			dispdebug->displayVar(flags.flagStatus(Settings, MEM_Saver), "Mem_Saver_ST3 turned off. (Settings, MEM_Saver): ");
#endif
		}
	}
}

void RO_Saver_ST1(){
	if(flags.flagStatus(Settings, RESIN_Saver)){
		disp->displayString("Completing Purification Cycle - Dosing RO Membrane.");
		uint32_t timevar = 0;
		while(current_ro_value != ro_saver_thresh_value){
			invoke_ST1(true);
			invoke_ST5(true);
			if(HAL_GetTick() - timevar > 1000) {
				disp->displayString("1Sec Passed.");
				current_ro_value -= 1;
				timevar = HAL_GetTick();	}
		}
	}
//	else{
//		disp->displayString("Tank Full - System Purification Stopped.");
//		mainMenufunc();
//	}
}

void Mem_Saver_ST3(){
//	bool levelSensor_timePeriod = flags.timeElapsed(Level_Sensor, FoD_autoRun_Period);
//	if(!flags.flagStatus(Settings, MEM_Saver))
//		bool MEMSaver_timePeriod = flags.timeElapsed(MEM_Saver, MEM_Saver_Pause_Timeout);
	if(flags.timeElapsed(MEM_Saver, MEM_Saver_Pause_Timeout) && flags.flagStatus(Settings, Mem_Saver_Pause)){
		flags.setFlag(Settings, Mem_Saver_Pause, false);
		dispdebug->displayString("***TIMEOUT OCCURRED***");

	}
	if(flags.flagStatus(Status, Level_Sensor) != false &&
	   flags.flagStatus(Status, MEM_Saver) &&
	   (flags.flagStatus(Settings, MEM_Saver) || flags.flagStatus(Settings, FoD) == true)){
			flags.setFlag(Settings, MEM_Saver, false);
//			flags.setFlag(Settings, FoD, false);
//			flags.flagStatus(Settings, FoD) == true
//			disp->displayString("System Purification stopped.");
//			dispdebug->displayVar(flags.timeElapsed(MEM_Saver, MEM_Saver_Pause_Timeout), "is time elapsed true? ");
#ifdef _DEBUG_
			dispdebug->displayVar(flags.flagStatus(Status, MEM_Saver), " FOD Off(Status, MEM_Saver): ");
			dispdebug->displayString("   ");
#endif
//			invoke_ST3(false);
			fillOpen();
	}
	else if(flags.timeElapsed(Level_Sensor, FoD_autoRun_delay_Period) &&
			flags.flagStatus(Settings, FoD) == true &&
//	   		flags.flagStatus(Settings, MEM_Saver) != false &&
			flags.flagStatus(Status, Level_Sensor) != true &&
			flags.flagStatus(Status, MEM_Saver) != true)	{
				disp->displayString("Starting Purification - System Filling.");
				flags.setFlag(Settings, MEM_Saver, true);
#ifdef _DEBUG_
				dispdebug->displayVar(flags.flagStatus(Status, MEM_Saver), " FOD On(Status, MEM_Saver): ");
				dispdebug->displayString("   ");
#endif
				invoke_ST3(true);
	}
}

void Buzzer_BUZZ(uint16_t x){
	TIM1->CCR1 = 50;
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_Delay(x);
//	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

//void debug(){
//
//}





