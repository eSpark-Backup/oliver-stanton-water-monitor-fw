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



void flagHandle::flashRetrieve(){

}

void flagHandle::flashCommit(){

}

void flagHandle::update(void){
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

//	flagsReg.bits.Pressure_Sensor = HAL_GPIO_ReadPin();
//	flagsReg.bits.Flow_Meter_1 = HAL_GPIO_ReadPin();
//	flagsReg.bits.Flow_Meter_2 = HAL_GPIO_ReadPin();
//	flagsReg.bits.Flow_Meter_3 = HAL_GPIO_ReadPin();

}

bool flagHandle::timeElapsed(uint8_t sensor, uint32_t elapsedTime){
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
//	if(sensor == Pressure_Sensor && flagsReg.bits.Pressure_Sensor == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == Flow_Meter_1 && flagsReg.bits.Flow_Meter_1 == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == Flow_Meter_2 && flagsReg.bits.Flow_Meter_2 == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == Flow_Meter_3 && flagsReg.bits.Flow_Meter_3 == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}

	return false;
}

bool flagHandle::flagStatus(uint8_t op, uint8_t sensor){
	if(op == Settings)	{
		if(sensor == FoD)					return flagsReg.settings.FillOnDemand;
		else if(sensor == Buzzer)			return flagsReg.settings.Buzzer;
		else if(sensor == RGB)				return flagsReg.settings.RGB;
		else if(sensor == WLED)				return flagsReg.settings.WLED;
		else if(sensor == MEM_Saver)		return flagsReg.settings.Mem_Saver;
		else if(sensor == Mem_Saver_Pause)	return flagsReg.settings.Mem_Saver_Pause;
		else if(sensor == RESIN_Saver)		return flagsReg.settings.Resin_Saver;
		else if(sensor == AUTO_Fill)		return flagsReg.settings.Auto_Fill;
		}
	else if (op == Status)	{
		if(sensor == Level_Sensor)				return flagsReg.status.Level_Sensor;		//ST9 pin is inverse logic?
		else if(sensor == WLED)					return flagsReg.status.WLED;
		else if(sensor == Pressure_Sensor)		return flagsReg.status.Pressure_Sensor;
		else if(sensor == Flow_Meter_1)			return flagsReg.status.Flow_Meter_1;
		else if(sensor == Flow_Meter_2)			return flagsReg.status.Flow_Meter_2;
		else if(sensor == Flow_Meter_3)			return flagsReg.status.Flow_Meter_3;
		else if(sensor == RESIN_Saver)			return flagsReg.status.Resin_Saver;
		else if(sensor == MEM_Saver)			return flagsReg.status.Mem_Saver;
		else if(sensor == AUTO_Fill)			return flagsReg.status.Auto_Fill;
		else if(sensor == Pure_CYCLE)			return flagsReg.status.Pure_CYCLE;
		}
	else
		return false;
}

bool flagHandle::setFlag(uint8_t op, uint8_t sensor, uint8_t state){
	DEBUG_I("Inside setFlag");
	DEBUG_(" operation: %d, Sensor: %d, State: %d", op, sensor, state);
	if(op == Settings)	{
		if(sensor == FoD)					flagsReg.settings.FillOnDemand = state;
		else if(sensor == Buzzer)			flagsReg.settings.Buzzer = state;
		else if(sensor == RGB)				flagsReg.settings.RGB = state;
		else if(sensor == WLED)				flagsReg.settings.WLED = state;
		else if(sensor == RESIN_Saver)		flagsReg.settings.Resin_Saver = state;			//used for ro_saver_thresh
		else if(sensor == MEM_Saver)		flagsReg.settings.Mem_Saver = state;
		else if(sensor == Mem_Saver_Pause)	flagsReg.settings.Mem_Saver_Pause = state;
		else if(sensor == AUTO_Fill)		flagsReg.settings.Auto_Fill = state;
		}
	else if (op == Status)	{
		if(sensor == Level_Sensor)			flagsReg.status.Level_Sensor = state;
		else if(sensor == WLED)				flagsReg.status.WLED = state;
		else if(sensor == Pressure_Sensor)	flagsReg.status.Pressure_Sensor = state;
		else if(sensor == Flow_Meter_1)		flagsReg.status.Flow_Meter_1 = state;
		else if(sensor == Flow_Meter_2)		flagsReg.status.Flow_Meter_2 = state;
		else if(sensor == Flow_Meter_3)		flagsReg.status.Flow_Meter_3 = state;
		else if(sensor == RESIN_Saver)		flagsReg.status.Resin_Saver = state;			//used for ro_saver_thresh
		else if(sensor == MEM_Saver)		flagsReg.status.Mem_Saver = state;
		else if(sensor == AUTO_Fill)		flagsReg.status.Auto_Fill = state;
		else if(sensor == Pure_CYCLE)		flagsReg.status.Pure_CYCLE = state;
		}
	DEBUG_I("Exiting setFlag.");
}

void flagHandle::flagsInit(){								//Implement flashRetrieve() here
	setFlag(Settings, WLED, true);
	setFlag(Settings, Buzzer, true);
}

//void flagHandle::(){
//
//}
