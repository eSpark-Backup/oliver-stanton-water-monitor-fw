//---------------------------wrong implementation, have to use addressable LED for this------------------
//-------------------------------------------also make RGB class here?-----------------------------
#pragma once

#include "stm32f0xx_hal.h"
#include "main.h"

class WLED{
public:
	void set();
	void reset();
};

class RGB{
public:
	RGB(GPIO_TypeDef* port, uint16_t redPin, uint16_t greenPin, uint16_t bluePin){
		
	}
};
