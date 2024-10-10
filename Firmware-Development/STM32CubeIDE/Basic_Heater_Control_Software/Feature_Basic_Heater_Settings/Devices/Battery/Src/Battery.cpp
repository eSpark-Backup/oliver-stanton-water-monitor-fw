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
long lastMessageDisplayed = 0u;
long lastStandByMessageDisplayed = 0u;
long lasttimeBatteryReading = 0u;
long lastMeasurementTaken = 0u;

long WarninigTriggerLast = 0u;
uint8_t triggers = 6;
#define NUM_READINGS 20

#define DEBUG_BATTERY_NORMAL 0
#define DEBUG_DISABLE_BATTERY_LOW_SHUTDOWN 0
#define DEBUG_BATTERY_LOW 0
#define DEBUG_FLAT_SHUTDOWN 0
#define DEBUG_WARNING_SPIKES 0
#define DEBUG_STANDBY_SCREEN 0
#define DEBUG_CHARGING_DETECTED 1

BatteryMonitor Battery;

static float BatteryReadings[NUM_READINGS]={0};
static int Battery_index = 0;
static bool charging = false;
static bool previous_charging = false;

// Function to calculate the average of an array
float calculate_average(float readings[], int size);

float BatteryMonitor::readBattery()
{
	char MessageBuffer[256];
	this->ADC_Battery_Analog = getValue(this->ADC_Channel_battery);
	calculateBattery();
	// DEBUG_RTT('0', "Battery Value %f",this->voltage);
//	memset(MessageBuffer,0,sizeof(MessageBuffer));
//	sprintf(MessageBuffer,"Battery Value %f | count %d",this->voltage,this->ADC_Battery_Analog);
//	menu.Display_Log(MessageBuffer);
	return this->voltage;
}

void BatteryMonitor::calculateBattery(void)
{
	//--have to use calculation methods here depending on our Battery voltage level --

	this->voltage = (this->ADC_Battery_Analog/MAX_VOLTAGE_COUNTS)*MAX_VOLTAGE;//this->adcRange*this->aref;
//	this->ecValue=(133.42*this->voltage*this->voltage*this->voltage - 255.86*this->voltage*this->voltage + 857.39*this->voltage);//*this->kValue;
//	this->ecValue25  =  this->ecValue / (1.0+0.02*(this->temperature-25.0));  //temperature compensation
//	this->tdsValue = ecValue25 * 1.0;  //TdsFactor;

//	this->voltage = 12.5;			//--this is just simulation for Battery voltage value
}


float calculate_average(float readings[], int size) {
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += readings[i];
    }
    return (sum / (float)size);
}

void BatteryMonitor::powerManager(void)
{
//	Switches Button5;
//	uint8_t status,flag;
	bool charging = false;
	MenuItem* Menu = (MenuItem*)currentM;
	char MessageBuffer[256]={0};
	float BatteryVoltage = readBattery();
	static float LastBatteryVoltage = BatteryVoltage;

#if DEBUG_BATTERY_NORMAL == 1
	BatteryVoltage = 12.2;
#elif DEBUG_BATTERY_LOW == 1
	BatteryVoltage = 12.0;
#elif DEBUG_FLAT_SHUTDOWN == 1
	BatteryVoltage = 10.4;
#if DEBUG_DISABLE_BATTERY_LOW_SHUTDOWN
	Flags.setFlag(Settings, BATTERY_LOW_SHUTDON
			,false);
#endif
#elif DEBUG_WARNING_SPIKES == 1
	BatteryVoltage = 13.2;
#endif




if((Monitor.isActivityDetected == false && (HAL_GetTick()-lastStandByMessageDisplayed)>1000) && (strcmp(Menu->label,DeviceStandByScreen->label)!=0 && (strcmp(Menu->label,DeviceShutDown->label)!=0)))
{

#if DEBUG_STANDBY_SCREEN == 1
	memset(MessageBuffer,0,sizeof(MessageBuffer));
	sprintf(MessageBuffer,"Going to Standby in %d Seconds",Monitor.StandByCounter);
	menu.Display_Log(MessageBuffer);
#endif
	getPumpAveragePowerConsumption(PUMP1);
	getPumpAveragePowerConsumption(PUMP2);
	if(Monitor.StandByCounter == 0)
	{
		Menu = DeviceStandByScreen;
		menu.displayMenu(Menu,  Menu->num_items);
		currentM = (int*)Menu;
		return;
	}
	lastStandByMessageDisplayed = HAL_GetTick();
	--Monitor.StandByCounter;

}
else
{
	if(Monitor.isActivityDetected == true)Monitor.StandByCounter = STANDBY_TIMEOUT_SEC;
	if(HAL_GetTick()-lastStandByMessageDisplayed>1000)
	{
		Monitor.isActivityDetected = false;
	}

}


#if DEBUG_CHARGING_DETECTED == 1

#endif


	if((strcmp(Menu->label,Battery_Index->label)==0) || (strcmp(Menu->label,DeviceStandByScreen->label)==0))
	{

		if ((BatteryVoltage - LastBatteryVoltage) > CHANGE_IN_BATTERY_VOLTAGE || (LastBatteryVoltage - BatteryVoltage) > CHANGE_IN_BATTERY_VOLTAGE)
		{
			menu.Display_Log("Updated Battery Voltage: %.2f v\r\n"
							"Controller Current: Not Available\r\n"
							"Sonar current: Not Available\r\n",
							BatteryVoltage);
			lastMessageDisplayed = HAL_GetTick();
			LastBatteryVoltage = BatteryVoltage;
		}
		if(BatteryVoltage < BATTERY_LOW_VOLTAGE && BatteryVoltage > BATTERY_CRITICAL_VOLTAGE)
		{
//			BATTERY_LOW
			//menu.Display_Log( "Setting Status for low");
			Flags.setFlag(Status, BATTERY_CRITICAL, false);
			Flags.setFlag(Status, BATTERY_LOW, true);
//			status = Status;
//			flag = BATTERY_LOW;
		}
		else if ((BatteryVoltage > BATTERY_HIGH_VOLTAGE) && (BatteryVoltage < CURRENT_SPIKES))
		{
//			menu.Display_Log( "Setting Status for high");
			Flags.setFlag(Status, BATTERY_LOW, false);
			Flags.setFlag(Status, BATTERY_HIGH, true);
//			status = Status;
//			flag = BATTERY_HIGH;
		}
		else if ((BatteryVoltage > BATTERY_HIGH_VOLTAGE))
		{
			Flags.setFlag(Status, BATTERY_LOW, false);
			Flags.setFlag(Status, BATTERY_CHARGING_DETECTED, true);
		}
		else
		{
			Flags.setFlag(Status, BATTERY_LOW, false);
			Flags.setFlag(Status, BATTERY_HIGH, false);
			Flags.setFlag(Status, BATTERY_CHARGING_DETECTED, false);
		}
//		if(charging != Monitor.isPreviouslyCharging)
//		{
//			if (charging) {
//				Flags.setFlag(Status,BATTERY_CHARGING_DETECTED,true);
//			 } else {
//				 Flags.setFlag(Status,BATTERY_CHARGING_DETECTED,false);
//				 menu.Display_Log("Charger Disconnected");
//
//			}
//			Monitor.isPreviouslyCharging = charging;
//		}

	}
	else if (strcmp(Menu->label,BatPowerConsumptiontable->label)==0)
	{
		if(HAL_GetTick()-lastMeasurementTaken>1000)
			{
				getPumpAveragePowerConsumption(PUMP1);
				getPumpAveragePowerConsumption(PUMP2);
				getTotalPowerConsumed();
				memset(MessageBuffer,0,sizeof(MessageBuffer));
				sprintf(MessageBuffer,"Pump1 Avg Power: %.2f W\n\rPump2 Avg Power: %.2f W\n\rTotal Power Consumed: %.2f kWh\n\r",Monitor.pump1_avg_power,Monitor.pump2_avg_power,Monitor.Total_Power_Consumption);
				menu.Display_Log(MessageBuffer);
				lastMeasurementTaken = HAL_GetTick();
			}


	}
	else if(strcmp(Menu->label,DeviceShutDown->label)==0)
	{
		if(BatteryVoltage < BATTERY_CRITICAL_VOLTAGE)
		{
			Flags.setFlag(Status, TRIGGER_WARNING,false
					);
			triggers = 6;
			Monitor.isKeepDeviceInShutdown = true;
		}
		else
		{
			Monitor.isKeepDeviceInShutdown = false;
		}

		return;
	}

	if (BatteryVoltage < BATTERY_CRITICAL_VOLTAGE)
	{
//		menu.Display_Log( "Setting Status for critical");
		Flags.setFlag(Status, BATTERY_CRITICAL, true);
//		status = Status;
//		flag = BATTERY_CRITICAL;
	}
	else if ((Monitor.pump1_current >= CURRENT_SPIKES || Monitor.pump2_current >= CURRENT_SPIKES) && !(Flags.flagStatus(Status, BATTERY_WARNING_SPIKES)))
	{
		Flags.setFlag(Status, BATTERY_WARNING_SPIKES, true);
		Flags.setFlag(Status, BATTERY_PUMP_IGNORE_WARNING, false);
		if(Monitor.pump1_current >= CURRENT_SPIKES)
		{
			memset(MessageBuffer,0,sizeof(MessageBuffer));
			sprintf(MessageBuffer, "Pump %s (LL.PDS.01) is drawing %.2f amps, higher than specification, service required\r\n","One",Monitor.pump1_current);
			menu.Display_Log(MessageBuffer);
		}
		else if(Monitor.pump2_current >= CURRENT_SPIKES)
		{
			memset(MessageBuffer,0,sizeof(MessageBuffer));
			sprintf(MessageBuffer, "Pump %s (LL.PDS.01) is drawing %.2f amps, higher than specification, service required\r\n","Two",Monitor.pump2_current);
			menu.Display_Log(MessageBuffer);
		}

	}
//	else if (Flags.flagStatus(Status, BATTERY_CHARGING_DETECTED))
//	{
//		menu.Display_Log("Battery Charging");
//		Flags.setFlag(Status, BATTERY_LOW, false);
//	}
//
//	else if(Flags.flagStatus(Status, BATTERY_HIGH))
//	{
//		menu.Display_Log("Battery Charged");
//		Flags.setFlag(Status, BATTERY_HIGH, false);
//	}

	if ((HAL_GetTick()-lastMessageDisplayed)>1000 )
	{
		if(Flags.flagStatus(Status, BATTERY_LOW))
		{
			memset(MessageBuffer,0,sizeof(MessageBuffer));
			sprintf(MessageBuffer,"Warning - Battery Voltage Low (%.2f v) -  System Requires a Charge\r\n",BatteryVoltage);
			menu.Display_Log(MessageBuffer);
			lastMessageDisplayed = HAL_GetTick();
		}
		else if ((((Flags.flagStatus(Status, BATTERY_CRITICAL)) && (Flags.flagStatus(Settings, BATTERY_LOW_SHUTDON))) && Monitor.isShutDownInitiated == false) )
		{
			menu.Display_Log("Battery Flat - Voltage 10.5v - Shutting System Down in (countdown 10 to 0)\r\n");
			Flags.setFlag(Status, BATTERY_FLAT_SHUTDOWN, true);
			Flags.setFlag(Settings, LOG_TO_MASTER, true);
			Monitor.isShutDownInitiated = true;
			Monitor.ShutDownCounter = 10;
			lastMessageDisplayed = HAL_GetTick();
			Flags.setFlag(Status, TRIGGER_WARNING,true);
		}
		else if((((Flags.flagStatus(Status, BATTERY_CRITICAL)) && (!Flags.flagStatus(Settings, BATTERY_LOW_SHUTDON)))))
		{
			Flags.setFlag(Status, TRIGGER_WARNING,true);
		}
		else if (Flags.flagStatus(Status, BATTERY_CHARGING_DETECTED))
		{
			Flags.setFlag(Settings, LOG_TO_MASTER, true);
			menu.Display_Log("System Battery Charging (%.2fv)", BatteryVoltage);
			((RgbDriver*)WLED1)->setRgbColor(GREEN);
			WLED1->setOutputPattern(1, 500, 500);
			lastMessageDisplayed = HAL_GetTick();
		}
	}

		if((Monitor.isShutDownInitiated == true && (HAL_GetTick()-lastShutDownTime)>1000))
		{
			memset(MessageBuffer,0,sizeof(MessageBuffer));
			sprintf(MessageBuffer,"Shutting Down in %d Seconds\r\n",Monitor.ShutDownCounter);
			menu.Display_Log(MessageBuffer);
			lastShutDownTime = HAL_GetTick();
			if(Monitor.ShutDownCounter == 0)
			{
				Menu = DeviceShutDown;
				menu.displayMenu(Menu,  Menu->num_items);
				currentM = (int*)Menu;
				Monitor.isActivityDetected = true;
				return;
			}

			Monitor.ShutDownCounter--;

		}



	if(Flags.flagStatus(Status, BATTERY_WARNING_SPIKES) && (Flags.flagStatus(Settings, BATTERY_POWER_MONITOR)))
	{
//		if((HAL_GetTick()-lastMessageDisplayed)>1000)
//						{
//			Button5.getState(BACK);
//							sprintf(MessageBuffer,"Button State %d",Button5.state());
//							menu.Display_Log(MessageBuffer);
//							lastMessageDisplayed = HAL_GetTick();
//						}
		if(!(Flags.flagStatus(Status, BATTERY_PUMP_IGNORE_WARNING))){
			if((Monitor.isBackButtonPressed) && (HAL_GetTick()-IgnoreSpikesCounter > PEAK_IGNORE_INTERVAL) ){
				IgnoreSpikesCounter = HAL_GetTick();
				Flags.setFlag(Status, BATTERY_PUMP_IGNORE_WARNING, true);

				menu.Display_Log("Ignore Peak Warning\r\n");
				Monitor.isBackButtonPressed = false;
			}
		}
		else if((HAL_GetTick()-IgnoreSpikesCounter > PEAK_IGNORE_INTERVAL) && Flags.flagStatus(Status, BATTERY_PUMP_IGNORE_WARNING))
		{
			Flags.setFlag(Status, BATTERY_WARNING_SPIKES, false);

		}
	}
//	long WarninigTriggerLast = 0u;
//	uint8_t triggers = 6;
	if(Flags.flagStatus(Status, TRIGGER_WARNING)&& ((HAL_GetTick()-WarninigTriggerLast > 1000) ) && (triggers > 0))
	{
		WarninigTriggerLast = HAL_GetTick();
		switch(triggers %2)
		{
		case 0:
			Buzzzer->setOutputPattern(1, 500, 500);
			((RgbDriver*)WLED1)->setRgbColor(RED);
			WLED1->setOutputPattern(1, 500, 500);
			menu.Display_Log("Turn On Buzzer\r\n");
			menu.Display_Log("Turn On LED\r\n");
			break;
		case 1:
			menu.Display_Log("Turn Off Buzzer\r\n");
			menu.Display_Log("Turn Off LED\r\n");
			break;
		}
		if(triggers == 0)
		{
			menu.Display_Log("Wait 30s");
			Flags.setFlag(Status, TRIGGER_WARNING,false);
		}
		triggers--;
	}
	else if(HAL_GetTick()-WarninigTriggerLast > 30000 && triggers == 0)
	{
		menu.Display_Log("30s Elapsed\r\n");
		if(Flags.flagStatus(Status, BATTERY_CRITICAL))
			Flags.setFlag(Status, TRIGGER_WARNING,true);
		triggers = 6;
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

uint16_t BatteryMonitor::getValue(uint16_t channel)
{
	uint16_t Adc_count = 0;
	ADCSelectCH(this->ADC_Channel_battery);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 100);
	Adc_count = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);
	return Adc_count;
}

float BatteryMonitor::getPumpCurrent(uint8_t pump)
{
	uint16_t Adc_counts = 0;
	float voltage,current = 0.0f;
	switch(pump)
	{
	case PUMP1:
		Adc_counts = getValue(this->ADC_VIOUT1_channel);

		break;
	case PUMP2:
		Adc_counts = getValue(this->ADC_VIOUT2_channel);
		break;
	default:
		break;
	}
	voltage = (Adc_counts / MAX_VOLTAGE_COUNTS) * ADC_REF_VOLTAGE;
	current = (voltage - ACS723_ZERO_CURRENT_VOLTAGE) / ACS723_SENSITIVITY;
	return current;
}


void BatteryMonitor::getPumpAveragePowerConsumption(uint8_t pump)
{
	float current;
	float voltage;
	float power;
	voltage = readBattery();
	switch(pump)
	{
	case PUMP1:
		current = getPumpCurrent(PUMP1);
		Monitor.pump1_current = current;
		power = voltage * current;
		Monitor.pump1_total_power += power;
		Monitor.pump1_avg_power = Monitor.pump1_total_power/Monitor.pump1_Readings;
		Monitor.pump1_Readings++;
		break;
	case PUMP2:
		current = getPumpCurrent(PUMP2);
		Monitor.pump2_current = current;
		power = voltage * current;
		Monitor.pump2_total_power += power;
		Monitor.pump2_avg_power = Monitor.pump2_total_power/Monitor.pump2_Readings;
		Monitor.pump2_Readings++;
		break;
	}

}

void BatteryMonitor::getTotalPowerConsumed()
{
	Monitor.Total_Power_Consumption += ((Monitor.pump1_total_power + Monitor.pump2_total_power)/1000.0)*(HAL_GetTick()/3600000.0);

}

