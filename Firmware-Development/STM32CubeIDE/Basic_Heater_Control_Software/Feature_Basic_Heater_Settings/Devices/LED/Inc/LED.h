//---------------------------wrong implementation, have to use addressable LED for this------------------
//-------------------------------------------also make RGB class here?-----------------------------
#pragma once

#include "stm32f0xx_hal.h"
#include "main.h"
#include "_DEBUG_.h"

typedef enum
{
	PwmSTART,
	PwmSTOP
}	PwmState;

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

class RGB{
public:
	RGB(GPIO_TypeDef* port, uint16_t redPin, uint16_t greenPin, uint16_t bluePin)
	: pwmPort_(port), redPin_(redPin), greenPin_(greenPin), bluePin_(bluePin)
	{
		DEBUG_RTT(LOG_WARNING, "RGB class obj constructor called");
	}

	~RGB()
	{
		DEBUG_RTT(LOG_WARNING, "~RGB class obj destructor called");
	}

	void Run(void);
	void setRgbColor(HexColor);
	void setOutputPattern(HexColor, bool fadeOn, uint16_t durOff, uint16_t durOn, uint16_t OnTime);
	void turnOn(void);
	void turnOff(void);
	void toggle(void);
	void pwmOeration(PwmState);

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

private:

};

/* class methods
color set (r, g, b, fade, duration off (0 means infinite), duration ON, ON times)

turnOn -> start pwm
turnOff / reset parameters and stop pwm

normal toggle
infinite toggle

run (call in while loop)

enum for default colors with default mode to set specific rgb values
default fade vs customize one fast/slow (will depend on duration ON/OFF time)
*/