/*
 * switches.cpp
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan Ahmed
 */
#include "switches.hpp"
#include "Battery.hpp"


void Switches::getState(SWITCHES SW)
{
	if(isButtonPressed(SW))
	{
		if(isButtonStillPressed(SW))
		{
			resetButtonState(SW);
			timeStamped = false;
			Button.period = HAL_GetTick()-buttonPressTime;
			if(Button.period > DEBOUNCE_PERIOD)
			{
				Button.state = true;
			}
			else
			{
				Button.state = false;
			}
		}
	 }
}

bool Switches::state()
{
	return Button.state;
}

uint16_t Switches::period()
{
	return Button.period;
}

void Switches::enableButton(bool enable)
{
	Button.enabled = enable;
}

bool Switches::isButtonEnabled()
{
	return Button.enabled;
}

bool Switches::isButtonPressed(SWITCHES SW)
{
		switch (SW)
		{
		case ENTER_OK:
			return ENTER_BUTTON_FLAG;
			break;
		case BACK:
			return BACK_BUTTON_FLAG;
			break;
		case UP:
			return UP_BUTTON_FLAG;
			break;
		case DOWN:
			return DOWN_BUTTON_FLAG;
			break;
		case RODI:
			return RODI_BUTTON_FLAG;
			break;
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
		case ENTER_OK:
			return HAL_GPIO_ReadPin(ENTER_BUTTON_PORT, ENTER_BUTTON);
			break;
		case BACK:
			return HAL_GPIO_ReadPin(BACK_BUTTON_PORT, BACK_BUTTON);
			break;
		case UP:
			return HAL_GPIO_ReadPin(UP_BUTTON_PORT, UP_BUTTON);
			break;
		case DOWN:
			return HAL_GPIO_ReadPin(DOWN_BUTTON_PORT, DOWN_BUTTON);
			break;
		case RODI:
			return HAL_GPIO_ReadPin(RODI_BUTTON_PORT, RODI_BUTTON);
			break;
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
			case ENTER_OK:
				ENTER_BUTTON_FLAG = false;
				break;
			case BACK:
				BACK_BUTTON_FLAG = false;
				break;
			case UP:
				UP_BUTTON_FLAG = false;
				break;
			case DOWN:
				DOWN_BUTTON_FLAG = false;
				break;
			case RODI:
				RODI_BUTTON_FLAG = false;
				break;
//			case DEL:
//				DEL_Flag = false;
//				break;
			default:
				break;
		}
}

void Switches::reset()
{
	Battery.getMonitor().isActivityDetected = true;
	Button = {0, false, Button.enabled};
}

void saveTime()
{
	buttonPressTime = HAL_GetTick();
	timeStamped = true;
}