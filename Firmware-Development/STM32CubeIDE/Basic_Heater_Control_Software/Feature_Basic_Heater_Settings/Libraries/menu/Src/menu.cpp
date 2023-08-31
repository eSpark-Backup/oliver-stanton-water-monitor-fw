#include "menu.h"
#include "string.h"
#include "lcd.h"
#include "string.h"
#include "mainApp.hpp"
#include "main.h"
#include "LED.h"
#include "switches.hpp"

Switches Switch;
extern LCD lcdObj;
extern W_LED wLED;
extern const char* mainMenu[];
volatile bool pumpConnectivityFlag=true;
static int Arrow=0;
int nextMenu=0;
bool ui;

const char* heaterN[]={"On","Off",NULL};
const char* frostN[]={"On","Off",NULL};
const char* frostPumpN[]={"On","Off",NULL};
extern TIM_HandleTypeDef htim2;

void Menu::createMenu(const char** menu)
{
	lcdObj.clear();

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
		w++;
	}
	if(Arrow==numberOfItems)
	Arrow=numberOfItems-1;
	displayArrow();
}

void Menu::arrowDown()
{
	if(Arrow==(this->numberOfItems)-1)
		Arrow=0;
	else
		Arrow++;
	displayArrow();
}

void Menu::arrowUp()
{
	if(Arrow==0)
		Arrow=(this->numberOfItems)-1;
	else
		Arrow--;
	displayArrow();
}

void Menu::displayArrow()
{
	for(int i=0;i<this->numberOfItems+1;i++)
	{
		lcdObj.setCursor(i,0);
		lcdObj.putString(" ");
	}
	lcdObj.setCursor(Arrow,0);
	lcdObj.putString(">");
}

void Menu::next()
{
	extern bool sw3Flag;
	if(Arrow==0){
	if(nextMenu==2)
	{
		switch(Arrow){
			case 1:
				printFlame();
				registerCallback(flameIconCallback,1);
				HAL_TIM_Base_Start_IT(&htim2);
				break;
			case 0:
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
						HAL_GPIO_WritePin(IOC9_GPIO_Port,IOC9_Pin,GPIO_PIN_SET);
						pumpConnection();
						if(ui)
						{
							registerCallback(pumpConnectivityCallback,2);
							HAL_TIM_Base_Start_IT(&htim2);
							while(pumpConnectivityFlag==true && sw3Flag)
							{
								lcdObj.setCursor(3,4);
								lcdObj.putString("Preheating");
							}

							lcdObj.clear();
							while(sw3Flag)
							{
							lcdObj.setCursor(0,4);
							lcdObj.putString("Heater On");
							if(getCurrent()<0.5)
								pumpConnection();
							else if(!Switch.getState(SW5)){
								back();
								ui=false;}
							}
						}
					}
				}
				break;
			default:
				lcdObj.clear();
				break;
		}
			}
	}
	if(nextMenu==1)
	{
		switch(Arrow)
		{
		case 0:
			createMenu(heaterN);
			break;
		case 1:
			createMenu(frostN);
			break;
		case 2:
			createMenu(frostPumpN);
			break;
		default:
			lcdObj.clear();
			break;
		}
	}
	else if(nextMenu==0){
		createMenu(mainMenu);
	}

}

void Menu::back(){
	if(nextMenu>0)
	{
		nextMenu=nextMenu-1;
		this->next();
	}
	else{
		this->next();
	}
}

void Menu::printFlame(){
	lcdObj.clear();
	lcdObj.fireIcon(3,7,6);
	lcdObj.fireIcon(3,9,6);
	lcdObj.fireIcon(2,6,6);
	lcdObj.fireIcon(2,10,6);
	lcdObj.fireIcon(1,7,6);
	lcdObj.fireIcon(1,9,6);
	lcdObj.fireIcon(0,8,6);
	lcdObj.fireIcon(3,6,3);
	lcdObj.fireIcon(2,7,3);
	lcdObj.fireIcon(1,8,3);
	lcdObj.fireIcon(0,9,3);
}

void Menu::pumpConnection()
{
	extern bool sw3Flag;
	float pumpC=getCurrent();
	lcdObj.clear();
	while(pumpC<0.5 && sw3Flag && ui)
	{
		pumpC=getCurrent();
		lcdObj.setCursor(0,4);
		lcdObj.putString("Heater On");
		lcdObj.setCursor(1,0);
		lcdObj.putString("Water not heatin");
//		lcdObj.setCursor(2,8);
//		char buffer[11];
//		sprintf(buffer, "%.2f", pumpC);
//		lcdObj.putString(buffer);
		if(!Switch.getState(SW5))
		{
			HAL_Delay(250);
			lcdObj.clear();
			back();
			ui=false;
		}
	}
}
