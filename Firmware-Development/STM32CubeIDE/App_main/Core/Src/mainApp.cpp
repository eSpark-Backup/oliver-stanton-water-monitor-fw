#include "main.h"
#include "mainApp.hpp"
#include "usb_device.h"
#include "stm32f0xx_hal.h"
#include "lcd.h"
#include "switches.hpp"
#include "main.h"
#include "motion.hpp"
#include <functional>


Switches switche;
extiCallback extiPtr;
timerCallback timerPtr;
motionCallback motionPtr;
MotionSensor motion;
MotionSensor* mtnPtr=&motion;
LCD lcdObj;
extern TIM_HandleTypeDef htim2;


void MainApp::setup(){
	switche.setState(GPIOE, PWM_CNTRL_Pin,GPIO_PIN_SET);
	switche.setState(GPIOE,PWM_CNTRL_Pin,GPIO_PIN_SET);
	registerCallback(timerElapsed);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim2);
}

void MainApp::loop(){
	if(!switche.getState(SW2_GPIO_Port, SW2_Pin))
	{
		lcdObj.lcdSequence();
	}
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	if(motion.getMotionFlag()){
		if(motion.getMotionWU()==true){
			lcdObj.lcdSequence();
			}
		if(motion.getMotionWelcome()==true)
			{
				switche.setState(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_SET);
				HAL_TIM_Base_Start_IT(&htim2);
				lcdObj.lcdSequence(motion.getMotionWelcome());
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
		registerCallback(WLED_callback);
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

void WLED_callback(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim)
{
	if(GPIO_Pin == GPIO_PIN_5) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
    	switche.setState(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_SET);
    	HAL_TIM_Base_Start_IT(htim);
    }
}

void timerElapsed(TIM_HandleTypeDef* htim)
{
	switche.setState(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_RESET);
	HAL_TIM_Base_Stop_IT(htim);
}
