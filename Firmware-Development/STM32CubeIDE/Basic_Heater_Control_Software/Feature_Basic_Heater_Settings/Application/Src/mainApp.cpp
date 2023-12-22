#include "main.h"
#include "mainApp.hpp"
#include "stm32f0xx_hal.h"
#include "usb_device.h"
//#include "lcd.h"
#include "switches.hpp"
#include "motion.hpp"
//#include "LED.h"
//#include <functional>
#include "menu.h"
#include "menuList.h"
#include "motorDriver.h"
#include "flash.h"
#include "callbacks.h"
#include "Display.h"
#include "CDC_Serial.h"
#include "flagHandler.h"
#include "controls.h"

//#include "usbd_cdc_if.h"



Switches switche;
//Display* disp = new CDC_Serial;
//flagHandle flags;
//Menu menu;

//motionCallback motionPtr;
//extern ADC_HandleTypeDef hadc;
//extern TIM_HandleTypeDef htim17;
//extern extiCallback extiPtr;
//extern timerCallback timerPtr;
//extern flameCallback flamePtr;
//extern pumpCallback pumpConPtr;
//LCD lcdObj;
//WLED wLED;

//MotorDriver motor;
//FlashStorage flash;
bool flag=false;
//bool flameFlag=false;
bool sw3Flag=true;
extern bool sw4Flag=true;		//up arrow button
extern bool sw2Flag=true;		//back button
extern bool sw5Flag=true;		//ENTER button
extern bool sw6Flag=true;		//down arrow button

//bool ONflag=true;
//extern TIM_HandleTypeDef htim1;
//extern int nextMenu;
//#define HEATER_STATE_ADDRESS 0x0801F800

///-------------------------------------Function declarations--------------

//bool PureFill(void);
//void Buzzer_BUZZ(uint16_t);
//void fillOpen(void);
//void fillClosed(void);
//bool fodON(void);
//bool fodRun(void);

///-----------------------------------Creating Menu Items List-------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <ParentItems>, <Items> }
//MenuItem fodOptnsOff[] = {
//    {"Press OK to commence the purification filling process", 	PureFill, fodMenu, fodOptns, 2, 1},
//};
//MenuItem fodOptns[] = {
//    {"On", 					fodON, nullptr, 	fodMenu, 2, 2},
//    {"Off", 				nullptr, fodOptnsOff, 	fodMenu, 2, 2},
//};
//MenuItem ro_saver[] = {
//    {"This",				nullptr, nullptr, 	fodMenu, 2, 2},
//    {"That",				nullptr, nullptr, 	fodMenu, 2 ,2},
//};
//MenuItem fodMenu[] = {
//    {"Fill on Demand", 		nullptr, fodOptns, mainMenu, 1, 2},
//    {"ro_saver_tds_thres",	nullptr, ro_saver, mainMenu, 1, 2},
//};
//MenuItem mainMenu[] = {
//    {"Default LLROFS Home Page", nullptr, fodMenu, nullptr, 0, 1},
//};
//MenuItem Intro[] = {
//	{"Heater Control Software", nullptr, mainMenu, nullptr, 0, 1}
//};

//extern int *currentM;// = (int*)(fodMenu);
//size_t currentE;// = sizeof(fodMenu) / sizeof(fodMenu[0]);
//extern short choiceSelect;

///-----------------------------------------------

//typedef enum{
//	Level_Sensor,
//	Pressure_Sensor,
//	Flow_Meter_1,
//	Flow_Meter_2,
//	Flow_Meter_3,
//	WLED,
//	RGB,
//	Buzzer,
//	FoD
//}sensors;
//
//typedef enum{
//	Settings,
//	Status
//}Task;





void MainApp::setup()
{
//	HAL_GPIO_WritePin(GPIOE, PWM_CNTRL_Pin,GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOE,PWM_CNTRL_Pin,GPIO_PIN_SET);
//	HAL_TIM_Base_Start_IT(&htim2);
//	HAL_TIM_Base_Stop_IT(&htim2);
//	lcdObj.start();
//	motor.enable();
//	motor.enableDT1();
//	motor.enablePump1();
//	motor.enablePump2();
//	motor.setZeroReference();

//	HAL_Delay(2000);
//	CDC_Transmit_FS((uint8_t *)"Heater Control Software \n", sizeof("Heater Control Software \n")-1);
//	HAL_Delay(5);
//	menu.BuildMenu(&mainMenu);
//	currentM = mainMenu;
////	printf("%d", currentM);

	currentM = (int*)(Intro);
	currentE = sizeof(Intro) / sizeof(Intro[0]);

	HAL_Delay(2000);  		menu.displayMenu((MenuItem*)currentM, currentE);

	TIM17->CCR1 = 50;
	TIM1->SR &= ~TIM_SR_UIF;

	HAL_TIM_Base_Start_IT(&htim1);
//	Buzzer_BUZZ();

//	int time = HAL_GetTick();
//	char strng[50];
//	int len = sprintf(strng, "Time: %d\r\n", time);
//	HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)strng, len);

//	flags.flagsInit();

}

void MainApp::loop()
{
//	Buzzer_BUZZ();

	if (sw4Flag==false){
		menu.navigateMenu("up", (MenuItem*)currentM);
//		disp->displayString("second");
		sw4Flag=true;
	}
	if (sw5Flag==false){
		menu.navigateMenu("enter", (MenuItem*)currentM);
		disp->displayVar(flags.flagStatus(Settings, FoD), "Button Press");
		sw5Flag=true;
		}
	if (sw2Flag==false){
		menu.navigateMenu("back", (MenuItem*)currentM);
//		disp->displayString("third");
		sw2Flag=true;
		}
	if (sw6Flag==false){
		menu.navigateMenu("down", (MenuItem*)currentM);
//		disp->displayString("fourth");
		sw6Flag=true;
		}
	flags.update();
//	fodRun();
//	HAL_Delay(1000);
}

//bool PureFill(){
////    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//	if(HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port ,LEVEL_SENSOR_Pin)){
//		fillOpen();
////		return false;
////		HAL_Delay(1);mainMenu
//	}
//	else{
//		fillClosed();
//	}
//	return false;
//}
//
//void fillOpen(){
//	disp->displayString("Tank Full - System not Filling");
//	flags.setFlag(Settings, FoD, false);
//	currentM = (int*)(mainMenu);
//	HAL_Delay(1);  		menu.displayMenu(mainMenu, mainMenu->num_items);
//}
//void fillClosed(){
//	disp->displayString("Starting Purification - System Filling");
////	flags.setFlag(Status, Mem_Saver, true);
////	HAL_GPIO_WritePin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin, GPIO_PIN_SET);
//	currentM = (int*)(mainMenu);
//	Buzzer_BUZZ(500);
//	HAL_Delay(50);
//	Buzzer_BUZZ(100);
//}
//bool fodON(){
//	flags.setFlag(Settings, FoD, true);
//	return true;
//}
//
//bool fodRun(){
//	if(flags.timeElapsed(Level_Sensor, 5000) && flags.flagStatus(Settings, FoD) == true && flags.flagStatus(Status, Level_Sensor) == true){
//		fillClosed();
////		HAL_Delay(1);  	CDC_Transmit_FS((u int8_t *)"turned on\n", sizeof("turned on\n")-1);
//		return false;
//	}
//	else
//		return true;
//}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==SW3_Pin)
	{
//		sw3Flag=false;
//		extern bool ui;
//		ui=false;
//		nextMenu=-1;
	}
	else if(GPIO_Pin==SW4_Pin)
		{
			sw4Flag=false;
		}
	else if(GPIO_Pin==SW5_Pin)
		{
			sw5Flag=false;
		}
	else if(GPIO_Pin==SW2_Pin)
		{
			sw2Flag=false;
		}
	else if(GPIO_Pin==SW6_Pin)
		{
			sw6Flag=false;
		}
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
//{
//
////	if(pumpConPtr!=NULL)
////	{
////		pumpConPtr(htim);
////		pumpConPtr=NULL;
////	}
////	else if(flamePtr!=NULL)
////	{
////		flamePtr(htim);
////		flamePtr=NULL;
////	}
//
//}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//    if (htim == &htim1)
//    {
//        // handle TIM1 interrupts
//    }
    if (htim == &htim1)
    {
//    	Buzzer_BUZZ();
//    	HAL_GPIO_TogglePin(LED_TOGGLE_GPIO_Port, LED_TOGGLE_Pin);
        // handle TIM17 interrupts
    }
}

