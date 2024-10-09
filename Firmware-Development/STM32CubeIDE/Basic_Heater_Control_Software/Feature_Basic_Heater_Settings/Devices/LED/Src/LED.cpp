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

	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, rgbColor[0]);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, rgbColor[1]);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, rgbColor[2]);

}


void RGB::setupInit(void)
{
	// Initialize GPIO pin

}


void RGB::setOutput(void)
{
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_4);
}


void RGB::stopOutput(void)
{
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_4);
}
