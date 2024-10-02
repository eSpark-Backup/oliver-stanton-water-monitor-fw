/*
 * TDS.cpp
 *
 *  Created on: Jan 4, 2024
 *      Author: Farhan Ahmed
 */



#include "TDS.hpp"

float TDS::readTDS()
{
	getValue();
	calculateTDS();
	DEBUG_RTT('0', "TDS Value %f",this->tdsValue);
	return this->tdsValue;
}

void TDS::calculateTDS(void)
{
	//--have to use calculation methods here depending on our TDS sensor/probe--

//	this->voltage = this->analogValueAN1/4096*3.3;//this->adcRange*this->aref;
//	this->ecValue=(133.42*this->voltage*this->voltage*this->voltage - 255.86*this->voltage*this->voltage + 857.39*this->voltage);//*this->kValue;
//	this->ecValue25  =  this->ecValue / (1.0+0.02*(this->temperature-25.0));  //temperature compensation
//	this->tdsValue = ecValue25 * 1.0;  //TdsFactor;

	this->tdsValue -= rand()%4;			//--this is just simulation for decreasing TDS value
}

void ADCSelectCH(uint32_t CH)
{
	ADC_ChannelConfTypeDef sConfig = {0};
//	DEBUG_RTT("W", "selected channel is %d", CH);

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

void TDS::getValue()
{
	HAL_GPIO_WritePin(this->config.T1_port, this->config.T1_pin, GPIO_PIN_SET);
	ADCSelectCH(this->config.AN1_pin_Channel);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 100);
	this->analogValueAN1 = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);

	ADCSelectCH(this->config.AN2_pin_Channel);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 100);
	this->analogValueAN2 = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);
	HAL_GPIO_WritePin(this->config.T1_port, this->config.T1_pin, GPIO_PIN_RESET);

}

