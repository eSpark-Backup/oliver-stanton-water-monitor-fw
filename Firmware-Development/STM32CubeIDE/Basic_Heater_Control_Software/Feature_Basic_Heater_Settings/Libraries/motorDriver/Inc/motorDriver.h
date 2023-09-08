#pragma once
#include "stm32f0xx_hal.h"
class MotorDriver{
public:
	void enablePump1();
	void enablePump2();
	void disablePump1();
	void disablePump2();
	void enableDT1();
	void disableDT1();
	void pumpConnectivityCallback(TIM_HandleTypeDef* htim);
	float getCurrent();
	void enablePumps();
};





