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

Switches switche;
extiCallback extiPtr;
timerCallback timerPtr;
flameCallback flamePtr;
pumpCallback pumpConPtr;
//motionCallback motionPtr;
extern ADC_HandleTypeDef hadc;
LCD lcdObj;
W_LED wLED;
Menu menu;
MotorDriver pump;
FlashStorage flash;
bool flag=false;
bool flameFlag=false;
bool sw3Flag=true;
extern TIM_HandleTypeDef htim2;
extern int nextMenu;
float Vref_PB1=0;
#define HEATER_STATE_ADDRESS 0x0801F800
const char* mainMenu[]= {"Heater: ","Frost Prev: ","Frost Pump: ",NULL};

void MainApp::setup()
{
	HAL_GPIO_WritePin(GPIOE, PWM_CNTRL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,PWM_CNTRL_Pin,GPIO_PIN_SET);
	registerCallback(timerElapsed,0);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim2);
	lcdObj.start();
	pump.enableDT1();
	startADC();
	/*Configure GPIO pin Output Level */
	pump.enablePumps();
	pump.enablePump1();
	pump.enablePump2();

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
		pump.disableDT1();
		lcdObj.clear();
		lcdObj.setCursor(1,0);
		lcdObj.putString("log to master");
		flash.writeNum(HEATER_STATE_ADDRESS,0);
		sw3Flag=true;
	}
}

void registerCallback(extiCallback ptr)
{
	extiPtr=ptr;
}

void registerCallback(timerCallback ptr,int n)
{
	if(n==0)
		timerPtr=ptr;
	else if(n==2)
		pumpConPtr=ptr;
}

void registerCallback(flameCallback ptr,int n)
{
		flamePtr=ptr;
}

void registerCallback(pumpCallback ptr)
{
	pumpConPtr=ptr;
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
	}
	if(flamePtr!=NULL)
	{
		flamePtr(htim);
		flamePtr=NULL;
	}
	if(timerPtr!=NULL)
	{
		timerPtr(htim);
	}
}

void timerElapsed(TIM_HandleTypeDef* htim)
{
	wLED.reset();
	HAL_TIM_Base_Stop_IT(htim);
}

void startADC(){
	// Start ADC Conversion
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, 10);
		Vref_PB1 = (HAL_ADC_GetValue(&hadc)) * 3.3/4096;
}
