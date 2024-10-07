//---------------------------wrong implementation, have to use addressable LED for this------------------
//-------------------------------------------also make RGB class here?-----------------------------
#pragma once

#include "stm32f0xx_hal.h"
#include "main.h"
#include "_DEBUG_.h"

typedef enum{
	START,
	STOP
}PWMSTATE;
typedef enum{
 	RED     = 0xFF0000,  // Red color in hex
    GREEN   = 0x00FF00,  // Green color in hex
    BLUE    = 0x0000FF,  // Blue color in hex
    YELLOW  = 0xFFFF00,  // Yellow color (Red + Green)
	ORANGE  = 0xFFA500,
    CYAN    = 0x00FFFF,  // Cyan color (Green + Blue)
    MAGENTA = 0xFF00FF,  // Magenta color (Red + Blue)
	PURPLE  = 0x800080,
    WHITE   = 0xFFFFFF,  // White color (Red + Green + Blue)
    BLACK   = 0x000000   // Black color (All off)
}RGBCOLOR;

class RGB{
public:
	RGB(GPIO_TypeDef* port, uint16_t redPin, uint16_t greenPin, uint16_t bluePin)
	: pwmPort_(port), redPin_(redPin), greenPin_(greenPin), bluePin_(bluePin)
	{
		//RGB pins PWM GPIO initialization
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = redPin_ | greenPin_ | bluePin_ ;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(pwmPort_, &GPIO_InitStruct);

		DEBUG_RTT(LOG_WARNING, "RGB class obj constructor called");
	}

	~RGB()
	{
		DEBUG_RTT(LOG_WARNING, "~RGB class obj destructor called");
	}

	void Run(void);
	void setRgbColor(RGBCOLOR);
	void rgbLed(RGBCOLOR, bool fadeOn, uint16_t durOff, uint16_t durOn, uint16_t OnTime);
	void turnOn(void);
	void turnOff(void);
	void toggle(void);
	void pwmOeration(PWMSTATE);

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