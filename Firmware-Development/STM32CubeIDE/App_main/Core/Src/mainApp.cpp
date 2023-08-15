#include "main.h"
#include "mainApp.hpp"
#include "usb_device.h"
#include "stm32f0xx_hal.h"
#include "lcd.h"
#include "switches.hpp"
#include "main.h"

Switches switche;
extiCallback extiPtr;
timerCallback timerPtr;
motionCallback motionPtr;
Lcd_HandleTypeDef lcd;
extern TIM_HandleTypeDef htim2;
bool motionWakeUp=true;
bool motionWelcome=true;
bool motionFlag=false;

void MainApp::setup(){
	switche.setState(GPIOE, PWM_CNTRL_Pin,GPIO_PIN_SET);
	switche.setState(GPIOE,PWM_CNTRL_Pin,GPIO_PIN_SET);
	registerCallback(timerElapsed);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim2);
}

void MainApp::loop(){
	if(!switche.getState(SW2_GPIO_Port, SW2_Pin))
	{
			  lcdSequence();
	}
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	if(motionFlag){
		if(motionWakeUp==true){
				lcdSequence();
			}
		if(motionWelcome==true)
			{
				switche.setState(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_SET);
				HAL_TIM_Base_Start_IT(&htim2);
				lcdSequence(motionWelcome);
			}
	}
}

void lcdSequence()
{
	Lcd_PortType ports[] = {D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port};
	Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
	lcd = Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
	Lcd_string(&lcd,"qwertyuiopasdfghABCD");
	Lcd_cursor(&lcd,1,0);
	Lcd_string(&lcd,"QWERTYUIOPASDFGH");
	Lcd_cursor(&lcd,2,0);
	Lcd_string(&lcd,"qwertyuiopasdfgh");
	Lcd_cursor(&lcd,3,0);
	Lcd_string(&lcd,"QWERTYUIOPASDFGH");
}

void lcdSequence(bool motion)
{
	Lcd_PortType ports[] = {D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port};
	Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
	lcd = Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
	if(motion)
	{
	Lcd_string(&lcd,"Motion Welcome");
	Lcd_cursor(&lcd,1,0);
	}
}

void registerCallback(extiCallback ptr)
{
	extiPtr=ptr;
}

void registerCallback(timerCallback ptr)
{
	timerPtr=ptr;
}

void registerCallback(motionCallback ptr)
{
	motionPtr=ptr;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==SW5_Pin){
		registerCallback(WLED_callback);
		if(extiPtr!=NULL)
			{
				extiPtr(GPIO_Pin,&htim2);
			}
	}
	else if(GPIO_Pin==PIR_OUT_Pin){
		registerCallback(motionDetected);
		if(motionPtr!=NULL)
			{
				motionPtr();
			}
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(timerPtr!=NULL)
		{
			timerPtr(htim);
		}
}

void WLED_callback(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim)
{
	if(GPIO_Pin == GPIO_PIN_5) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
    	switche.setState(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_SET);
    	HAL_TIM_Base_Start_IT(htim);
    }
}

void timerElapsed(TIM_HandleTypeDef* htim)
{
	switche.setState(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_RESET);
	HAL_TIM_Base_Stop_IT(htim);
}

void motionDetected(){
		motionFlag=true;
}
