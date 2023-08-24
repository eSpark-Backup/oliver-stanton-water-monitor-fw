#include "main.h"
#include "mainApp.hpp"
#include "usb_device.h"
#include "stm32f0xx_hal.h"
#include "lcd.h"
#include "switches.hpp"
#include "motion.hpp"
#include "LED.h"
#include <functional>
#include "menu.h"

Switches switche;
extiCallback extiPtr;
timerCallback timerPtr;
motionCallback motionPtr;
LCD lcdObj;
W_LED wLED;
Menu menu;
bool flag=false;
extern TIM_HandleTypeDef htim2;

const char* mainMenu[]= {"Heater: On","Frost Prev: On","Frost Pump: Off",NULL};

void MainApp::setup()
{
	HAL_GPIO_WritePin(GPIOE, PWM_CNTRL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,PWM_CNTRL_Pin,GPIO_PIN_SET);
	registerCallback(timerElapsed);
	registerCallback(WLEDCallback);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim2);
	lcdObj.start();
}

void MainApp::loop(){
	if(!switche.getState(SW2) && !flag)
	{
		menu.createMenu(mainMenu);
		flag=true;
	}
	if(!switche.getState(SW2) && flag)
	{
		menu.next();
	}
	if(!switche.getState(SW6) && flag)
	{
		HAL_Delay(250);
		menu.arrowDown();
	}
	if(!switche.getState(SW4) && flag)
	{
		HAL_Delay(250);
		menu.arrowUp();
	}
}

void registerCallback(extiCallback ptr)
{
	extiPtr=ptr;
}

void registerCallback(timerCallback ptr)
{
	timerPtr=ptr;
}

void registerCallback(motionCallback ptr)
{
	motionPtr=ptr;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==SW5_Pin)
	{
		if(extiPtr!=NULL)
			{
				extiPtr(GPIO_Pin,&htim2);
			}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(timerPtr!=NULL)
		{
			timerPtr(htim);
		}
}

void WLEDCallback(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim)
{
		wLED.set();
    	HAL_TIM_Base_Start_IT(htim);
}

void timerElapsed(TIM_HandleTypeDef* htim)
{
	wLED.reset();
	HAL_TIM_Base_Stop_IT(htim);
}



