#include "main.h"
#include "mainApp.hpp"
#include "usb_device.h"
#include "stm32f0xx_hal.h"
#include "lcd.h"
#include "switches.hpp"
#include "motion.hpp"
#include "LED.h"
#include <functional>

Switches switche;
extiCallback extiPtr;
timerCallback timerPtr;
motionCallback motionPtr;
MotionSensor motion;
MotionSensor* mtnPtr=&motion;
LCD lcdObj;
W_LED wLED;
extern TIM_HandleTypeDef htim2;


void MainApp::setup(){
	HAL_GPIO_WritePin(GPIOE, PWM_CNTRL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,PWM_CNTRL_Pin,GPIO_PIN_SET);
	registerCallback(timerElapsed);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim2);
}

void MainApp::loop(){
	if(!switche.getState(SW2))
	{
		lcdObj.sequence();
	}
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		  if(motion.getMotionFlag())
		  {
			  if(motion.getMotionWU()==true)
			  {
				  lcdObj.sequence();
			  }
			  if(motion.getMotionWelcome()==true)
			  {
				wLED.setWLED();
				HAL_TIM_Base_Start_IT(&htim2);
				lcdObj.sequence(motion.getMotionWelcome());
			  }
			  motion.setMotionFlag(0);
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
	if(GPIO_Pin==SW5_Pin){
		registerCallback(WLEDCallback);
		if(extiPtr!=NULL)
			{
				extiPtr(GPIO_Pin,&htim2);
			}
	}
	else if(GPIO_Pin==PIR_OUT_Pin){

		motion.setup();
		if(motionPtr!=NULL)
			{
				motionPtr();
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
	if(GPIO_Pin == GPIO_PIN_5) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
		wLED.setWLED();
    	HAL_TIM_Base_Start_IT(htim);
    }
}

void timerElapsed(TIM_HandleTypeDef* htim)
{
	wLED.resetWLED();
	HAL_TIM_Base_Stop_IT(htim);
}
