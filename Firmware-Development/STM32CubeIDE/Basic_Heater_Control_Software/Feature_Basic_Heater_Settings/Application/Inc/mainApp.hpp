#pragma once

#include <main.h>
#include "stm32f0xx_hal.h"
#include "stdio.h"
#include <functional>
#include "Display.h"
#include "CDC_Serial.h"

/*--------------debug-------------------*/
//#define _DEBUG_										//un-comment these three lines for receiving
#include "usbd_cdc_if.h"								//debugging messages on USB serial port
static Display* dispdebug = new CDC_Serial;				//Debugs will always be on USB_CDC

/* Private defines -----------------------------------------------------------*/
//#define LED_TOGGLE_Pin GPIO_PIN_8
//#define LED_TOGGLE_GPIO_Port GPIOE
//#define MEM_SAVER_Pin GPIO_PIN_10
//#define MEM_SAVER_GPIO_Port GPIOE
//#define BUZZER_Pin GPIO_PIN_8
//#define BUZZER_GPIO_Port GPIOA
//#define LEVEL_SENSOR_Pin GPIO_PIN_3
//#define LEVEL_SENSOR_GPIO_Port GPIOD
//#define W_LED_CTRL_Pin GPIO_PIN_4
//#define W_LED_CTRL_GPIO_Port GPIOD

#define UP_Button			SW4_Pin
#define UP_Button_Port		SW4_GPIO_Port
#define UP_Flag				sw4Flag
#define DOWN_Button			SW6_Pin
#define DOWN_Button_Port	SW6_GPIO_Port
#define DOWN_Flag			sw6Flag
#define ENTER_Button		SW7_Pin
#define ENTER_Button_Port	SW7_GPIO_Port
#define ENTER_Flag			sw7Flag
#define BACK_Button			SW5_Pin
#define BACK_Button_Port	SW5_GPIO_Port
#define BACK_Flag			sw5Flag

//#define PIR_OUT_Pin GPIO_PIN_9
//#define PIR_OUT_GPIO_Port GPIOB
//#define PIR_OUT_EXTI_IRQn EXTI4_15_IRQn
//-----------------------------------------------------------------------------------------

class MainApp {
public:
	void setup();
	void loop();
};



