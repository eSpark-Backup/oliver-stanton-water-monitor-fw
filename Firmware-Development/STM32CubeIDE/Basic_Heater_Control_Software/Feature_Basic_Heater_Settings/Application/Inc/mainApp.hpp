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

#define UPBUTTON			SW4_Pin
#define UPBUTTONPORT		SW4_GPIO_Port
#define UP_Flag				sw4Flag
#define DOWNBUTTON			SW6_Pin
#define DOWNBUTTONPORT		SW6_GPIO_Port
#define DOWN_Flag			sw6Flag
#define ENTERBUTTON			SW7_Pin
#define ENTERBUTTONPORT		SW7_GPIO_Port
#define ENTER_Flag			sw7Flag
#define BACKBUTTON			SW5_Pin
#define BACKBUTTONPORT		SW5_GPIO_Port
#define BACK_Flag			sw5Flag

//-----------------------------------------------------------------------------------------

//-----declared here, defined in controls.cpp
extern GPIOConfig TDS1;// = {TDS1_T2_GPIO_Port, TDS1_T1_GPIO_Port, ADC_CHANNEL_10, TDS1_T2_Pin, TDS1_T1_Pin, ADC_CHANNEL_11};
extern GPIOConfig TDS2;// = {TDS2_T2_GPIO_Port, TDS2_T1_GPIO_Port, ADC_CHANNEL_14, TDS2_T2_Pin, TDS2_T1_Pin, ADC_CHANNEL_15};
extern GPIOConfig TDS3;// = {TDS3_T2_GPIO_Port, TDS3_T1_GPIO_Port, ADC_CHANNEL_12, TDS3_T2_Pin, TDS3_T1_Pin, ADC_CHANNEL_13};
extern GPIOConfig TDS4;// = {TDS4_T2_GPIO_Port, TDS4_T1_GPIO_Port, ADC_CHANNEL_1, TDS4_T2_Pin, TDS4_T1_Pin, ADC_CHANNEL_2};



//-----------------------------------------------------------------------------------------

class MainApp {
public:
	void setup();
	void loop();
};



