#include "main.h"
#include "mainApp.hpp"
#include "stm32f0xx_hal.h"
#include "switches.hpp"
#include "menu.h"
#include "menu_01.h"
#include "menuList_01.h"
#include "menuList_02.h"
#include "menuList_03.h"
#include "menuList_04.h"
#include "menuList_05.h"
#include "menuList_07.h"
#include "pdsTdsMenuList.h"
#include "tdsMenuList_08.h"
#include "LL_TT_MenuList07.h"
#include "LL_TCHS_MenuList.h"
#include "motorDriver.h"
#include "Battery.hpp"
//#include "flash.h"
#include "Display.h"
#include "CDC_Serial.h"
#include "controlsequence.h"
#include "flagHandle.h"
#include "eventHandle.hpp"
#include "Buzzer.hpp"
#include "LED.h"
//#include "TDS.hpp"

#include "_DEBUG_.h"

//#include "usbd_cdc_if.h"



//Switches switche;
Switches ButtonE;			//Enter/OK Button obj
Switches ButtonB;			//Back Button obj
Switches ButtonU;			//UP Button obj
Switches ButtonD;			//Down Button obj
Switches ButtonR;			//RODI Button obj
Switches ButtonDel;			//Del Button obj

// generating struct for storing GPIO_Port and GPIO_Pins for TDS sensors
//GPIOConfig TDS1;// = {TDS1_T2_GPIO_Port, TDS1_T1_GPIO_Port, ADC_CHANNEL_10, TDS1_T2_Pin, TDS1_T1_Pin, ADC_CHANNEL_11};
//GPIOConfig TDS2;// = {TDS2_T2_GPIO_Port, TDS2_T1_GPIO_Port, ADC_CHANNEL_14, TDS2_T2_Pin, TDS2_T1_Pin, ADC_CHANNEL_15};
//GPIOConfig TDS3;// = {TDS3_T2_GPIO_Port, TDS3_T1_GPIO_Port, ADC_CHANNEL_12, TDS3_T2_Pin, TDS3_T1_Pin, ADC_CHANNEL_13};
//GPIOConfig TDS4;// = {TDS4_T2_GPIO_Port, TDS4_T1_GPIO_Port, ADC_CHANNEL_1, TDS4_T2_Pin, TDS4_T1_Pin, ADC_CHANNEL_2};
//Now declare TDS class objects for TDS sensors
extern TDS tds1; //= TDS(TDS1);
extern TDS tds2; //= TDS(TDS2);
extern TDS tds3; //= TDS(TDS3);
extern TDS tds4; //= TDS(TDS4);

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
MenuItem *previousMenuItem = nullptr;
bool didWork = false;

volatile uint32_t waste_pulse_count = 0;
volatile uint32_t wasteFlow_rate_Lhr = 0;
volatile uint32_t pure_pulse_count = 0;
volatile uint32_t pureFlow_rate_Lhr = 0;

//bool ONflag=true;
//TIM_HandleTypeDef htim1;
//extern int nextMenu;
//#define HEATER_STATE_ADDRESS 0x0801F800




void MainApp::setup()
{
	// currentM = &remote_TT_intro07;
	currentM = &heaterIntro;
	HAL_Delay(2000);  		
	menu.displayMenu(currentM, heaterIntro.Items[0].num_items);

	Flags.flagsInit();				//initiate flags from flash
	DEBUG_LOG(LOG_INFO, "Heater Controller Initialization Complete");

	WLED1->setupInit();
	// controlSetup();
}

void MainApp::loop()
{
	didWork = invokeMenu();
	controlPeripherals();
	menu.navigateMenu(currentM, didWork);	
	Buzzzer->Run();
	WLED1->Run();
	Battery.powerManager();
}

bool MainApp::invokeMenu()
{
	getButtonStates();

	MenuItem *currentMenuItem = currentM;

	if (currentMenuItem->oneTimeSetup != nullptr && currentMenuItem != previousMenuItem)
	{
		currentMenuItem->oneTimeSetup();
		previousMenuItem = currentMenuItem;
	}

	if (currentMenuItem->loopDoWork != nullptr)
		return currentMenuItem->loopDoWork(choiceSelect, currentM);
	else
		return false;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==DEL_BUTTON && !ButtonDel.timeStamped)
	{
		DEL_BUTTON_FLAG=true;
		ButtonDel.saveTime();
		DEBUG_LOG(LOG_INFO, "--DEL--");
	}
	else if(GPIO_Pin==RODI_BUTTON && !ButtonR.timeStamped)
	{
		RODI_BUTTON_FLAG=true;
		ButtonR.saveTime();
		DEBUG_LOG(LOG_INFO, "--RODI--");
	}
	else if(GPIO_Pin==UP_BUTTON && !ButtonU.timeStamped)
	{
		UP_BUTTON_FLAG=true;
		ButtonU.saveTime();
		DEBUG_LOG(LOG_INFO, "--UP--");
	}
	else if(GPIO_Pin==ENTER_BUTTON && !ButtonE.timeStamped)
	{
		ENTER_BUTTON_FLAG=true;
		ButtonE.saveTime();
		DEBUG_LOG(LOG_INFO, "--ENTER--");
	}
	else if(GPIO_Pin==BACK_BUTTON && !ButtonB.timeStamped)
	{
		BACK_BUTTON_FLAG=true;
		ButtonB.saveTime();
		DEBUG_LOG(LOG_INFO, "--BACK--");
	}
	else if(GPIO_Pin==DOWN_BUTTON && !ButtonD.timeStamped)
	{
		DOWN_BUTTON_FLAG=true;
		ButtonD.saveTime();
		DEBUG_LOG(LOG_INFO, "--DOWN--");
	}
	if (GPIO_Pin == PURE_FLOW_DT4_Pin)
    {
        pure_pulse_count++; // Increment on each pulse
    }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	DEBUG_RTT(LOG_ERROR, "Error@%s on line %d", file, line);
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
