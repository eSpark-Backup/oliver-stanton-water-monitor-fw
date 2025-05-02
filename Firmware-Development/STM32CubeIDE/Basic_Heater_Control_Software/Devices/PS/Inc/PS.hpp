/*
 * PS.hpp
 *
 *  Created on: Jan 4, 2024
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef _PS_HPP_
#define _PS_HPP_

#include "_DEBUG_.h"
#include "Adc_select.h"
#include "mainApp.hpp"

#define MAX_PS_PPM 100.0
#define MIN_PS_PPM 0.0

struct psGPIOConfig {
	GPIO_TypeDef* _port;
	uint16_t AN_pin_Channel;
	uint16_t _pin;
};

class PS {

private:
	psGPIOConfig config;
    float analogValueAN0;
    float psValue = MIN_PS_PPM;

	void calculatePS(void);
	void psADCSelectCH(uint32_t);
	void getPSValue(void);

public:
	PS(const psGPIOConfig& config) : config(config) {
		DEBUG_RTT(LOG_WARNING, "PS class obj constructor called.");
	}
	~PS(){
		DEBUG_RTT(LOG_WARNING, "PS class obj destructor called.");
	};

	int readPS(void);
	void resetPS(void);
};



#endif /* PS_INC_PS_HPP_ */
