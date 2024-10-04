//---------------------------wrong implementation, have to use addressable LED for this------------------
//-------------------------------------------also make RGB class here?-----------------------------
#pragma once

#include "stm32f0xx_hal.h"
#include "main.h"
#include "_DEBUG_.h"

class WLED{
public:
	void set();
	void reset();
};

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

		DEBUG_RTT(LOG_WARNING, "EventHandle obj constructor called");
	}

	~RGB()
	{
		DEBUG_RTT(LOG_WARNING, "~EventHandle obj destructor called");
	}

	void setRgbColor(uint8_t r,uint8_t g,uint8_t b);
	void setPwmColor(void);
	void fadeTo(uint8_t r, uint8_t g, uint8_t b, uint32_t duration);
    void updateFade(void);

private:
	GPIO_TypeDef* pwmPort_;
	uint16_t redPin_;
	uint16_t greenPin_;
	uint16_t bluePin_;
	uint8_t rgbColor[3];


};
