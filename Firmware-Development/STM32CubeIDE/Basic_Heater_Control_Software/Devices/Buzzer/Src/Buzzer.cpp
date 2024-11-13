#include "Buzzer.hpp"

void BuzzerDriver::setupInit(void)
{
	// Initialize GPIO pin

}


void BuzzerDriver::setOutput(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}


void BuzzerDriver::stopOutput(void)
{
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

