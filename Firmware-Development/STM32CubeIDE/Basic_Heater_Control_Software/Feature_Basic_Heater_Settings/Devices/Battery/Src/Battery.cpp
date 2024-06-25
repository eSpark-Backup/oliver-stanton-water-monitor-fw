/*
 * Battery.cpp
 *
 *  Created on: May 31, 2024
 *      Author:
 */


#include "menu.h"
#include "Battery.hpp"
#include "TDS.hpp"
#include "flagHandle.h"

long IgnoreSpikesCounter = 0u;
long lastShutDownTime = 0u;

BatteryMonitor Battery;

float BatteryMonitor::readBattery()
{
	getValue();
	calculateBattery();
	DEBUG_RTT('0', "Battery Value %f",this->voltage);
	return this->voltage;
}

void BatteryMonitor::calculateBattery(void)
{
	//--have to use calculation methods here depending on our Battery voltage level --

	this->voltage = this->ADC_Battery_Analog/4096*MAX_VOLTAGE;//this->adcRange*this->aref;
//	this->ecValue=(133.42*this->voltage*this->voltage*this->voltage - 255.86*this->voltage*this->voltage + 857.39*this->voltage);//*this->kValue;
//	this->ecValue25  =  this->ecValue / (1.0+0.02*(this->temperature-25.0));  //temperature compensation
//	this->tdsValue = ecValue25 * 1.0;  //TdsFactor;

	this->voltage = 12.5;			//--this is just simulation for Battery voltage value
}

void BatteryMonitor::powerManager(void)
{
	Switches Button5;

	MenuItem* Menu = (MenuItem*)currentM;
	float BatteryVoltage = readBattery();
	char MessageBuffer[50]={0};
	if((strcmp(Menu->label,Battery_Index->label)==0) || (strcmp(Menu->label,DeviceStandByScreen->label)==0))
	{
		if (BatteryVoltage < BATTERY_CRITICAL_VOLTAGE)
		{
			Flags.setFlag(Status, BATTERY_CRITICAL, true);
		}
		else if(BatteryVoltage < BATTERY_LOW_VOLTAGE)
		{
//			BATTERY_LOW
			Flags.setFlag(Status, BATTERY_CRITICAL, false);
			Flags.setFlag(Status, BATTERY_LOW, true);
		}
		else if ((BatteryVoltage > BATTERY_HIGH_VOLTAGE) && (BatteryVoltage < CURRENT_SPIKES))
		{
			Flags.setFlag(Status, BATTERY_LOW, false);
			Flags.setFlag(Status, BATTERY_HIGH, true);
		}
		else if (BatteryVoltage > CURRENT_SPIKES)
		{
			Flags.setFlag(Status, BATTERY_WARNING_SPIKES, true);
			menu.Display_Log( "Pump One (LL.PDS.01) is drawing 13.2 amps, higher than specification, service required");
		}
	}


	if(Flags.flagStatus(Status, BATTERY_LOW))
	{
		sprintf(MessageBuffer,"Warning - Battery Voltage Low (%.2f) -  System Requires a Charge",BatteryVoltage);
		menu.Display_Log(MessageBuffer);
	}
	else if ((Flags.flagStatus(Status, BATTERY_CRITICAL)) && (Flags.flagStatus(Settings, BATTERY_LOW_SHUTDON)) && Monitor.isShutDownInitiated == false)
	{
		menu.Display_Log("Battery Flat - Voltage 10.5v - Shutting System Down in (countdown 10 to 0)");
		Flags.setFlag(Status, BATTERY_FLAT_SHUTDOWN, true);
		Flags.setFlag(Settings, LOG_TO_MASTER, true);
		Monitor.isShutDownInitiated = true;
		Monitor.ShutDownCounter = 10;
	}

	if((Monitor.isShutDownInitiated == true && (HAL_GetTick()-lastShutDownTime)>1000))
	{
		memset(MessageBuffer,0,sizeof(MessageBuffer));
		sprintf(MessageBuffer,"Shutting Down in %d Seconds",Monitor.ShutDownCounter);
		menu.Display_Log(MessageBuffer);
		lastShutDownTime = HAL_GetTick();
		Monitor.ShutDownCounter--;
	}


	if(Flags.flagStatus(Status, BATTERY_WARNING_SPIKES))
	{
		if(!(Flags.flagStatus(Status, BATTERY_PUMP_IGNORE_WARNING))){
			if((Button5.state()) && (HAL_GetTick()-IgnoreSpikesCounter > 10000) ){
				IgnoreSpikesCounter = HAL_GetTick();
				Flags.setFlag(Status, BATTERY_PUMP_IGNORE_WARNING, true);
			}
		}
	}
}
//void BatteryMonitor::ADCSelectCH(uint32_t CH)
//{
//	ADC_ChannelConfTypeDef sConfig = {0};
////	DEBUG_RTT("W", "selected channel is %d", CH);
//
//	  hadc.Instance = ADC1;
//	  if (HAL_ADC_Init(&hadc) != HAL_OK)
//	  {
//		DEBUG_RTT(LOG_ERROR, "ADC ERROR");
//		Error_Handler();
//	  }
//
//	  // Deconfigure all channels
//	  for (int i = 0; i < 18; ++i)
//	  {
//	    sConfig.Channel = i;
//	    sConfig.Rank = ADC_RANK_NONE;
//	    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
//	    {
//	      DEBUG_RTT(LOG_ERROR, "ADC CH %d de-initialization failed", i);
//	      Error_Handler();
//	    }
//	  }
//
//	  // Configure the desired channel
//	  sConfig.Channel = CH;
//	  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
//	  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//
//	  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
//	  {
//		  DEBUG_RTT(LOG_ERROR, "ADC CH %d initialization failed", CH);
//	      Error_Handler();
//	  }
//}

void BatteryMonitor::getValue()
{
	ADCSelectCH(this->ADC_Channel_battery);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 100);
	this->ADC_Battery_Analog = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);

}

