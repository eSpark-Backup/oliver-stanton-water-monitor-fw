#include "main.h"
#include "mainApp.hpp"
#include "usb_device.h"
#include "stm32f0xx_hal.h"
#include "lcd.h"

funcPtrEXTI ptrObj;
funcPtrTimer ptrObj1;

Lcd_HandleTypeDef lcd;
void program_init()
{
	HAL_GPIO_WritePin(GPIOE,PWM_CNTRL_Pin,GPIO_PIN_SET);

}

void lcd_sequence()
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

void registerHandler(funcPtrEXTI ptr)
{
	ptrObj=ptr;
}

void registerHandler1(funcPtrTimer ptr)
{
	ptrObj1=ptr;
}

void handler(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim)
{
	if(ptrObj!=NULL)
	{
		ptrObj(GPIO_Pin,htim);
	}
}

void handler1(TIM_HandleTypeDef* htim)
{
	if(ptrObj1!=NULL)
	{
		ptrObj1(htim);
	}
}

void WLED_func(uint16_t GPIO_Pin,TIM_HandleTypeDef* htim)
{
	if(GPIO_Pin == GPIO_PIN_5) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
    	HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_SET);
    	HAL_TIM_Base_Start_IT(htim);
    }
}

void timerElapsed_func(TIM_HandleTypeDef* htim)
{
	HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_RESET);
	HAL_TIM_Base_Stop_IT(htim);
}
