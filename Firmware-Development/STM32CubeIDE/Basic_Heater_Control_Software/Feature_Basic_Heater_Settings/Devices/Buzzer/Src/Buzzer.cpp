#include "Buzzer.hpp"

void BuzzerDriver::setupInit(void)
{
	// Initialize GPIO pin

}


void BuzzerDriver::startPWM(void)
{
	if(isPWM_)
	{
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	}
	else
	{
		HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET);
	}
}


void BuzzerDriver::stopPWM(void)
{
	if(isPWM_)
	{
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	}
	else
	{
		HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET);
	}
}


void BuzzerDriver::turnOff(void)
{
	numBlinks_ = 0;
//	blinkCount_ = 0;
	offPeriodms_ = 0;
	isOn_ = false;
	if (onPeriodms_ != nullptr)
	{
        delete[] onPeriodms_;
        onPeriodms_ = nullptr;  // Reset to prevent future issues
    }
}


void BuzzerDriver::toggleInfinite()
{
//	DEBUG_RTT("E", "toggleInfinite Called");
    uint32_t currentTime = HAL_GetTick();
    if (state_ && (currentTime - lastToggleTime_) >= onPeriodms_[0] )  	//This will turn off the state of object
    {
    	DEBUG_RTT(LOG_ERROR, "toggleInfinite: OFF STATE -> on_period_ms_[%d] is %d", 0, onPeriodms_[0]);//i, params[i]);
		stopPWM();
		state_ = false;
		lastToggleTime_ = currentTime;
	}
    else if (!state_ && (((currentTime - lastToggleTime_) >= offPeriodms_) || blinkCount_ == 0))
    {
    	DEBUG_RTT(LOG_ERROR, "toggleInfinite: ON STATE -> off_period_ms_ is %d", offPeriodms_);//i, params[i]);
		startPWM();
		state_ = true;
		lastToggleTime_ = currentTime;
//				++blink_count_;
	}
}


void BuzzerDriver::toggle()
{
	uint32_t currentTime = HAL_GetTick();
	if (state_ && (currentTime - lastToggleTime_) >= onPeriodms_[(blinkCount_)-1] )		//This will turn off the state of object
	{
		DEBUG_RTT(LOG_ERROR, "OFF STATE -> on_period_ms_[%d] is %d", blinkCount_-1, onPeriodms_[(blinkCount_)-1]);//i, params[i]);
		stopPWM();
		state_ = false;
		lastToggleTime_ = currentTime;
	}
	else if (!state_ && (((currentTime - lastToggleTime_) >= offPeriodms_) || blinkCount_ == 0))
	{
		startPWM();
		state_ = true;
		++blinkCount_;
		lastToggleTime_ = currentTime;
	}
}


void BuzzerDriver::Run()
{
	if(isOn_)
	{
        if ((blinkCount_ < numBlinks_ + 1))
        {
        	toggle();
        }
        else if(numBlinks_ < 0 )
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
		stopPWM();
	}
}


void BuzzerDriver::setOutputPattern(int8_t numBlinks, uint16_t timePeriod, ...)
{
	va_list paramList;
//	first elements is the number of times you want it to run, second is the inactive time time period, rest are the time periods of on state
	lastToggleTime_ = HAL_GetTick();
	numBlinks_ = numBlinks;
	onPeriodms_ = new uint16_t[numBlinks_+1];			//if we also want pass the pause period of individual delays inbetween active time than use [count+(count-1)] as array length
	offPeriodms_ = timePeriod;
	blinkCount_ = 0;
	va_start(paramList, timePeriod);

	DEBUG_RTT(LOG_ERROR, "num_blinks_ is %d", numBlinks_);
	DEBUG_RTT(LOG_ERROR, "off_period_ms_ is %d", offPeriodms_);
	if(numBlinks_ > 0)
	{
		for(uint16_t i = 0; i < numBlinks_; i++)
		{
			onPeriodms_[i] = va_arg(paramList, uint32_t);
			DEBUG_RTT(LOG_ERROR, "on_period_ms_[%d] is %d", i, onPeriodms_[i]);//i, params[i]);
		}
	}
	else
	{
		onPeriodms_[0] = va_arg(paramList, uint32_t);
		DEBUG_RTT(LOG_ERROR, "on_period_ms_[%d] is %d", 0, onPeriodms_[0]);//i, params[i]);
		blinkCount_ = 1;
	}

	va_end(paramList);

	isOn_ = true;
}
