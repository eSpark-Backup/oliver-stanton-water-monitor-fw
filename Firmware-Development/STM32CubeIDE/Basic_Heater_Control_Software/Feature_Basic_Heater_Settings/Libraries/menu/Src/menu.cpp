#include "menu.h"
#include "string.h"
#include "lcd.h"
#include "string.h"
#include "mainApp.hpp"
#include "main.h"
#include "LED.h"
#include "switches.hpp"
#include "motorDriver.h"
#include "flash.h"

Switches Switch;
MotorDriver motor;
FlashStorage flashMem;
extern LCD lcdObj;
extern W_LED wLED;
extern const char* mainMenu[];
volatile bool pumpConnectivityFlag=true;
extern bool flameFlag;
typedef enum{
	OPTION_1,
	OPTION_2,
	OPTION_3
}arrowEnum;

typedef enum{
	OFF=0,
	On=1
}state;

#define HEATER_STATE_ADDRESS 0x0801F800
#define FROST_PREV_STATE_ADDRESS 0x0801F810
#define FROST_PUMP_STATE_ADDRESS 0x0801F820

arrowEnum arrow;
int nextMenu=0;
bool ui;
bool heaterFlag=false;

const char* heaterN[]={"On","Off",NULL};
const char* frostN[]={"On","Off",NULL};
const char* frostPumpN[]={"On","Off",NULL};
extern TIM_HandleTypeDef htim2;

void Menu::createMenu(const char** menu)
{
	lcdObj.clear();
	float heatState=flashMem.readNum(HEATER_STATE_ADDRESS);
	float frostPrevState=flashMem.readNum(FROST_PREV_STATE_ADDRESS);
	float frostPumpState=flashMem.readNum(FROST_PUMP_STATE_ADDRESS);
	this->numberOfItems=0;
	int w=0;
	while(menu[this->numberOfItems] != NULL)
	{
	        this->numberOfItems++;
	}
	while(w<this->numberOfItems)
	{
		lcdObj.setCursor(w,1);
		lcdObj.putString(menu[w]);
		if(nextMenu==0 && w==0 && heatState==OFF)
			lcdObj.putString("Off");
		else if(nextMenu==0 && w==0 && heatState==On)
			lcdObj.putString("On");
		else if(nextMenu==0 && w==1 && frostPrevState==OFF)
			lcdObj.putString("Off");
		else if(nextMenu==0 && w==1 && frostPrevState==On)
			lcdObj.putString("On");
		else if(nextMenu==0 && w==2 && frostPumpState==OFF)
			lcdObj.putString("Off");
		else if(nextMenu==0 && w==2 && frostPumpState==On)
			lcdObj.putString("On");
		w++;
	}
	if((int)arrow==numberOfItems)
		arrow=(arrowEnum)(numberOfItems-1);
	displayArrow();
}

void Menu::arrowDown()
{
	if(arrow==(arrowEnum)(this->numberOfItems-1))
		arrow=OPTION_1;
	else
		arrow=(arrowEnum)(arrow+1);
	displayArrow();
}

void Menu::arrowUp()
{
	if(arrow==0)
		arrow=(arrowEnum)(this->numberOfItems-1);
	else
		arrow=(arrowEnum)(arrow-1);
	displayArrow();
}

void Menu::displayArrow()
{
	for(int i=0;i<this->numberOfItems+1;i++)
	{
		lcdObj.setCursor(i,0);
		lcdObj.putString(" ");
	}
	lcdObj.setCursor(arrow,0);
	lcdObj.putString(">");
}

void Menu::next()
{
	nextMenu++;
	extern bool sw3Flag;
	if(nextMenu==1)
	{
		switch(arrow)
		{
		case OPTION_1:
			createMenu(heaterN);
			heaterFlag=true;
			break;
		case OPTION_2:
			createMenu(frostN);
			heaterFlag=false;
			break;
		case OPTION_3:
			createMenu(frostPumpN);
			heaterFlag=false;
			break;
		default:
			lcdObj.clear();
			heaterFlag=false;
			break;
		}
	}
	else if(nextMenu==0){
		createMenu(mainMenu);
	}
	else if(nextMenu==2 && heaterFlag)
		{
			switch(arrow)
			{
				case OPTION_1:
				lcdObj.clear();
				ui=true;
				while(ui && sw3Flag)
				{
					if(!Switch.getState(SW5))
					{
						HAL_Delay(250);
						back();
						ui=false;
					}
					if(!Switch.getState(SW2))
					{
						motor.enableDT1();
						pumpConnection();
						flashMem.writeNum(HEATER_STATE_ADDRESS,On);
						if(ui)
						{
							registerCallback(std::bind(&MotorDriver::pumpConnectivityCallback,motor,std::placeholders::_1));
							HAL_TIM_Base_Start_IT(&htim2);
							while(pumpConnectivityFlag==true && sw3Flag)
							{
								lcdObj.setCursor(3,4);
								lcdObj.putString("Preheating");
							}
							pumpConnectivityFlag=true;
							lcdObj.clear();
							while(sw3Flag && ui)
							{
								lcdObj.setCursor(0,4);
								lcdObj.putString("Heater On");
								if(motor.getCurrent()<0.5)
									pumpConnection();
								else if(!Switch.getState(SW5))
								{
									back();
									ui=false;}
								}
							}
						}
				}

				break;
				case OPTION_2:
					motor.disableDT1();
					lcdObj.clear();
					lcdObj.setCursor(1,4);
					lcdObj.putString("Flame Icon");
					lcdObj.setCursor(2,3);
					lcdObj.putString("Crossed Out");
					flashMem.writeNum(HEATER_STATE_ADDRESS,OFF);
					registerCallback(std::bind(&Menu::flameIconCallback,this,std::placeholders::_1));
					HAL_TIM_Base_Start_IT(&htim2);
					break;
				default:
					lcdObj.clear();
					break;
			}
		}
		else
			nextMenu--;
}

void Menu::back(){
	if(nextMenu>0)
	{
		nextMenu=nextMenu-1;
		nextMenu--;
		this->next();
	}
	else{
		nextMenu--;
		this->next();
	}
}

void Menu::pumpConnection()
{
	extern bool sw3Flag;
	float pumpC=motor.getCurrent();
	lcdObj.clear();
	while(pumpC<0.5 && sw3Flag && ui)
	{
		pumpC=motor.getCurrent();
		lcdObj.setCursor(0,4);
		lcdObj.putString("Heater On");
		lcdObj.setCursor(1,0);
		lcdObj.putString("Water not heatin");
		if(!Switch.getState(SW5))
		{
			HAL_Delay(250);
			lcdObj.clear();
			back();
			ui=false;
		}
	}
}

void Menu::flameIconCallback(TIM_HandleTypeDef* htim)
{
	flameFlag=true;
	nextMenu=0;
	HAL_TIM_Base_Stop_IT(htim);
}
