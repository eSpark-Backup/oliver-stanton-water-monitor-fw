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
timerCallback flamePtr;
timerCallback pumpConPtr;
motionCallback motionPtr;
extern ADC_HandleTypeDef hadc;
//flameIconCb flamePtr;
LCD lcdObj;
W_LED wLED;
Menu menu;
bool flag=false;
bool flameFlag=false;
bool SW3flag=true;
extern bool pumpConnectivityFlag;
extern TIM_HandleTypeDef htim2;
extern int Next;
double voltage  = 0;
double current = 0;
double sensitivity = 0.2 ; // volts/Amp
float Vref_PB1=0;
uint32_t adc_val;

const char* mainMenu[]= {"Heater: On","Frost Prev: On","Frost Pump: Off",NULL};

uint8_t customChar[] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

uint8_t customChar1[] = {
  0b11100,
  0b11110,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

uint8_t customChar2[] = {
  0b00111,
  0b01111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

uint8_t customChar3[] = {
		0b00001,
		0b00011,
		0b00111,
		0b01111,
		0b11110,
		0b11100,
		0b11000,
		0b10000
};

uint8_t customChar4[] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00001,
  0b00011
};

uint8_t customChar5[]={
   0b00000,
   0b00000,
   0b00100,
   0b01000,
   0b01110,
   0b11111,
   0b11111,
   0b11111
};
uint8_t customChar6[]={
		 0b00100,
		  0b01110,
		  0b11111,
		  0b11111,
		  0b01110,
		  0b00100,
		  0b00000,
		  0b00000
};
uint8_t customChar7[]={
		0b00000,
		0b00000,
		0b00000,
		0b00100,
		0b01010,
		0b10001,
		0b11011,
		0b01110
};



void MainApp::setup()
{
	HAL_GPIO_WritePin(GPIOE, PWM_CNTRL_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,PWM_CNTRL_Pin,GPIO_PIN_SET);
	registerCallback(timerElapsed,0);
	registerCallback(WLEDCallback);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim2);
	lcdObj.start();
	HAL_GPIO_WritePin(IOC9_GPIO_Port,IOC9_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, GPIO_PIN_SET);
	  // Start ADC Conversion
	  HAL_ADC_Start(&hadc);
	  HAL_ADC_PollForConversion(&hadc, 10);
	  adc_val = HAL_ADC_GetValue(&hadc); // get the adc value
	  Vref_PB1 = adc_val * 3.3/4096;
	  HAL_GPIO_WritePin(Enable_GPIO_Port, Enable_Pin, GPIO_PIN_SET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, GPIO_PIN_RESET);

	lcdObj.defineChar(0,customChar);
	lcdObj.defineChar(1,customChar1);
	lcdObj.defineChar(2,customChar2);
	lcdObj.defineChar(3,customChar3);
	lcdObj.defineChar(4,customChar4);
	lcdObj.defineChar(5,customChar5);
	lcdObj.defineChar(6,customChar7);


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
		extern int Next;
		Next++;
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
	if(SW3flag==false){
		HAL_GPIO_WritePin(IOC9_GPIO_Port,IOC9_Pin,GPIO_PIN_RESET);
		lcdObj.clear();
		lcdObj.setCursor(1,0);
		lcdObj.putString("log to master");
		SW3flag=true;
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
	else if(n==1)
		flamePtr=ptr;
	else if(n==2)
		pumpConPtr=ptr;
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
	if(GPIO_Pin==SW3_Pin){
		SW3flag=false;
		extern bool ui;

		ui=false;
		Next=-1;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(pumpConPtr!=NULL){
		pumpConPtr(htim);
	}
	if(flamePtr!=NULL){
		flamePtr(htim);
		flamePtr=NULL;
	}
	if(timerPtr!=NULL)
		{
			timerPtr(htim);
		}
}

void WLEDCallback(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim)
{
		SW3flag=false;
		wLED.set();
    	HAL_TIM_Base_Start_IT(htim);
}

void timerElapsed(TIM_HandleTypeDef* htim)
{
	wLED.reset();
	HAL_TIM_Base_Stop_IT(htim);
}

void flameIconCallback(TIM_HandleTypeDef* htim)
{
	flameFlag=true;
	Next=0;
	HAL_TIM_Base_Stop_IT(htim);
}

float getCurrent(){
	HAL_ADC_Start(&hadc);
			  // Poll ADC1 Peripheral & TimeOut = 1mSec
	HAL_ADC_PollForConversion(&hadc, 10);

	adc_val = HAL_ADC_GetValue(&hadc); // get the adc value

	voltage = adc_val * 3.3/4096; //reference voltage (volts) when 0 current is flowing

	current = (Vref_PB1-voltage)/sensitivity;

	return current;
}

void pumpConnectivityCallback(TIM_HandleTypeDef* htim)
{
	pumpConnectivityFlag=false;
	HAL_TIM_Base_Stop_IT(htim);
}


