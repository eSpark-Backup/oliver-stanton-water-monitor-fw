#include "LED.h"

void WLED::set()
{
	HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_SET);
}
void WLED::reset()
{
	HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_RESET);
}
