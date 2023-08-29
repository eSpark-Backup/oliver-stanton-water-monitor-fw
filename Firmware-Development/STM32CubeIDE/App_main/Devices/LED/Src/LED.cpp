#include "LED.h"

void W_LED::set()
{
	HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_SET);
}
void W_LED::reset()
{
	HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_RESET);
}
