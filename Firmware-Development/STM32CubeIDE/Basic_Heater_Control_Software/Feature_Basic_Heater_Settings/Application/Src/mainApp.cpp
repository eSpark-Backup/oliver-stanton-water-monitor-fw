#include "main.h"
#include "mainApp.hpp"
#include "stm32f0xx_hal.h"
#include "switches.hpp"
#include "menu.h"
#include "menuList.h"
#include "motorDriver.h"
#include "flash.h"
#include "Display.h"
#include "CDC_Serial.h"
#include "controls.h"
#include "flagHandle.h"
#include "eventHandle.hpp"
//#include "TDS.hpp"

#include "_DEBUG_.h"

//#include "usbd_cdc_if.h"



Switches switche;
BUTTON buttonStateE;			//Enter/OK button obj
BUTTON buttonStateB;			//Back button obj
BUTTON buttonStateU;			//UP button obj
BUTTON buttonStateD;			//Down button obj

// generating struct for storing GPIO_Port and GPIO_Pins for TDS sensors
static GPIOConfig TDS1;// = {TDS1_T2_GPIO_Port, TDS1_T1_GPIO_Port, ADC_CHANNEL_10, TDS1_T2_Pin, TDS1_T1_Pin, ADC_CHANNEL_11};
static GPIOConfig TDS2;// = {TDS2_T2_GPIO_Port, TDS2_T1_GPIO_Port, ADC_CHANNEL_14, TDS2_T2_Pin, TDS2_T1_Pin, ADC_CHANNEL_15};
static GPIOConfig TDS3;// = {TDS3_T2_GPIO_Port, TDS3_T1_GPIO_Port, ADC_CHANNEL_12, TDS3_T2_Pin, TDS3_T1_Pin, ADC_CHANNEL_13};
static GPIOConfig TDS4;// = {TDS4_T2_GPIO_Port, TDS4_T1_GPIO_Port, ADC_CHANNEL_1, TDS4_T2_Pin, TDS4_T1_Pin, ADC_CHANNEL_2};
//Now declare TDS class objects for TDS sensors
extern TDS tds1; //= TDS(TDS1);
extern TDS tds2; //= TDS(TDS2);
extern TDS tds3; //= TDS(TDS3);
extern TDS tds4; //= TDS(TDS4);

extern eventHandle Buzzzer;
extern eventHandle WLED1;


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

//	TIM17->CCR1 = 50;
//	TIM1->SR &= ~TIM_SR_UIF;

//	HAL_TIM_Base_Start_IT(&htim6);

	flags.flagsInit();
	DEBUG_A("Heater Controller Initialization complete.");
}

void MainApp::loop()
{

	buttonStateE = switche.getState(EnterOK);
	buttonStateB = switche.getState(Back);
	buttonStateU = switche.getState(UP);
	buttonStateD = switche.getState(DOWN);
	if (buttonStateU.state){//sw4Flag==true){
		DEBUG_I("*******up button pressed.*******");
		DEBUG_("buttonStateU.state : %d\nbuttonStateU.period : %d", buttonStateU.state, buttonStateU.period);
		if(buttonStateU.period < single_press)
			menu.navigateMenu("up", (MenuItem*)currentM);
		buttonStateU = {false, 0};
	}
	if (buttonStateD.state){
		DEBUG_I("*******down button pressed.*******");
		DEBUG_("buttonStateD.state : %d\nbuttonStateD.period : %d", buttonStateD.state, buttonStateD.period);
		if(buttonStateD.period < single_press)
			menu.navigateMenu("down", (MenuItem*)currentM);
		buttonStateD = {false, 0};
		}
	if (buttonStateE.state){
		DEBUG_I("*******enter button pressed.*******");
		DEBUG_("buttonStateE.state : %d\nbuttonStateE.period : %d", buttonStateE.state, buttonStateE.period);
//		if(buttonStateE.period < single_press)
			menu.navigateMenu("enter", (MenuItem*)currentM);
		buttonStateE = {false, 0};
		}
	if (buttonStateB.state){
		DEBUG_I("*******back button pressed.*******");
		DEBUG_("buttonStateB.state : %d\nbuttonStateB.period : %d", buttonStateB.state, buttonStateB.period);
		if(buttonStateB.period < single_press)
			menu.navigateMenu("back", (MenuItem*)currentM);
		buttonStateB = {false, 0};
		}

	flags.update();
	Mem_Saver_ST3();
	RO_Saver_ST1();
	Buzzzer.Run();
	WLED1.Run();


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
		}
	else if(GPIO_Pin==ENTER_Button)		{
			ENTER_Flag=true;
		}
	else if(GPIO_Pin==BACK_Button)		{
			BACK_Flag=true;
		}
	else if(GPIO_Pin==DOWN_Button)		{
			DOWN_Flag=true;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim->Instance == TIM6)
	{
		DEBUG_E("%d", HAL_GetTick());
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
	DEBUG_E("Error@%s on line %d", file, line);
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
