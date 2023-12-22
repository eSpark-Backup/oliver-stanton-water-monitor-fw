/*
 * flagHandler.c
 *
 *  Created on: Dec 15, 2023
 *      Author: Farhan Ahmed
 */


#include "flagHandler.h"
#include "controls.h"

#include "usbd_cdc_if.h"

static Display* dispa = new CDC_Serial;

uint32_t st9;

//typedef enum{
//	Level_Sensor,
//	Pressure_Sensor,
//	Flow_Meter_1,
//	Flow_Meter_2,
//	Flow_Meter_3,
//	WLED,
//	RGB,
//	Buzzer,
//	FoD
//}sensors;
//
//typedef enum{
//	Settings,
//	Status
//}Task;

//flagRegister flagsReg;


//namespace
//{
//	flagHandle* flagHandleInstance = nullptr;
//}
//flagHandle& flagHandle::getInstance()
//{
//    if(nullptr == flagHandleInstance)
//    {
//    	flagHandleInstance = new flagHandle();
//    }
//    return *flagHandleInstance;
//}


void flagHandle::flashRetrieve(){

}

void flagHandle::flashCommit(){

}

void flagHandle::update(void){
	if(HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port, LEVEL_SENSOR_Pin) != flagStatus(Status, Level_Sensor))
	{
		setFlag(Status, Level_Sensor, HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port, LEVEL_SENSOR_Pin));		//inverse logic
//		flagsReg.status.Level_Sensor = HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port, LEVEL_SENSOR_Pin);
//		flagsReg.status.Level_Sensor = !flagsReg.status.Level_Sensor;
		if(flagStatus(Status, Level_Sensor) == false)
		{
			st9 = HAL_GetTick();
		}
		char strng[50];
		int len = sprintf(strng, "saved state: %d\r\n", flagStatus(Status, Level_Sensor));
		HAL_Delay(1);
		CDC_Transmit_FS((uint8_t *)strng, len);
	}

//	flagsReg.bits.Pressure_Sensor = HAL_GPIO_ReadPin();
//	flagsReg.bits.Flow_Meter_1 = HAL_GPIO_ReadPin();
//	flagsReg.bits.Flow_Meter_2 = HAL_GPIO_ReadPin();
//	flagsReg.bits.Flow_Meter_3 = HAL_GPIO_ReadPin();

}

bool flagHandle::timeElapsed(uint8_t sensor, uint32_t elapsedTime){
	if((sensor == Level_Sensor) && (flagStatus(Status, Level_Sensor) != true)){
		uint32_t elapsed = HAL_GetTick()-st9;
//		char strng[90];
//		int len = sprintf(strng, "elapsed: %ld ", elapsed);
//		HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)strng, len);
//		len = sprintf(strng, " %d\r\n", flagStatus(Status, Level_Sensor));
//		HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)strng, len);
		if(elapsed >= elapsedTime){
//			char strng[50];
//			int len = sprintf(strng, "elapsed: %ld\r\n", elapsed);
//			HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)strng, len);
			return true;	}

	}
//	if(sensor == Pressure_Sensor && flagsReg.bits.Pressure_Sensor == false){
//		uint32_t elapsed = HAL_GetTick()-st9;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == Flow_Meter_1 && flagsReg.bits.Flow_Meter_1 == false){
//		uint32_t elapsed = HAL_GetTick()-st9;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == Flow_Meter_2 && flagsReg.bits.Flow_Meter_2 == false){
//		uint32_t elapsed = HAL_GetTick()-st9;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == Flow_Meter_3 && flagsReg.bits.Flow_Meter_3 == false){
//		uint32_t elapsed = HAL_GetTick()-st9;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}

	return false;
}

bool flagHandle::flagStatus(uint8_t op, uint8_t sensor){
	if(op == Settings)	{
			if(sensor == FoD)		{dispa->displayVar(flagsReg->settings.FillOnDemand, "Returning FoD");	return flagsReg->settings.FillOnDemand;}
			if(sensor ==  Buzzer)	return flagsReg->settings.Buzzer;
			if(sensor ==  RGB)		return flagsReg->settings.RGB;
			if(sensor ==  WLED)		return flagsReg->settings.WLED;
		}
	else if (op == Status)	{
			if(sensor == Level_Sensor)		return flagsReg->status.Level_Sensor;		//ST9 pin is inverse logic?
			if(sensor == Pressure_Sensor)	return flagsReg->status.Pressure_Sensor;
			if(sensor == Flow_Meter_1)		return flagsReg->status.Flow_Meter_1;
			if(sensor == Flow_Meter_2)		return flagsReg->status.Flow_Meter_2;
			if(sensor == Flow_Meter_3)		return flagsReg->status.Flow_Meter_3;
		}
	else
		return false;
}

bool flagHandle::setFlag(uint8_t op, uint8_t sensor, uint8_t state){
	if(op == Settings)	{
		if(sensor == FoD)		{flagsReg->settings.FillOnDemand = state;		dispa->displayVar(flagStatus(Settings, FoD), "inside setFlag");	}
		if(sensor == Buzzer)	flagsReg->settings.Buzzer = state;
		if(sensor == RGB)		flagsReg->settings.RGB = state;
		if(sensor == WLED)		flagsReg->settings.WLED = state;
		}
	else if (op == Status)	{
		if(sensor == Level_Sensor)		flagsReg->status.Level_Sensor = state;
		if(sensor == Pressure_Sensor)	flagsReg->status.Pressure_Sensor = state;
		if(sensor == Flow_Meter_1)		flagsReg->status.Flow_Meter_1 = state;
		if(sensor == Flow_Meter_2)		flagsReg->status.Flow_Meter_2 = state;
		if(sensor == Flow_Meter_3)		flagsReg->status.Flow_Meter_3 = state;
		}
}

void flagHandle::flagsInit(){
	setFlag(Settings, FoD, true);
}

//void flagHandle::(){
//
//}
