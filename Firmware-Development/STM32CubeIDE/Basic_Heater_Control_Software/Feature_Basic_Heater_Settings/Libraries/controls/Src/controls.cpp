/*
 * controls.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan AHmed
 */

//#include "mainApp.hpp"
#include "controls.h"



bool PureFill(){
//    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	flags.setFlag(Settings, FoD, false);
	if(flags.flagStatus(Status, Level_Sensor)){
		fillOpen();
	}
	else{
		fillClosed();
	}
	return false;
}

void fillOpen(){
	disp->displayString("Tank Full - System not Filling");
	flags.setFlag(Settings, FoD, false);
	currentM = (int*)(mainMenu);
	HAL_Delay(1);  		menu.displayMenu(mainMenu, mainMenu->num_items);
}
void fillClosed(){
	disp->displayString("Starting Purification - System Filling");
//	flags.setFlag(Status, Mem_Saver, true);
//	HAL_GPIO_WritePin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin, GPIO_PIN_SET);
	currentM = (int*)(mainMenu);
	Buzzer_BUZZ(500);
	HAL_Delay(50);
	Buzzer_BUZZ(100);
}
bool fodON(){
	flags.setFlag(Settings, FoD, true);
	disp->displayVar(flags.flagStatus(Settings, Level_Sensor), "FoD set to value");
	return true;
}

bool fodRun(){
	if(flags.timeElapsed(Level_Sensor, 5000) && flags.flagStatus(Settings, FoD) == true && flags.flagStatus(Status, Level_Sensor) != true){
		fillClosed();
//		HAL_Delay(1);  	CDC_Transmit_FS((u int8_t *)"turned on\n", sizeof("turned on\n")-1);
		return false;
	}
	else
		return true;
}

void Buzzer_BUZZ(uint16_t x){
	TIM1->CCR1 = 50;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_Delay(x);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}
