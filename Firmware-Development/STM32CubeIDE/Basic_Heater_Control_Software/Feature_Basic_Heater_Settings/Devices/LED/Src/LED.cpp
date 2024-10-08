#include "LED.h"


void RGB::setRgbColor(HexColor color)
{
    // Extract Red, Green, and Blue values from the color enum (hex value)
    int16_t r = (color >> 16) & 0xFF;  // Extract Red (8 bits)
    int16_t g = (color >> 8) & 0xFF;   // Extract Green (8 bits)
    int16_t b = color & 0xFF;          // Extract Blue (8 bits)

    // Ensure brightness values are within the correct range (0 to 255)
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

	rgbColor[0] = r; rgbColor[1] = g; rgbColor[2] = b;

}


void RGB::turnOn(void)
{

}

void RGB::turnOff(void)
{

}

void RGB::pwmOeration(PwmState Action)
{
	if(Action == PwmSTART)
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	}
	if(Action == PwmSTOP)
	{	
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	}
}

void RGB::toggle(void)
{
	uint32_t currentTime = HAL_GetTick();
	if(durationTime[0] == 0 && durationTime[1] == 0 && durationTime[2] == 0)
	{	
		curState = !curState;
	}
	else if (onTimeCount_ < durationTime[2] + 1)
	{

	}
	else
	{
		turnOff();
	}

		pwmOeration(curState ? PwmSTART : PwmSTOP);

}


void RGB::Run(void)
{
	if(isOn_)
	{
        toggle();
	}
	else
	{
		DEBUG_RTT(LOG_ERROR, "RGB Led operation is OFF");
	}
}


void RGB::setOutputPattern(HexColor color, bool fadeOn, uint16_t durOff, uint16_t durOn, uint16_t onTime)
{
	setRgbColor(color);
	durationTime[0] = durOff; durationTime[1] = durOn; durationTime[2] = onTime;

	
	onTimeCount_ = 0;
	isOn_ = true;
}
