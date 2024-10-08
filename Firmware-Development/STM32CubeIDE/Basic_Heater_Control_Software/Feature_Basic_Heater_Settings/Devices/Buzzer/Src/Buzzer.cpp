#include "Buzzer.hpp"

void BuzzerDriver::setupInit(void)
{
	// Initialize GPIO pin

}


void BuzzerDriver::setOutput(void)
{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}


void BuzzerDriver::stopOutput(void)
{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}


void BuzzerDriver::turnOff(void)
{
	this->numBlinks_ = 0;
//	blinkCount_ = 0;
	this->offPeriodms_ = 0;
	this->isOn_ = false;
	if (this->onPeriodms_ != nullptr)
	{
        delete[] this->onPeriodms_;
        this->onPeriodms_ = nullptr;  // Reset to prevent future issues
    }
}


void BuzzerDriver::Run()
{
	if(this->isOn_)
	{
        if ((this->blinkCount_ < this->numBlinks_ + 1))
        {
        	toggle();
        }
        else if(this->numBlinks_ < 0 )
        {
        	toggleInfinite();
        }
        else {
        	turnOff();
		}
	}
	else
	{
		// All blinks completed, stop the timer or handle as needed
		stopOutput();
	}
}

