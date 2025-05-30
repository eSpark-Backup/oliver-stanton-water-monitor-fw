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
    BLACK   = 0x000000,

	STATUS_BLUE = 0x0FA6D1
}	HexColor;

class RgbDriver : public EventHandle{
public:
	RgbDriver(GPIO_TypeDef* port, uint16_t redPin, uint16_t greenPin, uint16_t bluePin, IndicatorType indicatorType)
	: EventHandle(indicatorType), pwmPort_(port), redPin_(redPin), greenPin_(greenPin), bluePin_(bluePin)
	{
		DEBUG_RTT(LOG_WARNING, "RgbDriver class obj constructor called");
	}

	~RgbDriver()
	{
		DEBUG_RTT(LOG_WARNING, "~RgbDriver class obj destructor called");
	}

	void setupInit(void) override;
	void setRgbColor(HexColor);

private:
	static void setRgbLedGpio(TIM_HandleTypeDef *htim);
	static void resetRgbLedGpio(TIM_HandleTypeDef *htim);
	void setOutput(void) override;
	void stopOutput(void) override;

private:
	GPIO_TypeDef* pwmPort_;
	uint16_t redPin_;
	uint16_t greenPin_;
	uint16_t bluePin_;
	uint16_t rgbColor[3] = {0};
};


#endif /* _LED_H_ */