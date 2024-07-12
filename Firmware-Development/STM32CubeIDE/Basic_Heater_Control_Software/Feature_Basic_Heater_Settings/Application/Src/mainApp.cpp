#include "main.h"
#include "mainApp.hpp"
#include "stm32f0xx_hal.h"
#include "switches.hpp"
#include "menu.h"
#include "menuList.h"
#include "motorDriver.h"
#include "Battery.hpp"
//#include "flash.h"
#include "Display.h"
#include "CDC_Serial.h"
#include "controls.h"
#include "flagHandle.h"
#include "eventHandle.hpp"
//#include "TDS.hpp"

#include "_DEBUG_.h"

//#include "usbd_cdc_if.h"



//Switches switche;
Switches ButtonE;			//Enter/OK Button obj
Switches ButtonB;			//Back Button obj
Switches ButtonU;			//UP Button obj
Switches ButtonD;			//Down Button obj

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

extern EventHandle Buzzzer;
extern EventHandle WLED1;


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




void MainApp::setup()
{
	currentM = (int*)(intro);
	HAL_Delay(2000);  		menu.displayMenu((MenuItem*)currentM, (sizeof(intro) / sizeof(intro[0])));

	Flags.flagsInit();				//initiate flags from flash
	DEBUG_LOG(menu, "Heater Controller Initialization complete.");
}

void MainApp::loop()
{

	ButtonE.getState(ENTER_OK);
	ButtonB.getState(BACK);
	ButtonU.getState(UP);
	ButtonD.getState(DOWN);
	if (ButtonU.state()){
		DEBUG_LOG(menu, "*******up Button pressed.*******");
		DEBUG_LOG(menu, "ButtonU.state : %d\nButtonU.period : %d", ButtonU.state(), ButtonU.period());
		if(ButtonU.period() < SINGLE_PRESS)
			menu.navigateMenu("up", (MenuItem*)currentM);
		ButtonU.reset();
	}
	if (ButtonD.state()){
		DEBUG_LOG(menu, "*******down Button pressed.*******");
		DEBUG_LOG(menu, "ButtonD.state : %d\nButtonD.period : %d", ButtonD.state(), ButtonD.period());
		if(ButtonD.period() < SINGLE_PRESS)
			menu.navigateMenu("down", (MenuItem*)currentM);
		ButtonD.reset();
		}
	if (ButtonE.state()){
		DEBUG_LOG(menu, "*******enter Button pressed.*******");
		DEBUG_LOG(menu, "ButtonE.state : %d\nButtonE.period : %d", ButtonE.state(), ButtonE.period());
//		if(ButtonE.period() < SINGLE_PRESS)
			menu.navigateMenu("enter", (MenuItem*)currentM);
			ButtonE.reset();
		}
	if (ButtonB.state()){
		DEBUG_LOG(menu, "*******back Button pressed.*******");
		DEBUG_LOG(menu, "ButtonB.state : %d\nButtonB.period : %d", ButtonB.state(), ButtonB.period());
		Battery.getMonitor().isBackButtonPressed = true;
		if(ButtonB.period() < SINGLE_PRESS)
			menu.navigateMenu("back", (MenuItem*)currentM);
		ButtonB.reset();
		}

	Flags.update();
	memSaverST3();
	roSaverST1();
	Buzzzer.Run();
	WLED1.Run();
	Battery.powerManager();

//	HAL_Delay(1000);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(!timeStamped)	{
		saveTime();
	if(GPIO_Pin==SW3_Pin)
	{	}
	else if(GPIO_Pin==UP_BUTTON)		{
			UP_BUTTON_FLAG=true;
		}
	else if(GPIO_Pin==ENTER_BUTTON)		{
			ENTER_BUTTON_FLAG=true;
		}
	else if(GPIO_Pin==BACK_BUTTON)		{
			BACK_BUTTON_FLAG=true;
		}
	else if(GPIO_Pin==DOWN_BUTTON)		{
			DOWN_BUTTON_FLAG=true;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim->Instance == TIM6)
	{
		DEBUG_RTT(LOG_ERROR, "%d", HAL_GetTick());
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
