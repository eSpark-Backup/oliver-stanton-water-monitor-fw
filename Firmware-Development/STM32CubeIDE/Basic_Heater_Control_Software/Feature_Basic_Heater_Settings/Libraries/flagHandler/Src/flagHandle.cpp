/*
 * flagHandler.c
 *
 *  Created on: Dec 15, 2023
 *      Author: Farhan Ahmed
 */


#include "flagHandle.h"
#include "controls.h"


//uint32_t st9;
uint32_t statusFlagTimeStamp[_sensors_];



void flagHandle::flashRetrieve(){

}

void flagHandle::flashCommit(){

}

void flagHandle::update(void){
	if(HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port, LEVEL_SENSOR_Pin) != flagStatus(Status, Level_Sensor))	{
		setFlag(Status, Level_Sensor, HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port, LEVEL_SENSOR_Pin));		//inverse logic
		if(flagStatus(Status, Level_Sensor) == false)	{
			statusFlagTimeStamp[Level_Sensor] = HAL_GetTick();
		}
	}
	if(HAL_GPIO_ReadPin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin) != flagStatus(Status, MEM_Saver))	{
		setFlag(Status, MEM_Saver, HAL_GPIO_ReadPin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin));		//inverse logic
		if(flagStatus(Status, MEM_Saver) == false)	{
			statusFlagTimeStamp[MEM_Saver] = HAL_GetTick();
//			dispdebug->displayVar(statusFlagTimeStamp[MEM_Saver], "tick saved with stamp: ");
		}
	}

//	flagsReg.bits.Pressure_Sensor = HAL_GPIO_ReadPin();
//	flagsReg.bits.Flow_Meter_1 = HAL_GPIO_ReadPin();
//	flagsReg.bits.Flow_Meter_2 = HAL_GPIO_ReadPin();
//	flagsReg.bits.Flow_Meter_3 = HAL_GPIO_ReadPin();

#ifdef _DEBUG_
//		char strng[50];
//		sprintf(strng, "saved state: %d\r\n", flagStatus(Status, Level_Sensor));
//		dispdebug->displayString(strng);
#endif
}

bool flagHandle::timeElapsed(uint8_t sensor, uint32_t elapsedTime){
	if((sensor == Level_Sensor) && (flagStatus(Status, Level_Sensor) != true)){
		uint32_t elapsed = HAL_GetTick()-statusFlagTimeStamp[sensor];
#ifdef _DEBUG_
		dispdebug->displayVar(elapsed, "elapsed: ");
		dispdebug->displayVar(flagStatus(Status, Level_Sensor), " ");
#endif
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
#ifdef _DEBUG_
//	dispdebug->displayVar(op, "inside flagStatus, op:");
//	dispdebug->displayVar(sensor, "inside flagStatus, sensor: ");
#endif
	if(op == Settings)	{
			if(sensor == FoD)					return flagsReg.settings.FillOnDemand;
			else if(sensor ==  Buzzer)			return flagsReg.settings.Buzzer;
			else if(sensor ==  RGB)				return flagsReg.settings.RGB;
			else if(sensor ==  WLED)			return flagsReg.settings.WLED;
			else if(sensor ==  RESIN_Saver)		return flagsReg.settings.Resin_Saver;
			else if(sensor ==  MEM_Saver)		return flagsReg.settings.Mem_Saver;
			else if(sensor ==  Mem_Saver_Pause)	return flagsReg.settings.Mem_Saver_Pause;
		}
	else if (op == Status)	{
			if(sensor == Level_Sensor)			return flagsReg.status.Level_Sensor;		//ST9 pin is inverse logic?
			else if(sensor == Pressure_Sensor)	return flagsReg.status.Pressure_Sensor;
			else if(sensor == Flow_Meter_1)		return flagsReg.status.Flow_Meter_1;
			else if(sensor == Flow_Meter_2)		return flagsReg.status.Flow_Meter_2;
			else if(sensor == Flow_Meter_3)		return flagsReg.status.Flow_Meter_3;
			else if(sensor == MEM_Saver)		return flagsReg.status.Mem_Saver;
		}
	else
		return false;
}

bool flagHandle::setFlag(uint8_t op, uint8_t sensor, uint8_t state){
#ifdef _DEBUG_
	dispdebug->displayVar(op, "inside setFlag, op: ");
	dispdebug->displayVar(sensor, "inside setFlag, sensor: ");
	dispdebug->displayVar(state, "inside setFlag, state: ");
#endif
	if(op == Settings)	{
		if(sensor == FoD)					flagsReg.settings.FillOnDemand = state;
		else if(sensor == Buzzer)			flagsReg.settings.Buzzer = state;
		else if(sensor == RGB)				flagsReg.settings.RGB = state;
		else if(sensor == WLED)				flagsReg.settings.WLED = state;
		else if(sensor ==  RESIN_Saver)		flagsReg.settings.Resin_Saver = state;			//used for ro_saver_thresh
		else if(sensor ==  MEM_Saver)		flagsReg.settings.Mem_Saver = state;
		else if(sensor ==  Mem_Saver_Pause)	flagsReg.settings.Mem_Saver_Pause = state;
		}
	else if (op == Status)	{
		if(sensor == Level_Sensor)			flagsReg.status.Level_Sensor = state;
		else if(sensor == Pressure_Sensor)	flagsReg.status.Pressure_Sensor = state;
		else if(sensor == Flow_Meter_1)		flagsReg.status.Flow_Meter_1 = state;
		else if(sensor == Flow_Meter_2)		flagsReg.status.Flow_Meter_2 = state;
		else if(sensor == Flow_Meter_3)		flagsReg.status.Flow_Meter_3 = state;
		else if(sensor == MEM_Saver)		flagsReg.status.Mem_Saver = state;
		}
#ifdef _DEBUG_
	dispdebug->displayString("Exiting setFlag.\r\n");
#endif
}

void flagHandle::flagsInit(){								//Implement flashRetrieve() here
//	setFlag(Settings, FoD, false);
}

//void flagHandle::(){
//
//}
