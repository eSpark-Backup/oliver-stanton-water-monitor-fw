
#include "switches.hpp"
#include "stm32f0xx_hal.h"

bool Switches::getState(GPIO_TypeDef * GPIO_Port, uint16_t GPIO_Pin)
{
		return HAL_GPIO_ReadPin(GPIO_Port, GPIO_Pin);
}

void Switches::setState(GPIO_TypeDef * GPIO_Port, uint16_t GPIO_Pin,GPIO_PinState state)
{
		HAL_GPIO_WritePin(GPIO_Port,GPIO_Pin, state);
}
