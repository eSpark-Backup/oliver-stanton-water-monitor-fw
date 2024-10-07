/*
 * timeHandle.cpp
 *
 *  Created on: Dec 27, 2023
 *      Author: Farhan Ahmed
 */

#include "eventHandle.hpp"






void EventHandle::toggleInfinite()
{
//	DEBUG_RTT("E", "toggleInfinite Called");
    uint32_t currentTime = HAL_GetTick();
    if (state_ && (currentTime - lastToggleTime_) >= onPeriodms_[0] )  	//This will turn off the state of object
    {
    	DEBUG_RTT(LOG_ERROR, "toggleInfinite: OFF STATE -> on_period_ms_[%d] is %d", 0, onPeriodms_[0]);//i, params[i]);
		stopTimer();
		state_ = false;
		lastToggleTime_ = currentTime;
	}
    else if (!state_ && (((currentTime - lastToggleTime_) >= offPeriodms_) || blinkCount_ == 0))
    {
    	DEBUG_RTT(LOG_ERROR, "toggleInfinite: ON STATE -> off_period_ms_ is %d", offPeriodms_);//i, params[i]);
		setTimer();
		state_ = true;
		lastToggleTime_ = currentTime;
//				++blink_count_;
	}
}

void EventHandle::toggle()
{
	uint32_t currentTime = HAL_GetTick();
	if (state_ && (currentTime - lastToggleTime_) >= onPeriodms_[(blinkCount_)-1] )		//This will turn off the state of object
	{
		DEBUG_RTT(LOG_ERROR, "OFF STATE -> on_period_ms_[%d] is %d", blinkCount_-1, onPeriodms_[(blinkCount_)-1]);//i, params[i]);
		stopTimer();
		state_ = false;
		lastToggleTime_ = currentTime;
	}
	else if (!state_ && (((currentTime - lastToggleTime_) >= offPeriodms_) || blinkCount_ == 0))
	{
		setTimer();
		state_ = true;
		++blinkCount_;
		lastToggleTime_ = currentTime;
	}
}




//void EventHandle::(){
//
//}
