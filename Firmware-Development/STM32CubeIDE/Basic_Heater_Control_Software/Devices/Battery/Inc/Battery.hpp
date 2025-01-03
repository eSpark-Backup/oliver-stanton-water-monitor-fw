/*
 * TDS.hpp
 *
 *  Created on: Jan 4, 2024
 *      Author: Ahtasham Baig
 */
#pragma once

#ifndef BATTERY_IGNITION_MONITOR_HPP_
#define BATTERY_IGNITION_MONITOR_HPP_

#include "_DEBUG_.h"
#include "Adc_select.h"
#include "mainApp.hpp"
#include "controlsequence.h"
#include "switches.hpp"

extern BatteryMonitor Battery;
extern EventHandle* Buzzzer;
extern EventHandle* WLED1;

#define TDS_DUMMY_VALUE 18000

#define CHANGE_IN_BATTERY_VOLTAGE 0.05
#define BATTERY_CRITICAL_VOLTAGE 10.5
#define BATTERY_LOW_VOLTAGE 12.1
#define BATTERY_HIGH_VOLTAGE 13.0
#define CURRENT_SPIKES 13.1


#define PEAK_IGNORE_INTERVAL 10000 // equals to 10min

#define MAX_VOLTAGE 16.0
#define MAX_VOLTAGE_COUNTS 4096.0

#define STANDBY_TIMEOUT_SEC 30

#define ADC_REF_VOLTAGE 5.0

// ACS723LLCTR-05AB specific constants
#define ACS723_ZERO_CURRENT_VOLTAGE (ADC_REF_VOLTAGE / 2.0)
#define ACS723_SENSITIVITY (0.4) // V/A (for the 05AB variant)

enum PUMPS{
	PUMP1,
	PUMP2
};

struct BatteryMonitoring
{
	bool isShutDownInitiated;
	bool isActivityDetected;
	bool isKeepDeviceInShutdown;
	bool isPumpDrawPeaksScreenShown;
	bool isBackButtonPressed;
	bool isPreviouslyCharging;
	uint8_t StandByCounter;
	uint8_t ShutDownCounter;
	uint32_t batteryVoltageCounts;
	uint32_t pump1_Readings;
	uint32_t pump2_Readings;
	float avg_battery_voltage;
	float pump1_current;
	float pump2_current;
	float pump1_avg_power;
	float pump2_avg_power;
	float pump1_total_power;
	float pump2_total_power;
	float Total_Power_Consumption;
};


class BatteryMonitor{
private:
	uint16_t ADC_Channel_battery = ADC_CHANNEL_7;
	uint16_t ADC_Battery_Analog;
	uint16_t ADC_VIOUT1_channel = ADC_CHANNEL_8;
	uint16_t ADC_VIOUT2_channel = ADC_CHANNEL_9;
	float voltage;

    float tdsValue = TDS_DUMMY_VALUE;
    BatteryMonitoring Monitor;


	void calculateBattery(void);
	friend void ADCSelectCH(uint32_t);  // Declare friend function
	uint16_t getValue(uint16_t channel);

	float getPumpCurrent(uint8_t pump);
	void getPumpAveragePowerConsumption(uint8_t pump);
	void getTotalPowerConsumed();

public:
	BatteryMonitor(){
	        // Constructor to initialize the GPIO port and pin
	        // You may add more parameters as needed for your use case
		DEBUG_RTT(LOG_WARNING, "BatteryMonitor obj constructor called.");
		Monitor.StandByCounter = STANDBY_TIMEOUT_SEC;
		Monitor.ShutDownCounter = 10;
	}
	~BatteryMonitor(){
		DEBUG_RTT(LOG_WARNING, "BatteryMonitor obj destructor called.");
	};

	void powerManager(void);
	float readBattery(void);
	// Public getter and setter for Monitor
	    BatteryMonitoring& getMonitor() {
	        return Monitor;
	    }
protected:


};



#endif /* BATTERY_IGNITION_MONITOR_HPP_ */
