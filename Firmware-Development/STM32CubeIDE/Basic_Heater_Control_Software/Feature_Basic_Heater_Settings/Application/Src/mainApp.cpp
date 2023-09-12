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
#include "motorDriver.h"
#include "flash.h"
#include "callbacks.h"

Switches switche;

//motionCallback motionPtr;
extern ADC_HandleTypeDef hadc;
extern extiCallback extiPtr;
extern timerCallback timerPtr;
extern flameCallback flamePtr;
extern pumpCallback pumpConPtr;
LCD lcdObj;
WLED wLED;
Menu menu;
MotorDriver motor;
FlashStorage flash;
bool flag=false;
bool flameFlag=false;
bool sw3Flag=true;
extern TIM_HandleTypeDef htim2;
extern int nextMenu;
#define HEATER_STATE_ADDRESS 0x0801F800
const char* mainMenu[]= {"Heater: ","Frost Prev: ","Frost Pump: ",NULL};

void MainApp::setup()
{
	HAL_GPIO_WritePin(GPIOE, PWM_CNTRL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,PWM_CNTRL_Pin,GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim2);
	lcdObj.start();
	motor.enable();
	motor.enableDT1();
	motor.enablePump1();
	motor.enablePump2();
	motor.setZeroReference();
}

void MainApp::loop()
{
	if((!switche.getState(SW2) && !flag) || flameFlag )
	{
		menu.createMenu(mainMenu);
		flag=true;
		flameFlag=false;
	}
	if(!switche.getState(SW2) && flag)
	{
		HAL_Delay(250);
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
	if(!switche.getState(SW5) && flag)
	{
		HAL_Delay(250);
		menu.back();
	}
	if(sw3Flag==false)
	{
		motor.disableDT1();
		lcdObj.clear();
		lcdObj.setCursor(1,0);
		lcdObj.putString("log to master");
		flash.writeNum(HEATER_STATE_ADDRESS,0);
		sw3Flag=true;
	}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==SW3_Pin)
	{
		sw3Flag=false;
		extern bool ui;
		ui=false;
		nextMenu=-1;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{

	if(pumpConPtr!=NULL)
	{
		pumpConPtr(htim);
		pumpConPtr=NULL;
	}
	else if(flamePtr!=NULL)
	{
		flamePtr(htim);
		flamePtr=NULL;
	}

}




