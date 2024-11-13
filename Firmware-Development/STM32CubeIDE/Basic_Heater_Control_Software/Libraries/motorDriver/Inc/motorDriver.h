#pragma once
#include "stm32f0xx_hal.h"
class MotorDriver{
private:
	bool pumpConnectivityFlag;
	double voltage = 0;
	double current = 0;
	double sensitivity = 0.2 ; // volts/Amp
	float vRefPB1=0;
public:
	MotorDriver();
	void enablePump1();
	void enablePump2();
	void disablePump1();
	void disablePump2();
	void enableDT1();
	void disableDT1();
//	void pumpConnectivityCallback(TIM_HandleTypeDef* htim);
	float getCurrent();
	void enable();
	void setFlag(bool );
	bool getFlag();
	void setZeroReference();

};





