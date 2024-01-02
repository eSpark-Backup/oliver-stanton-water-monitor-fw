#include "main.h"
#include "mainApp.hpp"
#include "stm32f0xx_hal.h"
//#include "usb_device.h"
#include "switches.hpp"
//#include "motion.hpp"
#include "menu.h"
#include "menuList.h"
#include "motorDriver.h"
#include "flash.h"
//#include "callbacks.h"
#include "Display.h"
#include "CDC_Serial.h"
#include "controls.h"
#include "flagHandle.h"
#include "eventHandle.hpp"

//#include "usbd_cdc_if.h"



Switches switche;
BUTTON buttonStateE;			//Enter/OK button obj
BUTTON buttonStateB;			//Back button obj
BUTTON buttonStateU;			//UP button obj
BUTTON buttonStateD;			//Down button obj
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

///-------------------declared in switches.hpp-----------------------
bool sw2Flag = false;
bool sw3Flag = false;
bool sw4Flag = false;
bool sw5Flag = false;
bool sw6Flag = false;
bool sw7Flag = false;
bool timeStamped = false;
uint32_t buttonPressTime;




//bool ONflag=true;
//TIM_HandleTypeDef htim1;
//extern int nextMenu;
//#define HEATER_STATE_ADDRESS 0x0801F800

///-------------------------------------Function declarations--------------
//--------------------------------------declared in controls.h-------------


///-----------------------------------Creating Menu Items List-------------
//-------------------------------------created in  menuList.h-------------


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

	currentM = (int*)(Intro);
	currentE = sizeof(Intro) / sizeof(Intro[0]);
	HAL_Delay(2000);  		menu.displayMenu((MenuItem*)currentM, currentE);

	TIM17->CCR1 = 50;
	TIM1->SR &= ~TIM_SR_UIF;

	HAL_TIM_Base_Start_IT(&htim1);

//	int time = HAL_GetTick();
//	char strng[50];
//	int len = sprintf(strng, "Time: %d\r\n", time);
//	HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)strng, len);

//	flags.flagsInit();
}

void MainApp::loop()
{

	buttonStateE = switche.getState(EnterOK);
	buttonStateB = switche.getState(Back);
	buttonStateU = switche.getState(UP);
	buttonStateD = switche.getState(DOWN);
	if (buttonStateU.state){//sw4Flag==true){
		if(buttonStateU.period < single_press)
			menu.navigateMenu("up", (MenuItem*)currentM);
#ifdef _DEBUG_
		dispdebug->displayString("*******up button pressed.*******");
		dispdebug->displayVar(buttonStateU.state, "buttonStateU.state : ");
		dispdebug->displayVar(buttonStateU.period, "buttonStateU.period : ");
#endif
		buttonStateU = {false, 0};
	}
	if (buttonStateD.state){
		if(buttonStateD.period < single_press)
			menu.navigateMenu("down", (MenuItem*)currentM);
#ifdef _DEBUG_
		dispdebug->displayString("*******down button pressed.*******");
		dispdebug->displayVar(buttonStateD.state, "buttonStateD.state : ");
		dispdebug->displayVar(buttonStateD.period, "buttonStateD.period : ");
#endif
		buttonStateD = {false, 0};
		}
	if (buttonStateE.state){
//		if(buttonStateE.period < single_press)
			menu.navigateMenu("enter", (MenuItem*)currentM);
#ifdef _DEBUG_
		dispdebug->displayString("*******enter button pressed.*******");
		dispdebug->displayVar(buttonStateE.state, "buttonStateE.state : ");
		dispdebug->displayVar(buttonStateE.period, "buttonStateE.period : ");
#endif
		buttonStateE = {false, 0};
		}
	if (buttonStateB.state){
		if(buttonStateB.period < single_press)
			menu.navigateMenu("back", (MenuItem*)currentM);
#ifdef _DEBUG_
		dispdebug->displayString("*******back button pressed.*******");
		dispdebug->displayVar(buttonStateB.state, "buttonStateB.state : ");
		dispdebug->displayVar(buttonStateB.period, "buttonStateB.period : ");
#endif
		buttonStateB = {false, 0};
		}

	flags.update();
	Mem_Saver_ST3();
//	RO_Saver_ST1();
//	dispdebug->displayVar(flags.timeElapsed(MEM_Saver, MEM_Saver_Pause_Timeout), " MEM_Saver_Pause_Timeout: ");

//	HAL_Delay(1000);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(!timeStamped)	{
		switche.saveTime();
	if(GPIO_Pin==SW3_Pin)
	{	}
	else if(GPIO_Pin==UP_Button)		{
			UP_Flag=true;
//			switche.saveTime();
		}
	else if(GPIO_Pin==ENTER_Button)		{
			ENTER_Flag=true;
//			switche.saveTime();
		}
	else if(GPIO_Pin==BACK_Button)		{
			BACK_Flag=true;
//			switche.saveTime();
		}
	else if(GPIO_Pin==DOWN_Button)		{
			DOWN_Flag=true;
//			switche.saveTime();
		}
	}
}

