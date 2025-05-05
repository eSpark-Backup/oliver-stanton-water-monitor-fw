/*
 * PS.cpp
 *
 *  Created on: May 2nd, 2025
 *      Author: Nehal Abdul Rehman
 */


#include "PS.hpp"

int PS::readPS()
{
	getPSValue();
	calculatePS();
	// if (psValue > MIN_PS_PPM && psValue < MAX_PS_PPM)
	return (int)this->psValue;
}

void PS::resetPS(void)
{
	this->psValue = MIN_PS_PPM;
}

void PS::calculatePS(void)
{
	const int adcMin = 819;      // Corresponds to 0.6V (0 PSI)
    const int adcMax = 4095;     // Corresponds to 3.0V (100 PSI)

    if (this->analogValueAN0 < adcMin) this->analogValueAN0 = -1;
    if (this->analogValueAN0 > adcMax) this->analogValueAN0 = adcMax;

    this->psValue = ((float)(this->analogValueAN0 - adcMin) / (adcMax - adcMin)) * MAX_PS_PPM;
	// DEBUG_RTT(LOG_INFO, "Pressure Sensor : psValue: %d analog %f psi***", (int)psValue, analogValueAN0);
}

void PS::psADCSelectCH(uint32_t CH)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	// DEBUG_RTT(LOG_WARNING, "selected channel is %d", CH);

	  hadc.Instance = ADC1;
	  hadc.Init.SamplingTimeCommon = ADC_SAMPLETIME_1CYCLE_5;
	  if (HAL_ADC_Init(&hadc) != HAL_OK)
	  {
		DEBUG_RTT(LOG_ERROR, "ADC ERROR");
		Error_Handler();
	  }

	  // Deconfigure all channels
	  for (int i = 0; i < 18; ++i)
	  {
	    sConfig.Channel = i;
	    sConfig.Rank = ADC_RANK_NONE;
	    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	    {
	      DEBUG_RTT(LOG_ERROR, "ADC CH %d de-initialization failed", i);
	      Error_Handler();
	    }
	  }

	  // Configure the desired channel
	  sConfig.Channel = CH;
	  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

	  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	  {
		  DEBUG_RTT(LOG_ERROR, "ADC CH %d initialization failed", CH);
	      Error_Handler();
	  }
}

void PS::getPSValue()
{
	HAL_GPIO_WritePin(this->config._port, this->config._pin, GPIO_PIN_SET);
	psADCSelectCH(this->config.AN_pin_Channel);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 100);
	this->analogValueAN0 = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);
	HAL_GPIO_WritePin(this->config._port, this->config._pin, GPIO_PIN_RESET);
}

