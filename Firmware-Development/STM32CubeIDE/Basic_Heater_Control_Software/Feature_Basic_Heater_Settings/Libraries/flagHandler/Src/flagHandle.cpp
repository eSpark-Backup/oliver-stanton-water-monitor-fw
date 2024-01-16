/*
 * flagHandler.c
 *
 *  Created on: Dec 15, 2023
 *      Author: Farhan Ahmed
 */


#include "flagHandle.h"
#include "controls.h"
#include "eventHandle.hpp"


uint32_t statusFlagTimeStamp[_sensors_];			//Time when status of output/sensor was changed



void FlagHandle::flashRetrieve(){

}

void FlagHandle::flashCommit(){

}

void FlagHandle::update(void){
	//INPUTS
	if(HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port, LEVEL_SENSOR_Pin) != flagStatus(Status, Level_Sensor))	{
		setFlag(Status, Level_Sensor, HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port, LEVEL_SENSOR_Pin));		//inverse logic
		if(flagStatus(Status, Level_Sensor) == false)	{
			statusFlagTimeStamp[Level_Sensor] = HAL_GetTick();
			DEBUG_W("Level Sensor Closed.");
		}
		else
			DEBUG_W("Level Sensor Open.");
	}
	//OUTPUTS
	if(HAL_GPIO_ReadPin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin) != flagStatus(Status, MEM_Saver))	{
//		setFlag(Status, MEM_Saver, HAL_GPIO_ReadPin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin));		//inverse logic
		if(flagStatus(Status, MEM_Saver))	{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin, GPIO_PIN_SET);
			DEBUG_W("Mem_Saver_ST3 turned on.");	}
		else	{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin, GPIO_PIN_RESET);
			DEBUG_W("Mem_Saver_ST3 turned off.");	//}
//		if(flagStatus(Status, MEM_Saver) == false)	{
			statusFlagTimeStamp[MEM_Saver] = HAL_GetTick();
		}
	}
	if(HAL_GPIO_ReadPin(RO_SAVER_GPIO_Port, RO_SAVER_Pin) != flagStatus(Status, RESIN_Saver))	{
		if(flagStatus(Status, RESIN_Saver))	{
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(RO_SAVER_GPIO_Port, RO_SAVER_Pin, GPIO_PIN_SET);
			DEBUG_W("RO_Saver_ST1 turned on.");	}
		else	{
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(RO_SAVER_GPIO_Port, RO_SAVER_Pin, GPIO_PIN_RESET);
			DEBUG_W("RO_Saver_ST1 turned off.");	//}
//		if(flagStatus(Status, MEM_Saver) == false)	{
			statusFlagTimeStamp[RESIN_Saver] = HAL_GetTick();
		}
	}
	if(HAL_GPIO_ReadPin(AUTO_FILL_GPIO_Port, AUTO_FILL_Pin) != flagStatus(Status, AUTO_Fill))	{
		if(flagStatus(Status, AUTO_Fill))	{
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AUTO_FILL_GPIO_Port, AUTO_FILL_Pin, GPIO_PIN_SET);
			DEBUG_W("Auto_Fill_ST5 turned on.");	}
		else	{
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AUTO_FILL_GPIO_Port, AUTO_FILL_Pin, GPIO_PIN_RESET);
			DEBUG_W("Auto_Fill_ST5 turned off.");	//}
//		if(flagStatus(Status, MEM_Saver) == false)	{
			statusFlagTimeStamp[AUTO_Fill] = HAL_GetTick();
		}
	}
//	if(HAL_GPIO_ReadPin(W_LED_CTRL_GPIO_Port, W_LED_CTRL_Pin) != flagStatus(Status, AUTO_Fill))	{
//		if(flagStatus(Status, AUTO_Fill))	{
//			HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port, W_LED_CTRL_Pin, GPIO_PIN_SET);
//			DEBUG_W("W_LED turned on.");	}
//		else	{
//			HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port, W_LED_CTRL_Pin, GPIO_PIN_RESET);
//			DEBUG_W("W_LED turned off.");	//}
////		if(flagStatus(Status, MEM_Saver) == false)	{
//			statusFlagTimeStamp[WLED] = HAL_GetTick();
//		}
//	}

//	FlagsReg.bits.Pressure_Sensor = HAL_GPIO_ReadPin();
//	FlagsReg.bits.Flow_Meter_1 = HAL_GPIO_ReadPin();
//	FlagsReg.bits.Flow_Meter_2 = HAL_GPIO_ReadPin();
//	FlagsReg.bits.Flow_Meter_3 = HAL_GPIO_ReadPin();

}

bool FlagHandle::timeElapsed(uint8_t sensor, uint32_t elapsedTime){
	if((sensor == Level_Sensor) && (flagStatus(Status, Level_Sensor) != true)){
		uint32_t elapsed = HAL_GetTick()-statusFlagTimeStamp[sensor];
		if(elapsed >= elapsedTime){
			return true;	}
	}
	if(sensor == MEM_Saver){// && flagStatus(Settings, MEM_Saver) != true){
		uint32_t elapsed = HAL_GetTick() - statusFlagTimeStamp[sensor];
//		dispdebug->displayVar(statusFlagTimeStamp[sensor], "statusFlagTimeStamp[sensor]: ");
//		dispdebug->displayVar(HAL_GetTick(), "HAL_GetTick(): ");
//		dispdebug->displayVar(elapsed, "mem_Saver elapsed: ");
		if(elapsed >= elapsedTime)
		{	return true;			}
		else
		{	return false;			}
	}
//	if(sensor == Pressure_Sensor && FlagsReg.bits.Pressure_Sensor == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == Flow_Meter_1 && FlagsReg.bits.Flow_Meter_1 == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == Flow_Meter_2 && FlagsReg.bits.Flow_Meter_2 == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == Flow_Meter_3 && FlagsReg.bits.Flow_Meter_3 == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}

	return false;
}

bool FlagHandle::flagStatus(uint8_t op, uint8_t sensor){
	if(op == Settings)	{
		if(sensor == FoD)					return FlagsReg.settings.FillOnDemand;
		else if(sensor == Buzzer)			return FlagsReg.settings.Buzzer;
		else if(sensor == RGB)				return FlagsReg.settings.RGB;
		else if(sensor == WLED)				return FlagsReg.settings.WLED;
		else if(sensor == MEM_Saver)		return FlagsReg.settings.Mem_Saver;
		else if(sensor == Mem_Saver_Pause)	return FlagsReg.settings.Mem_Saver_Pause;
		else if(sensor == RESIN_Saver)		return FlagsReg.settings.Resin_Saver;
		else if(sensor == AUTO_Fill)		return FlagsReg.settings.Auto_Fill;
		}
	else if (op == Status)	{
		if(sensor == Level_Sensor)				return FlagsReg.status.Level_Sensor;		//ST9 pin is inverse logic?
		else if(sensor == WLED)					return FlagsReg.status.WLED;
		else if(sensor == Pressure_Sensor)		return FlagsReg.status.Pressure_Sensor;
		else if(sensor == Flow_Meter_1)			return FlagsReg.status.Flow_Meter_1;
		else if(sensor == Flow_Meter_2)			return FlagsReg.status.Flow_Meter_2;
		else if(sensor == Flow_Meter_3)			return FlagsReg.status.Flow_Meter_3;
		else if(sensor == RESIN_Saver)			return FlagsReg.status.Resin_Saver;
		else if(sensor == MEM_Saver)			return FlagsReg.status.Mem_Saver;
		else if(sensor == AUTO_Fill)			return FlagsReg.status.Auto_Fill;
		else if(sensor == Pure_CYCLE)			return FlagsReg.status.Pure_CYCLE;
		}
//	else
		return false;
}

bool FlagHandle::setFlag(uint8_t op, uint8_t sensor, uint8_t state){
	DEBUG_I("Inside setFlag");
	DEBUG_(" operation: %d, Sensor: %d, State: %d", op, sensor, state);
	if(op == Settings)	{
		if(sensor == FoD)					FlagsReg.settings.FillOnDemand = state;
		else if(sensor == Buzzer)			FlagsReg.settings.Buzzer = state;
		else if(sensor == RGB)				FlagsReg.settings.RGB = state;
		else if(sensor == WLED)				FlagsReg.settings.WLED = state;
		else if(sensor == RESIN_Saver)		FlagsReg.settings.Resin_Saver = state;			//used for ro_saver_thresh
		else if(sensor == MEM_Saver)		FlagsReg.settings.Mem_Saver = state;
		else if(sensor == Mem_Saver_Pause)	FlagsReg.settings.Mem_Saver_Pause = state;
		else if(sensor == AUTO_Fill)		FlagsReg.settings.Auto_Fill = state;
		}
	else if (op == Status)	{
		if(sensor == Level_Sensor)			FlagsReg.status.Level_Sensor = state;
		else if(sensor == WLED)				FlagsReg.status.WLED = state;
		else if(sensor == Pressure_Sensor)	FlagsReg.status.Pressure_Sensor = state;
		else if(sensor == Flow_Meter_1)		FlagsReg.status.Flow_Meter_1 = state;
		else if(sensor == Flow_Meter_2)		FlagsReg.status.Flow_Meter_2 = state;
		else if(sensor == Flow_Meter_3)		FlagsReg.status.Flow_Meter_3 = state;
		else if(sensor == RESIN_Saver)		FlagsReg.status.Resin_Saver = state;			//used for ro_saver_thresh
		else if(sensor == MEM_Saver)		FlagsReg.status.Mem_Saver = state;
		else if(sensor == AUTO_Fill)		FlagsReg.status.Auto_Fill = state;
		else if(sensor == Pure_CYCLE)		FlagsReg.status.Pure_CYCLE = state;
		}
	DEBUG_I("Exiting setFlag.");
	return false;
}

void FlagHandle::flagsInit(){								//Implement flashRetrieve() here
//	flashRetrieve();
	setFlag(Settings, WLED, true);
	setFlag(Settings, Buzzer, true);
}

//void flagHandle::(){
//
//}
