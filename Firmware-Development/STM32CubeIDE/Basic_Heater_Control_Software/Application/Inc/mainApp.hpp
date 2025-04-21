#pragma once

#include <main.h>
#include "stm32f0xx_hal.h"
#include "stdio.h"
#include <functional>
#include "Display.h"
#include "CDC_Serial.h"
//#include "TDS.hpp"
//#include "eventHandle.hpp"



/* Private defines -----------------------------------------------------------*/

#define RODI_BUTTON			SW3_Pin
#define RODI_BUTTON_PORT	SW3_GPIO_Port
#define RODI_BUTTON_FLAG	sw3Flag
#define UP_BUTTON			SW4_Pin
#define UP_BUTTON_PORT		SW4_GPIO_Port
#define UP_BUTTON_FLAG		sw4Flag
#define BACK_BUTTON			SW5_Pin
#define BACK_BUTTON_PORT	SW5_GPIO_Port
#define BACK_BUTTON_FLAG	sw5Flag
#define DOWN_BUTTON			SW6_Pin
#define DOWN_BUTTON_PORT	SW6_GPIO_Port
#define DOWN_BUTTON_FLAG	sw6Flag
#define ENTER_BUTTON		SW7_Pin
#define ENTER_BUTTON_PORT	SW7_GPIO_Port
#define ENTER_BUTTON_FLAG	sw7Flag


//-----------------------------------------------------------------------------------------

//-----declared here, defined in controls.cpp
//extern GPIOConfig tds1PinConfig;// = {TDS1_T2_GPIO_Port, TDS1_T1_GPIO_Port, ADC_CHANNEL_10, TDS1_T2_Pin, TDS1_T1_Pin, ADC_CHANNEL_11};
//extern GPIOConfig tds2PinConfig;// = {TDS2_T2_GPIO_Port, TDS2_T1_GPIO_Port, ADC_CHANNEL_14, TDS2_T2_Pin, TDS2_T1_Pin, ADC_CHANNEL_15};
//extern GPIOConfig tds3PinConfig;// = {TDS3_T2_GPIO_Port, TDS3_T1_GPIO_Port, ADC_CHANNEL_12, TDS3_T2_Pin, TDS3_T1_Pin, ADC_CHANNEL_13};
//extern GPIOConfig tds4PinConfig;// = {TDS4_T2_GPIO_Port, TDS4_T1_GPIO_Port, ADC_CHANNEL_1, TDS4_T2_Pin, TDS4_T1_Pin, ADC_CHANNEL_2};



//-----------------------------------------------------------------------------------------

class MainApp {
public:
	void setup();
	void loop();

private:
	bool invokeMenu();
};


extern volatile uint32_t waste_pulse_count;
extern volatile uint32_t wasteFlow_rate_Lhr;
extern volatile uint32_t pure_pulse_count;
extern volatile uint32_t pureFlow_rate_Lhr;
