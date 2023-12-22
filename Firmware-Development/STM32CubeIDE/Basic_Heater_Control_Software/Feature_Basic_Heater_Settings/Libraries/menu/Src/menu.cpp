#include "menu.h"
#include "controls.h"

//#include "main.h"
//#include "LED.h"
//#include "switches.hpp"
//#include "motorDriver.h"
//#include "flash.h"
//#include "callbacks.h"
//#include "usbd_cdc_if.h"


//const char* arrowEnumToString(arrowEnum value);

//extern struct MenuItem *currentM;

//#define HEATER_STATE_ADDRESS 0x0801F800
//#define FROST_PREV_STATE_ADDRESS 0x0801F810
//#define FROST_PUMP_STATE_ADDRESS 0x0801F820

//const char* stateS[]={"On\n","Off\n",NULL};
//const char* heaterN[]={"On","Off",NULL};
//const char* frostN[]={"On","Off",NULL};
//const char* frostPumpN[]={"On","Off",NULL};
//extern TIM_HandleTypeDef htim2;
//Display* display = new CDC_Serial;



//-------------------------------Function Definitions---------------------

int *currentM;
short choiceSelect = 0;

void Menu::displayMenu(const MenuItem* menu, int elements) {

    for(uint8_t count = 0; count < elements; count++){
    	disp->displayString(menu[count].label);	}
}

void Menu::navigateMenu(const char* optn, MenuItem* currentMenu){
	short num_parent = currentMenu->num_parentmenuItems;
//	short num_childs = currentMenu->num_childmenus;
	short num_items = currentMenu->num_items;
	short c = choiceSelect ;

	if(strcmp(optn, "up") == 0 && num_items > 1){
		if(c==num_items){
			c=1;			}
		else{
			c += 1;		}
		disp->displayChoice(c);
		choiceSelect = c;
	}
	if(strcmp(optn, "down") == 0 && num_items > 1){
		if(c<=1){
			c=num_items;	}
		else if (c>1){
			c -= 1;		}
		disp->displayChoice(c);
		choiceSelect = c;
	}
	if(strcmp(optn, "back") == 0){
		currentMenu = (MenuItem*)currentMenu->prev;
		if(num_parent!=0){
		displayMenu(currentMenu, num_parent);
		currentM = (int*)currentMenu;	}
	}
	if(strcmp(optn, "enter") == 0){
//		displayMenu(currentMenu, num_items);
			if(num_items>1 && c == 0){
				disp->displayString("Choose one of the options.");
			}
			else if(num_items>1 && c != 0){
//				displayMenu(currentMenu[c-1], num_items);
				if(currentMenu[c-1].next != nullptr){
					currentMenu = (MenuItem*)currentMenu[c-1].next;
//					displayMenu(currentMenu, 2);
					num_items = currentMenu->num_items;
					displayMenu(currentMenu, num_items);
					currentM = (int*)currentMenu;	}
				else if (currentMenu->DoWork!= nullptr){
					if(currentMenu[0].DoWork() != true){
						return;							}
					currentMenu = (MenuItem*)currentMenu->prev;
					displayMenu(currentMenu, num_parent);
					currentM = (int*)currentMenu;
				}
//				if(currentMenu->DoWork!= nullptr){
//					currentMenu[0].DoWork();
//				}
				choiceSelect = 0;						}
			else if (currentMenu->DoWork!= nullptr){
				if(currentMenu[0].DoWork() != true){
					return;							}
				currentMenu = (MenuItem*)currentMenu->prev;
				displayMenu(currentMenu, num_parent);
				currentM = (int*)currentMenu;
			}
			else{
//				displayMenu(currentMenu, num_items);
				currentMenu = (MenuItem*)currentMenu->next;
				num_items = currentMenu->num_items;
				displayMenu(currentMenu, num_items);
				currentM = (int*)currentMenu;
			}
	}
}





//--------------------------------------

//void Menu::createMenu(const char** menu)
//{
//	lcdObj.clear();
//
//	float heatState=flashMem.readNum(HEATER_STATE_ADDRESS);
//	float frostPrevState=flashMem.readNum(FROST_PREV_STATE_ADDRESS);
//	float frostPumpState=flashMem.readNum(FROST_PUMP_STATE_ADDRESS);
//	this->numberOfItems=0;
//	int w=0;
//	while(menu[this->numberOfItems] != NULL)
//	{
//	        this->numberOfItems++;
//	}
//	while(w<this->numberOfItems)
//	{
//		lcdObj.setCursor(w,1);
//		lcdObj.putString(menu[w]);
////		CDC_Transmit_FS((uint8_t*)menu[w], strlen(menu[w]));
////		HAL_Delay(5);
//		if(nextMenu==0 && w==0 && heatState==OFF)
////			{CDC_Transmit_FS((uint8_t*)stateS[1], strlen(stateS[1]));
////			HAL_Delay(10);	}
//			lcdObj.putString("Off");
//		else if(nextMenu==0 && w==0 && heatState==On)
////			{CDC_Transmit_FS((uint8_t*)stateS[0], strlen(stateS[0]));
////			HAL_Delay(10);	}
//			lcdObj.putString("On");
//		else if(nextMenu==0 && w==1 && frostPrevState==OFF)
////			{CDC_Transmit_FS((uint8_t*)stateS[1], strlen(stateS[1]));
////			HAL_Delay(10);	}
//			lcdObj.putString("Off");
//		else if(nextMenu==0 && w==1 && frostPrevState==On)
////			{CDC_Transmit_FS((uint8_t*)stateS[0], strlen(stateS[0]));
////			HAL_Delay(10);	}
//			lcdObj.putString("On");
//		else if(nextMenu==0 && w==2 && frostPumpState==OFF)
////			{CDC_Transmit_FS((uint8_t*)stateS[1], strlen(stateS[1]));
////			HAL_Delay(10);	}
//			lcdObj.putString("Off");
//		else if(nextMenu==0 && w==2 && frostPumpState==On)
////			{CDC_Transmit_FS((uint8_t*)stateS[0], strlen(stateS[0]));
////			HAL_Delay(10);	}
//			lcdObj.putString("On");
//
//		w++;
//	}
//	if((int)arrow==numberOfItems)
//		arrow=(arrowEnum)(numberOfItems-1);
//	displayArrow();
//}
//
////const char* debug[]={"option1", "option2", "option3", "option4"};

//void Menu::arrowDown()
//{
//	if(arrow==(arrowEnum)(this->numberOfItems)){
//		arrow=OPTION_1;
////		CDC_Transmit_FS((uint8_t*)debug[0], 7);
//	}
//	else{
//		arrow=(arrowEnum)(arrow+1);}
////		CDC_Transmit_FS((uint8_t*)debug[1], 7);	}
//	displayArrow();
//}
//
//void Menu::arrowUp()
//{
//	if(arrow==1)
//		arrow=(arrowEnum)(this->numberOfItems);
////	CDC_Transmit_FS((uint8_t*)debug[2], 7);}
//	else
//		arrow=(arrowEnum)(arrow-1);
////	CDC_Transmit_FS((uint8_t*)debug[3], 7);
////}
//	displayArrow();
//}
//
//void Menu::displayArrow()
//{
//	for(int i=0;i<this->numberOfItems+1;i++)
//	{
//		lcdObj.setCursor(i,0);
//		lcdObj.putString(" ");
//	}
//	lcdObj.setCursor(arrow,0);
//	lcdObj.putString(">");
////	CDC_Transmit_FS((uint8_t*)sizeof(arrow), 1);
////	char intString[2];
////	sprintf(intString, "%d", arrow);
////	CDC_Transmit_FS((uint8_t*)intString, strlen(intString));
//}
//
//
//
//
//void Menu::next()
//{
//	nextMenu++;
//	extern bool sw3Flag;
//	if(nextMenu==1)
//	{
//		switch(arrow)
//		{
//		case OPTION_1:
//			createMenu(heaterN);
//			heaterFlag=true;
//			break;
//		case OPTION_2:
//			createMenu(frostN);
//			heaterFlag=false;
//			break;
//		case OPTION_3:
//			createMenu(frostPumpN);
//			heaterFlag=false;
//			break;
//		default:
//			lcdObj.clear();
//			heaterFlag=false;
//			break;
//		}
//	}
//	else if(nextMenu==0){
//		createMenu(mainMenu);
//	}
//	else if(nextMenu==2 && heaterFlag)
//		{
//			switch(arrow)
//			{
//				case OPTION_1:
//				lcdObj.clear();
//				ui=true;
//				while(ui && sw3Flag)
//				{
//					if(!Switch.getState(SW5))
//					{
//						HAL_Delay(250);
//						back();
//						ui=false;
//					}
//					if(!Switch.getState(SW2))
//					{
//						motor.enableDT1();
//						pumpConnection();
//						flashMem.writeNum(HEATER_STATE_ADDRESS,On);
//						if(ui)
//						{
//							registerCallback(std::bind(&MotorDriver::pumpConnectivityCallback,&motor,std::placeholders::_1));
//							HAL_TIM_Base_Start_IT(&htim2);
//							while((motor.getFlag()==true) && sw3Flag)
//							{
//								lcdObj.setCursor(3,4);
//								lcdObj.putString("Preheating");
//							}
//							motor.setFlag(true);
//							lcdObj.clear();
//							while(sw3Flag && ui)
//							{
//								lcdObj.setCursor(0,4);
//								lcdObj.putString("Heater On");
//								if(motor.getCurrent()<0.5)
//									pumpConnection();
//								else if(!Switch.getState(SW5))
//								{
//									back();
//									ui=false;}
//								}
//							}
//						}
//				}
//
//				break;
//				case OPTION_2:
//					motor.disableDT1();
//					lcdObj.clear();
//					lcdObj.setCursor(1,4);
//					lcdObj.putString("Flame Icon");
//					lcdObj.setCursor(2,3);
//					lcdObj.putString("Crossed Out");
//					flashMem.writeNum(HEATER_STATE_ADDRESS,OFF);
//					registerCallback(std::bind(&Menu::flameIconCallback,this,std::placeholders::_1));
//					HAL_TIM_Base_Start_IT(&htim2);
//					break;
//				default:
//					lcdObj.clear();
//					break;
//			}
//		}
//		else
//			nextMenu--;
//}
//
//void Menu::back(){
//	if(nextMenu>0)
//	{
//		nextMenu=nextMenu-1;
//		nextMenu--;
//		this->next();
//	}
//	else{
//		nextMenu--;
//		this->next();
//	}
//}
//
//void Menu::pumpConnection()
//{
//	extern bool sw3Flag;
//	float pumpC=motor.getCurrent();
//	lcdObj.clear();
//	while(pumpC<0.5 && sw3Flag && ui)
//	{
//		pumpC=motor.getCurrent();
//		lcdObj.setCursor(0,4);
//		lcdObj.putString("Heater On");
//		lcdObj.setCursor(1,0);
//		lcdObj.putString("Water not heatin");
//		lcdObj.setCursor(3,2);
//		lcdObj.putInt(pumpC);
//		if(!Switch.getState(SW5))
//		{
//			HAL_Delay(250);
//			lcdObj.clear();
//			back();
//			ui=false;
//		}
//	}
//}
//
//void Menu::flameIconCallback(TIM_HandleTypeDef* htim)
//{
//	flameFlag=true;
//	nextMenu=0;
//	HAL_TIM_Base_Stop_IT(htim);
//}
