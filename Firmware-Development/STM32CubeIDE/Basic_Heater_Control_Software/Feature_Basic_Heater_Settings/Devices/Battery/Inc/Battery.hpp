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
#include "controls.h"
#include "switches.hpp"

extern BatteryMonitor Battery;

#define TDS_DUMMY_VALUE 18000
#define BATTERY_CRITICAL_VOLTAGE 10.5
#define BATTERY_LOW_VOLTAGE 12.1
#define BATTERY_HIGH_VOLTAGE 13.0
#define CURRENT_SPIKES 13.2

#define MAX_VOLTAGE 14.0

struct BatteryMonitoring
{
	bool isShutDownInitiated;
	uint8_t ShutDownCounter;
	uint8_t Index;
	float input_voltage;
	float input_current;
	float output_voltage;
	float output_current;
};


class BatteryMonitor{
private:
	 uint16_t ADC_Channel_battery = ADC_CHANNEL_7;
	uint16_t ADC_Battery_Analog;

	float voltage;

    float tdsValue = TDS_DUMMY_VALUE;
    BatteryMonitoring Monitor;

	void calculateBattery(void);
	friend void ADCSelectCH(uint32_t);  // Declare friend function
	void getValue(void);

public:
	BatteryMonitor(){
	        // Constructor to initialize the GPIO port and pin
	        // You may add more parameters as needed for your use case
		DEBUG_RTT(LOG_WARNING, "BatteryMonitor obj constructor called.");
	}
	~BatteryMonitor(){
		DEBUG_RTT(LOG_WARNING, "BatteryMonitor obj destructor called.");
	};

	void powerManager(void);
	float readBattery(void);
protected:


};



#endif /* BATTERY_IGNITION_MONITOR_HPP_ */
