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
