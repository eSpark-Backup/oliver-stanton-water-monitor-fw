#include "motorDriver.h"
#include "mainApp.hpp"
#include "main.h"
#include "stm32f0xx_hal.h"

extern bool pumpConnectivityFlag;
double voltage  = 0;
double current = 0;
double sensitivity = 0.2 ; // volts/Amp
extern float Vref_PB1;
extern ADC_HandleTypeDef hadc;

void MotorDriver::enablePump1(){
	HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, GPIO_PIN_RESET);
}

void MotorDriver::enablePump2(){
	HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, GPIO_PIN_RESET);
}

void MotorDriver::disablePump1(){
	HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, GPIO_PIN_SET);
}

void MotorDriver::disablePump2(){
	HAL_GPIO_WritePin(INB_GPIO_Port, INB_Pin, GPIO_PIN_SET);
}

void MotorDriver::enableDT1(){
	HAL_GPIO_WritePin(IOC9_GPIO_Port,IOC9_Pin,GPIO_PIN_SET);
}

void MotorDriver::disableDT1(){
	HAL_GPIO_WritePin(IOC9_GPIO_Port,IOC9_Pin,GPIO_PIN_RESET);
}

void MotorDriver::pumpConnectivityCallback(TIM_HandleTypeDef* htim)
{
	pumpConnectivityFlag=false;
	HAL_TIM_Base_Stop_IT(htim);
}

void MotorDriver::enablePumps(){
	HAL_GPIO_WritePin(Enable_GPIO_Port, Enable_Pin, GPIO_PIN_SET);
}

float MotorDriver::getCurrent(){
	uint32_t adc_val;
	HAL_ADC_Start(&hadc);
	// Poll ADC1 Peripheral & TimeOut = 1mSec
	HAL_ADC_PollForConversion(&hadc, 10);

	adc_val = HAL_ADC_GetValue(&hadc); // get the adc value

	voltage = adc_val * 3.3/4096; //reference voltage (volts) when 0 current is flowing

	current = (Vref_PB1-voltage)/sensitivity;

	return current;
	return (float)3.142;
}

