/*
 * switches.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan Ahmed
 */
#include "switches.hpp"

//BUTTON button;


BUTTON Switches::getState(SWITCHES SW)
{
	BUTTON button;
	uint16_t period = Debounce_Period;
	 if(isButtonPressed(SW)){
		 if(isButtonStillPressed(SW)){
			 resetButtonState(SW);
			 timeStamped = false;
			 button.period = HAL_GetTick()-buttonPressTime;
			 if(button.period > period){
				 button.state = true;
//				 resetButtonState(SW);
//				 timeStamped = false;
				 return button;		}
			 else	{
				 button.state = false;

				 		}
		 }
	 }
}

bool Switches::getState(SWITCHES SW, uint16_t period)
{
	BUTTON button;
	if(period <  Debounce_Period)
		period = Debounce_Period;
	if(isButtonPressed(SW)){
	 if(isButtonStillPressed(SW)){
		 button.period = HAL_GetTick()-buttonPressTime;
		 if(button.period > period)
			 button.state = true;
		 else
			 button.state = false;
		 resetButtonState(SW);
		 timeStamped = false;
		 return button.state;		}
//			 timeStamped = false;
	 	 }
//	 else {
//		 button.state = false;
//		 button.period = 0;
//	 }
//	 while(!resetButtonState(SW)){
//	 }
//	if(button.period >= period)
//			 return button;
}

bool Switches::isButtonPressed(SWITCHES SW){
	switch(SW)	{
		case EnterOK:
			return ENTER_Flag;
			break;
		case Back:
			return BACK_Flag;
			break;
		case UP:
			return UP_Flag;
			break;
		case DOWN:
			return DOWN_Flag;
			break;
//		case RODI:
//			return RODI_Flag;
//			break;
//		case DEL:
//			return DEL_Flag;
//			break;
		default:
			break;
	}
}

bool Switches::isButtonStillPressed(SWITCHES SW){
	switch(SW)	{
		case EnterOK:
			return HAL_GPIO_ReadPin(ENTER_Button_Port, ENTER_Button);
			break;
		case Back:
			return HAL_GPIO_ReadPin(BACK_Button_Port, BACK_Button);
			break;
		case UP:
			return HAL_GPIO_ReadPin(UP_Button_Port, UP_Button);
			break;
		case DOWN:
			return HAL_GPIO_ReadPin(DOWN_Button_Port, DOWN_Button);
			break;
//		case RODI:
//				return HAL_GPIO_ReadPin(RODI_Button_Port, RODI_Button);
//				break;
//		case DEL:
//				return HAL_GPIO_ReadPin(DEL_Button_Port, DEL_Button);
//				break;
		default:
			break;
	}
//	return false;
}

bool Switches::resetButtonState(SWITCHES SW){
	switch(SW)	{
			case EnterOK:
				ENTER_Flag = false;
				return 1;
				break;
			case Back:
				BACK_Flag = false;
				return 1;
				break;
			case UP:
				UP_Flag = false;
				return 1;
				break;
			case DOWN:
				DOWN_Flag = false;
				return 1;
				break;
//			case RODI:
//				RODI_Flag = false;
//				return 1;
//				break;
//			case DEL:
//				DEL_Flag = false;
//				return 1;
//				break;
			default:
				return 0;
				break;
		}
}

void Switches::saveTime(){
	buttonPressTime = HAL_GetTick();
	timeStamped = true;
}
