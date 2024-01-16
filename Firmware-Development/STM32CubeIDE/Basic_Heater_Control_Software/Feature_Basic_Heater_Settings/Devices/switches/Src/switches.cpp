/*
 * switches.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan Ahmed
 */
#include "switches.hpp"


void Switches::getState(SWITCHES SW)
{
	 if(isButtonPressed(SW))
	 {
		 if(isButtonStillPressed(SW))
		 {
			 resetButtonState(SW);
			 timeStamped = false;
			 button.period = HAL_GetTick()-buttonPressTime;
			 if(button.period > DEBOUNCEPERIOD)
			 {
				 button.state = true;
			 }
			 else
			 {
				 button.state = false;
			 }
		 }
	 }
}

bool Switches::state()
{
	return button.state;
}

uint16_t Switches::period()
{
	return button.period;
}

bool Switches::isButtonPressed(SWITCHES SW)
{
	switch(SW)	{
		case EnterOK:
			return ENTER_Flag;
			break;
		case BACK:
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
			return false;
			break;
	}
}

bool Switches::isButtonStillPressed(SWITCHES SW)
{
	switch(SW)	{
		case EnterOK:
			return HAL_GPIO_ReadPin(ENTERBUTTONPORT, ENTERBUTTON);
			break;
		case BACK:
			return HAL_GPIO_ReadPin(BACKBUTTONPORT, BACKBUTTON);
			break;
		case UP:
			return HAL_GPIO_ReadPin(UPBUTTONPORT, UPBUTTON);
			break;
		case DOWN:
			return HAL_GPIO_ReadPin(DOWNBUTTONPORT, DOWNBUTTON);
			break;
//		case RODI:
//				return HAL_GPIO_ReadPin(RODIBUTTONPORT, RODIBUTTON);
//				break;
//		case DEL:
//				return HAL_GPIO_ReadPin(DELBUTTONPORT, DELBUTTON);
//				break;
		default:
			return false;
			break;
	}
}

void Switches::resetButtonState(SWITCHES SW)
{
	switch(SW)	{
			case EnterOK:
				ENTER_Flag = false;
				break;
			case BACK:
				BACK_Flag = false;
				break;
			case UP:
				UP_Flag = false;
				break;
			case DOWN:
				DOWN_Flag = false;
				break;
//			case RODI:
//				RODI_Flag = false;
//				break;
//			case DEL:
//				DEL_Flag = false;
//				break;
			default:
				break;
		}
}

void Switches::reset()
{
	button = {false, 0};
}

void saveTime(){
	buttonPressTime = HAL_GetTick();
	timeStamped = true;
}
