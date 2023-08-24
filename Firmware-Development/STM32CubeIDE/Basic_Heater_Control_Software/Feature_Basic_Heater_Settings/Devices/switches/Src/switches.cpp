
#include "switches.hpp"
#include "stm32f0xx_hal.h"
#include "main.h"

bool Switches::getState(SWITCH SW)
{
	 bool ret=0;
	 switch(SW)
	 {
	 case SW2:
		 ret = HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin);
		 break;
//	 case SW3:
//		 ret = HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin);
//		 break;
	 case SW4:
	 	 ret = HAL_GPIO_ReadPin(SW4_GPIO_Port, SW4_Pin);
	 	 break;
	 case SW5:
	 	 ret = HAL_GPIO_ReadPin(SW5_GPIO_Port, SW5_Pin);
	 	 break;
	 case SW6:
	 	 ret = HAL_GPIO_ReadPin(SW6_GPIO_Port, SW6_Pin);
	 	 break;
//	 case SW7:
//	 	 ret = HAL_GPIO_ReadPin(SW7_GPIO_Port, SW7_Pin);
//	 	 break;
	 default:
		 break;
	 }
	 return ret;
}



//
//#include "switches.hpp"
//
//
//bool Switches::getState(GPIO_TypeDef * GPIO_Port, uint16_t GPIO_Pin)
//{
//		return HAL_GPIO_ReadPin(GPIO_Port, GPIO_Pin);
//}
//
//void Switches::setState(GPIO_TypeDef * GPIO_Port, uint16_t GPIO_Pin,GPIO_PinState state)
//{
//		HAL_GPIO_WritePin(GPIO_Port,GPIO_Pin, state);
//}
