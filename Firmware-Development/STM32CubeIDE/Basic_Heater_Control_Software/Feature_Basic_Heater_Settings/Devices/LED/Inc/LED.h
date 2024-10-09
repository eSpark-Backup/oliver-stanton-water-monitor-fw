//---------------------------wrong implementation, have to use addressable LED for this------------------
//-------------------------------------------also make RGB class here?-----------------------------
#pragma once


#ifndef _LED_H_
#define _LED_H_

#include "stm32f0xx_hal.h"
#include "main.h"
#include "eventHandle.hpp"


typedef enum
{
 	RED     = 0xFF0000,  // 0xRRGGBB
    GREEN   = 0x00FF00,
    BLUE    = 0x0000FF,
    YELLOW  = 0xFFFF00,
	ORANGE  = 0xFFA500,
    CYAN    = 0x00FFFF,
    MAGENTA = 0xFF00FF,
	PURPLE  = 0x800080,
    WHITE   = 0xFFFFFF,
    BLACK   = 0x000000
}	HexColor;

class RGB : public EventHandle{
public:
	RGB(GPIO_TypeDef* port, uint16_t redPin, uint16_t greenPin, uint16_t bluePin, IndicatorType indicatorType)
	: EventHandle(indicatorType), pwmPort_(port), redPin_(redPin), greenPin_(greenPin), bluePin_(bluePin)
	{
		DEBUG_RTT(LOG_WARNING, "RGB class obj constructor called");
	}

	~RGB()
	{
		DEBUG_RTT(LOG_WARNING, "~RGB class obj destructor called");
	}

	void setRgbColor(HexColor);

private:
	void setupInit(void) override;
	void setOutput(void) override;
	void stopOutput(void) override;

private:
	GPIO_TypeDef* pwmPort_;
	uint16_t redPin_;
	uint16_t greenPin_;
	uint16_t bluePin_;
	uint8_t rgbColor[3] = {0};

	bool isOn_ = false;
	bool curState = false;
	uint16_t onTimeCount_ = 0;
	uint16_t durationTime[3] = {0};


};


#endif /* _LED_H_ */