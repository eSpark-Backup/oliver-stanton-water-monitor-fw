#include "LED.h"

void WLED::set()
{
	HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_SET);
}
void WLED::reset()
{
	HAL_GPIO_WritePin(W_LED_CTRL_GPIO_Port,W_LED_CTRL_Pin,GPIO_PIN_RESET);
}


void RGB::setRgbColor(uint8_t r,uint8_t g,uint8_t b)
{
	rgbColor[0] = r; rgbColor[1] = g; rgbColor[2] = b;

}

void RGB::setPwmColor()
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, rgbColor[0]);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, rgbColor[1]);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, rgbColor[2]);
}

void RGB::Run()
{
	if(isFading_)
	{
        if (fadeStartTime_ < fadeDuration_ + 1)
        {
        	updateFade();
        }
        else
		{
        	turnOff();
		}
	}
	else if (setColorHigh_)
	{
		
	}
	else
	{
		// All blinks completed, stop the timer or handle as needed
		 stopTimer();
	}
}

void updateFade()
{
	uint32_t currentTime = HAL_GetTick();
	if (currentTime - lastUpdateTime_ >= fadeStepTime_)
	{
		// Update current color
		for (int i = 0; i < 3; i++) {
			if (currentColor_[i] != targetColor_[i]) {
				currentColor_[i] += step_[i];
				if ((step_[i] > 0 && currentColor_[i] >= targetColor_[i]) || (step_[i] < 0 && currentColor_[i] <= targetColor_[i])) {
					currentColor_[i] = targetColor_[i]; // Clamp value
				}
				setPWMColor(); // Update PWM for the current color
			}
		}

		// Check if fading is complete
		if (currentColor_[0] == targetColor_[0] && currentColor_[1] == targetColor_[1] && currentColor_[2] == targetColor_[2]) {
			isFading_ = false; // Fading is complete
		}

		lastUpdateTime_ = currentTime; // Reset last update time
	}

}

void RGB::fadeTo(uint8_t r, uint8_t g, uint8_t b, uint32_t duration)
{
    fadeDuration_ = duration;
    fadeStartTime_ = HAL_GetTick();
    isFading_ = true;

    for (int i = 0; i < 3; i++) {
        step_[i] = (targetColor_[i] - currentColor_[i]) * fadeStepTime_ / duration; // Step calculation
    }
}
    