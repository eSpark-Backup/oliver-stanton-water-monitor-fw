/*
 * flagHandler.c
 *
 *  Created on: Dec 15, 2023
 *      Author: Farhan Ahmed
 */


#include "flagHandle.h"
#include "controlsequence.h"
#include "eventHandle.hpp"
#include "Buzzer.hpp"


uint32_t statusFlagTimeStamp[_sensors_];			//Time when status of output/sensor was changed
uint32_t statusOnFlagTimeStamp[_sensors_];


void FlagHandle::flashRetrieve()
{

}

void FlagHandle::flashCommit()
{

}

void FlagHandle::update(void)
{
	//INPUTS
	if(HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port, LEVEL_SENSOR_Pin) != flagStatus(Status, LEVEL_SENSOR))
	{
		setFlag(Status, LEVEL_SENSOR, HAL_GPIO_ReadPin(LEVEL_SENSOR_GPIO_Port, LEVEL_SENSOR_Pin));		//inverse logic
		if(flagStatus(Status, LEVEL_SENSOR) == false)
		{
			statusFlagTimeStamp[LEVEL_SENSOR] = HAL_GetTick();
			DEBUG_RTT(LOG_WARNING, "Level Sensor Closed.");
		}
		else
		{
			statusOnFlagTimeStamp[LEVEL_SENSOR] = HAL_GetTick();
			DEBUG_RTT(LOG_WARNING, "Level Sensor Open.");
		}
	}
	//OUTPUTS
	if(HAL_GPIO_ReadPin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin) != flagStatus(Status, MEM_SAVER))
	{
//		setFlag(Status, MEM_SAVER, HAL_GPIO_ReadPin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin));		//inverse logic
		if(flagStatus(Status, MEM_SAVER))
		{
			HAL_GPIO_WritePin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin, GPIO_PIN_SET);
			DEBUG_RTT(LOG_WARNING, "Mem_Saver_ST3 turned on.");
			statusOnFlagTimeStamp[MEM_SAVER] = HAL_GetTick();
		}
		else
		{
			HAL_GPIO_WritePin(MEM_SAVER_GPIO_Port, MEM_SAVER_Pin, GPIO_PIN_RESET);
			DEBUG_RTT(LOG_WARNING, "Mem_Saver_ST3 turned off.");	//}
//		if(flagStatus(Status, MEM_SAVER) == false)	{
			statusFlagTimeStamp[MEM_SAVER] = HAL_GetTick();
		}
	}
	if(HAL_GPIO_ReadPin(RO_SAVER_GPIO_Port, RO_SAVER_Pin) != flagStatus(Status, RESIN_SAVER))
	{
		if(flagStatus(Status, RESIN_SAVER))
		{
			HAL_GPIO_WritePin(RO_SAVER_GPIO_Port, RO_SAVER_Pin, GPIO_PIN_SET);
			DEBUG_RTT(LOG_WARNING, "RO_Saver_ST1 turned on.");
			statusOnFlagTimeStamp[RESIN_SAVER] = HAL_GetTick();
		}
		else
		{
			HAL_GPIO_WritePin(RO_SAVER_GPIO_Port, RO_SAVER_Pin, GPIO_PIN_RESET);
			DEBUG_RTT(LOG_WARNING, "RO_Saver_ST1 turned off.");	//}
//		if(flagStatus(Status, MEM_SAVER) == false)	{
			statusFlagTimeStamp[RESIN_SAVER] = HAL_GetTick();
		}
	}
	if(HAL_GPIO_ReadPin(AUTO_FLUSH_GPIO_Port, AUTO_FLUSH_Pin) != flagStatus(Status, AUTO_FLUSH))
	{
		if(flagStatus(Status, AUTO_FLUSH))
		{
			HAL_GPIO_WritePin(AUTO_FLUSH_GPIO_Port, AUTO_FLUSH_Pin, GPIO_PIN_SET);
			DEBUG_RTT(LOG_WARNING, "AUTO_FLUSH_ST4 turned on.");
			statusOnFlagTimeStamp[AUTO_FLUSH] = HAL_GetTick();
		}
		else
		{
			HAL_GPIO_WritePin(AUTO_FLUSH_GPIO_Port, AUTO_FLUSH_Pin, GPIO_PIN_RESET);
			DEBUG_RTT(LOG_WARNING, "AUTO_FLUSH_ST4 turned off.");	
			statusFlagTimeStamp[AUTO_FLUSH] = HAL_GetTick();
		}
	}
	if(HAL_GPIO_ReadPin(AUTO_FILL_GPIO_Port, AUTO_FILL_Pin) != flagStatus(Status, AUTO_FILL))
	{
		if(flagStatus(Status, AUTO_FILL))
		{
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AUTO_FILL_GPIO_Port, AUTO_FILL_Pin, GPIO_PIN_SET);
			DEBUG_RTT(LOG_WARNING, "Auto_Fill_ST5 turned on.");
			statusOnFlagTimeStamp[AUTO_FILL] = HAL_GetTick();
		}
		else
		{
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AUTO_FILL_GPIO_Port, AUTO_FILL_Pin, GPIO_PIN_RESET);
			DEBUG_RTT(LOG_WARNING, "Auto_Fill_ST5 turned off.");	//}
//		if(flagStatus(Status, MEM_SAVER) == false)	{
			statusFlagTimeStamp[AUTO_FILL] = HAL_GetTick();
		}
	}
//	if(HAL_GPIO_ReadPin(W_LED_CTRL_GPIO_Port, W_LED_CTRL_Pin) != flagStatus(Status, AUTO_FILL))	{
//		if(flagStatus(Status, AUTO_FILL))	{
//			HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port, W_LED_CTRL_Pin, GPIO_PIN_SET);
//			DEBUG_RTT("W", "W_LED turned on.");	}
//		else	{
//			HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port, W_LED_CTRL_Pin, GPIO_PIN_RESET);
//			DEBUG_RTT("W", "W_LED turned off.");	//}
////		if(flagStatus(Status, MEM_SAVER) == false)	{
//			statusFlagTimeStamp[WLED] = HAL_GetTick();
//		}
//	}

//	FlagsReg.bits.Pressure_Sensor = HAL_GPIO_ReadPin();
//	FlagsReg.bits.Flow_Meter_1 = HAL_GPIO_ReadPin();
//	FlagsReg.bits.Flow_Meter_2 = HAL_GPIO_ReadPin();
//	FlagsReg.bits.Flow_Meter_3 = HAL_GPIO_ReadPin();

}

bool FlagHandle::checkOnTime(uint8_t sensor, uint32_t elapsedTime)
{
	uint32_t elapsed = HAL_GetTick() - statusOnFlagTimeStamp[sensor];
	if ((sensor == LEVEL_SENSOR) && (flagStatus(Status, LEVEL_SENSOR) != true) && elapsed >= elapsedTime)
	{
		return true;
	}
	if ((sensor == MEM_SAVER) && (flagStatus(Status, MEM_SAVER) != true) && elapsed >= elapsedTime)
	{
		return true;
	}
	if ((sensor == AUTO_FILL) && (flagStatus(Status, AUTO_FILL) != true) && elapsed >= elapsedTime)
	{
		return true;
	}
	if ((sensor == AUTO_FLUSH) && (flagStatus(Status, AUTO_FLUSH) != true) && elapsed >= elapsedTime)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool FlagHandle::timeElapsed(uint8_t sensor, uint32_t elapsedTime)
{
	if((sensor == LEVEL_SENSOR) && (flagStatus(Status, LEVEL_SENSOR) != true))
	{
		uint32_t elapsed = HAL_GetTick()-statusFlagTimeStamp[sensor];
		if(elapsed >= elapsedTime)
		{
			return true;
		}
	}
	if(sensor == MEM_SAVER)// && flagStatus(Settings, MEM_SAVER) != true)
	{
		uint32_t elapsed = HAL_GetTick() - statusFlagTimeStamp[sensor];
//		dispdebug->displayVar(statusFlagTimeStamp[sensor], "statusFlagTimeStamp[sensor]: ");
//		dispdebug->displayVar(HAL_GetTick(), "HAL_GetTick(): ");
//		dispdebug->displayVar(elapsed, "mem_Saver elapsed: ");
		if(elapsed >= elapsedTime)
		{
			return true;
		}
	}
	if ((sensor == AUTO_FLUSH) && (flagStatus(Status, AUTO_FLUSH)))
	{
		uint32_t elapsed = HAL_GetTick() - statusFlagTimeStamp[sensor];
		if (elapsed >= elapsedTime)
		{
			return true;
		}
	}
//	if(sensor == PRESSURE_SENSOR && FlagsReg.bits.Pressure_Sensor == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == FLOW_METER_1 && FlagsReg.bits.Flow_Meter_1 == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == FLOW_METER_2 && FlagsReg.bits.Flow_Meter_2 == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}
//	if(sensor == FLOW_METER_3 && FlagsReg.bits.Flow_Meter_3 == false){
//		uint32_t elapsed = HAL_GetTick()- <insert variable here> ;
//		if(elapsed >= elapsedTime)
//		{	return true;			}
//		else
//		{	return false;			}
//	}

	return false;
}

bool FlagHandle::flagStatus(uint8_t op, uint8_t sensor)
{
	if(op == Settings)
	{
		if(sensor == FoD)							return FlagsReg.settings.FillOnDemand;
		else if(sensor == BUZZER)					return FlagsReg.settings.Buzzer;
		else if(sensor == RGB)						return FlagsReg.settings.RGB;
		else if(sensor == WLED)						return FlagsReg.settings.WLED;
		else if(sensor == MEM_SAVER)				return FlagsReg.settings.Mem_Saver;
		else if(sensor == MEM_SAVER_PAUSE)			return FlagsReg.settings.Mem_Saver_Pause;
		else if(sensor == RESIN_SAVER)				return FlagsReg.settings.Resin_Saver;
		else if(sensor == AUTO_FLUSH)				return FlagsReg.settings.Auto_Flush;
		else if(sensor == AUTO_FILL)				return FlagsReg.settings.Auto_Fill;
		else if(sensor == MAINS_TDS_THRESHOLD)		return FlagsReg.settings.mainsTdsThresh;
		else if(sensor == RO_TDS_THRESHOLD)			return FlagsReg.settings.roTdsThresh;
		else if(sensor == DI_TDS_THRESHOLD)			return FlagsReg.settings.diTdsThresh;
		else if(sensor == MAINS_TDS_TEMPERATURE)	return FlagsReg.settings.mainsTdsTemp;
		else if(sensor == RO_TDS_TEMPERATURE)		return FlagsReg.settings.roTdsTemp;
		else if(sensor == DI_TDS_TEMPERATURE)		return FlagsReg.settings.diTdsTemp;
		else if(sensor == MAINS_TDS_SHUTDOWN)		return FlagsReg.settings.mainsTdsShutdown;
		else if(sensor == RO_TDS_SHUTDOWN)			return FlagsReg.settings.roTdsShutdown;
		else if(sensor == DI_TDS_SHUTDOWN)			return FlagsReg.settings.diTdsShutdown;
		else if(sensor == BATTERY_LOW_SHUTDON)		return FlagsReg.settings.Battery_low_shutdown;
		else if(sensor == BATTERY_POWER_MONITOR)	return FlagsReg.settings.Battery_power_monitor;
	}
	else if (op == Status)
	{
		if(sensor == LEVEL_SENSOR)				return FlagsReg.status.Level_Sensor;		//ST9 pin is inverse logic?
		else if(sensor == WLED)					return FlagsReg.status.WLED;
		else if(sensor == PRESSURE_SENSOR)		return FlagsReg.status.Pressure_Sensor;
		else if(sensor == FLOW_METER_1)			return FlagsReg.status.Flow_Meter_1;
		else if(sensor == FLOW_METER_2)			return FlagsReg.status.Flow_Meter_2;
		else if(sensor == FLOW_METER_3)			return FlagsReg.status.Flow_Meter_3;
		else if(sensor == RESIN_SAVER)			return FlagsReg.status.Resin_Saver;
		else if(sensor == MEM_SAVER)			return FlagsReg.status.Mem_Saver;
		else if(sensor == AUTO_FLUSH)			return FlagsReg.status.Auto_Flush;
		else if(sensor == AUTO_FILL)			return FlagsReg.status.Auto_Fill;
		else if(sensor == PURE_CYCLE)			return FlagsReg.status.Pure_CYCLE;
		else if(sensor == BATTERY_CHARGING_DETECTED) return FlagsReg.status.Battery_charging_detected;
		else if(sensor == BATTERY_CRITICAL)		return FlagsReg.status.Battery_critical;
		else if(sensor == BATTERY_LOW)		return FlagsReg.status.Battery_low;
		else if(sensor == BATTERY_HIGH)		return FlagsReg.status.Battery_high;
		else if(sensor == BATTERY_VOLTAGE_LOW)		return FlagsReg.status.Battery_voltage_low;
		else if(sensor == BATTERY_FLAT_SHUTDOWN)		return FlagsReg.status.Battery_flat_shutdown;
		else if(sensor == BATTERY_WARNING_SPIKES)		return FlagsReg.status.Battery_warning_spikes;
		else if(sensor == BATTERY_PUMP_IGNORE_WARNING)		return FlagsReg.status.Battery_Pump_Ignore;
		else if(sensor == TRIGGER_WARNING) return FlagsReg.status.Battery_warning;
	}
//	else
		return false;
}

bool FlagHandle::setFlag(uint8_t op, uint8_t sensor, uint8_t state)
{
	// DEBUG_RTT(LOG_INFO, "Inside setFlag");
	// DEBUG_RTT('0', " operation: %d, Sensor: %d, State: %d", op, sensor, state);
	if(op == Settings)
	{
		if(sensor == FoD)							FlagsReg.settings.FillOnDemand = state;
		else if(sensor == BUZZER)					FlagsReg.settings.Buzzer = state;
		else if(sensor == RGB)						FlagsReg.settings.RGB = state;
		else if(sensor == WLED)						FlagsReg.settings.WLED = state;
		else if(sensor == RESIN_SAVER)				FlagsReg.settings.Resin_Saver = state;			//used for ro_saver_thresh
		else if(sensor == MEM_SAVER)				FlagsReg.settings.Mem_Saver = state;
		else if(sensor == MEM_SAVER_PAUSE)			FlagsReg.settings.Mem_Saver_Pause = state;
		else if(sensor == AUTO_FLUSH)				FlagsReg.settings.Auto_Flush = state;
		else if(sensor == AUTO_FILL)				FlagsReg.settings.Auto_Fill = state;
		else if(sensor == MAINS_TDS_THRESHOLD)		FlagsReg.settings.mainsTdsThresh = state;
		else if(sensor == RO_TDS_THRESHOLD)			FlagsReg.settings.roTdsThresh = state;
		else if(sensor == DI_TDS_THRESHOLD)			FlagsReg.settings.diTdsThresh = state;
		else if(sensor == MAINS_TDS_TEMPERATURE)	FlagsReg.settings.mainsTdsTemp = state;
		else if(sensor == RO_TDS_TEMPERATURE)		FlagsReg.settings.roTdsTemp = state;
		else if(sensor == DI_TDS_TEMPERATURE)		FlagsReg.settings.diTdsTemp = state;
		else if(sensor == MAINS_TDS_SHUTDOWN)		FlagsReg.settings.mainsTdsShutdown = state;
		else if(sensor == RO_TDS_SHUTDOWN)			FlagsReg.settings.roTdsShutdown = state;
		else if(sensor == DI_TDS_SHUTDOWN)			FlagsReg.settings.diTdsShutdown = state;
		else if(sensor == BATTERY_LOW_SHUTDON)		FlagsReg.settings.Battery_low_shutdown = state;
		else if(sensor == BATTERY_POWER_MONITOR)	FlagsReg.settings.Battery_power_monitor = state;
	}
	else if (op == Status)
	{
		if(sensor == LEVEL_SENSOR)			FlagsReg.status.Level_Sensor = state;
		else if(sensor == WLED)				FlagsReg.status.WLED = state;
		else if(sensor == PRESSURE_SENSOR)	FlagsReg.status.Pressure_Sensor = state;
		else if(sensor == FLOW_METER_1)		FlagsReg.status.Flow_Meter_1 = state;
		else if(sensor == FLOW_METER_2)		FlagsReg.status.Flow_Meter_2 = state;
		else if(sensor == FLOW_METER_3)		FlagsReg.status.Flow_Meter_3 = state;
		else if(sensor == RESIN_SAVER)		FlagsReg.status.Resin_Saver = state;			//used for ro_saver_thresh
		else if(sensor == MEM_SAVER)		FlagsReg.status.Mem_Saver = state;
		else if(sensor == AUTO_FLUSH)		FlagsReg.status.Auto_Flush = state;
		else if(sensor == AUTO_FILL)		FlagsReg.status.Auto_Fill = state;
		else if(sensor == PURE_CYCLE)		FlagsReg.status.Pure_CYCLE = state;
		else if(sensor == BATTERY_CHARGING_DETECTED) FlagsReg.status.Battery_charging_detected = state;
		else if(sensor == BATTERY_CRITICAL)		FlagsReg.status.Battery_critical= state;
		else if(sensor == BATTERY_LOW)		FlagsReg.status.Battery_low = state;
		else if(sensor == BATTERY_HIGH)		FlagsReg.status.Battery_high = state;
		else if(sensor == BATTERY_VOLTAGE_LOW)		FlagsReg.status.Battery_voltage_low = state;
		else if(sensor == BATTERY_FLAT_SHUTDOWN)		FlagsReg.status.Battery_flat_shutdown = state;
		else if(sensor == BATTERY_WARNING_SPIKES)		FlagsReg.status.Battery_warning_spikes = state;
		else if(sensor == BATTERY_PUMP_IGNORE_WARNING)		FlagsReg.status.Battery_Pump_Ignore = state;
		else if(sensor == TRIGGER_WARNING) FlagsReg.status.Battery_warning = state;
	}
	// DEBUG_RTT(LOG_INFO, "Exiting setFlag.");
	return false;
}

void FlagHandle::flagsInit()								//Implement flashRetrieve() here
{
//	flashRetrieve();
	setFlag(Settings, WLED, true);
	setFlag(Settings, BUZZER, true);
	setFlag(Settings, BATTERY_LOW_SHUTDON,true);
	setFlag(Settings, BATTERY_POWER_MONITOR,true);
	setFlag(Settings, MAINS_TDS_THRESHOLD, true);
	setFlag(Settings, RO_TDS_THRESHOLD, true);
	setFlag(Settings, DI_TDS_THRESHOLD, true);
	setFlag(Settings, MAINS_TDS_TEMPERATURE, true);
	setFlag(Settings, RO_TDS_TEMPERATURE, true);
	setFlag(Settings, DI_TDS_TEMPERATURE, true);
}

//void flagHandle::(){
//
//}
