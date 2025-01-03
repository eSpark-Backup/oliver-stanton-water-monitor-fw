/*
 * TDS.hpp
 *
 *  Created on: Jan 4, 2024
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef TDS_INC_TDS_HPP_
#define TDS_INC_TDS_HPP_

#include "_DEBUG_.h"
#include "Adc_select.h"
#include "mainApp.hpp"

#define TDS_DUMMY_VALUE 1000
#define MAX_TDS_PPM 999
#define TEMP_DUMMY_VALUE 100
#define MAX_TEMP_C 99

/* The Physical header of TDS probes are mentioned below
 *
 * 		-	3.3				//3.3v power to daughter board
 * 		-	AN1				//Main analog read pin, read this pin to find out the TDS value
 * 		-	T2				//Calibration pin, give high on this pin to receive max value on AN1 pin
 * 		-	T1				//TDS enable pin?,	connected directly to one of the probes, enable this pin to get reading on AN1
 * 		-	AN2				//Analog read pin of the probe's second pin (or) maybe it can be used for calibration
 * 		-	GND				//GND
 */

/* As per Renesas [ RL78/G11 ] TDS Water Quality Tester
 *
 * 		-	VDD				//3.3v power
 * 		-	AN0				//Temperature out
 * 		-	T3				//Tester probe
 * 		-	T4				//Tester probe
 * 		-	AN2				//TDS out
 * 		-	GND				//GND
 */

struct GPIOConfig {
//	GPIO_TypeDef* AN1_port;
	GPIO_TypeDef* T2_port;
	GPIO_TypeDef* T1_port;
//	GPIO_TypeDef* AN2_port;
	uint16_t AN1_pin_Channel;
	uint16_t T2_pin;
	uint16_t T1_pin;
	uint16_t AN2_pin_Channel;
};

//GPIOConfig tds1PinConfig;
//GPIOConfig tds2PinConfig;
//GPIOConfig tds3PinConfig;
//GPIOConfig tds4PinConfig;



class TDS{
private:
//	GPIO_TypeDef* AN1_port;
//	GPIO_TypeDef* T2_port;
//	GPIO_TypeDef* T1_port;
//	GPIO_TypeDef* AN2_port;
//	uint16_t AN1;
//	uint16_t T2;
//	uint16_t T1;
//	uint16_t AN2;

	GPIOConfig config;

	float voltage;
    float ecValue; //before temperature compensation
    float ecValue25; //after temperature compensation
    float temperature = 25.0;

    float analogValueAN1;
    float analogValueAN2;
    float tdsValue = TDS_DUMMY_VALUE;
    float tempValue = TEMP_DUMMY_VALUE;
//    float aref = 3.3;				//voltage reference, 3.3v
//    float adcRange = 4096;			//12bit ADC
//    float kValue;      // k value of the probe,you can calibrate in buffer solution ,such as 706.5ppm(1413us/cm)@25^C



	void calculateTDS(void);
	void calculateTemp(void);
	friend void ADCSelectCH(uint32_t);
	void getTDSValue(void);
	void getTempValue(void);
public:
	TDS(const GPIOConfig& config) : config(config) {
	        // Constructor to initialize the GPIO port and pin
	        // You may add more parameters as needed for your use case
		DEBUG_RTT(LOG_WARNING, "TDS obj constructor called.");
	}
	~TDS(){
		DEBUG_RTT(LOG_WARNING, "TDS obj destructor called.");
	};


	int readTDS(void);
	int readTemp(void);
	void resetTDS(void);
protected:


};



#endif /* TDS_INC_TDS_HPP_ */
