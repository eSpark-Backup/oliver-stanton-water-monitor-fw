#pragma once

#include <main.h>
#include "stm32f0xx_hal.h"
#include "stdio.h"
#include <functional>
#include "Display.h"
#include "CDC_Serial.h"
#include "TDS.hpp"
//#include "eventHandle.hpp"

/*--------------debug-------------------*/
//#define _DEBUG_										//un-comment these three lines for receiving DEBUG Messages
#define USE_FULL_ASSERT 1

/* Private defines -----------------------------------------------------------*/

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





//-----------------------------------------------------------------------------------------

class MainApp {
public:
	void setup();
	void loop();
};



