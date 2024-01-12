/*
 * timeHandle.cpp
 *
 *  Created on: Dec 27, 2023
 *      Author: Farhan Ahmed
 */

#include "eventHandle.hpp"


//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//    if (htim->Instance == TIM2) {
//        // TIM2 overflow event occurred
//        // Your code here
//    }
//}


bool eventHandle::setTimer(void){
	if(is_pwm_){
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	}
	else{
		HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET);
	}
	return false;
}

bool eventHandle::stopTimer(void){
	if(is_pwm_){
		return HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	}
	else{
		HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET);
	}
}

void eventHandle::TurnOff(void){
	is_on_ = false;
}

void eventHandle::ToggleInfinite(){
//	DEBUG_E("ToggleInfinite Called");
    uint32_t current_time = HAL_GetTick();
    if (state_ && (current_time - last_toggle_time_) >= on_period_ms_[0] )  	//This will turn off the state of object
    {
		DEBUG_E("ToggleInfinite: OFF STATE -> on_period_ms_[%d] is %d", 0, on_period_ms_[0]);//i, params[i]);
		stopTimer();
		state_ = false;
		last_toggle_time_ = current_time;
	}
    else if (!state_ && (((current_time - last_toggle_time_) >= off_period_ms_) || blink_count_ == 0))
    {
		DEBUG_E("ToggleInfinite: ON STATE -> off_period_ms_ is %d", off_period_ms_);//i, params[i]);
		setTimer();
		state_ = true;
		last_toggle_time_ = current_time;
//				++blink_count_;
	}
}

void eventHandle::Toggle(){
	uint32_t current_time = HAL_GetTick();
	if (state_ && (current_time - last_toggle_time_) >= on_period_ms_[(blink_count_)-1] )		//This will turn off the state of object
	{
		DEBUG_E("OFF STATE -> on_period_ms_[%d] is %d", blink_count_-1, on_period_ms_[(blink_count_)-1]);//i, params[i]);
		stopTimer();
		state_ = false;
		last_toggle_time_ = current_time;
	}
	else if (!state_ && (((current_time - last_toggle_time_) >= off_period_ms_) || blink_count_ == 0))
	{
		setTimer();
		state_ = true;
		++blink_count_;
		last_toggle_time_ = current_time;
	}
}

void eventHandle::Run(){
	if(is_on_)
	{
        if ((blink_count_ < num_blinks_ + 1))
        {
        	Toggle();
        }
        else if(num_blinks_ < 0 )
        {
        	ToggleInfinite();
        }
        else {
        	 is_on_ = false;
//            HAL_TIM_Base_Stop_IT(&htim2);
		}
	}
	else
	{
		// All blinks completed, stop the timer or handle as needed
		 stopTimer();
	}
}

bool eventHandle::Init(int8_t num_blinks, uint16_t timePeriod, ...){
	va_list ParamList;
//	first elements is the number of times you want it to run, second is the inactive time time period, rest are the time periods of on state
//	timePeriodArray = new uint16_t[num_blinks_+1];//+(num_blinks_-1)]		//if we also want pass the pause period of individual delays inbetween active time than use [count+(count-1)] as array length
	last_toggle_time_ = HAL_GetTick();
	num_blinks_ = num_blinks;
	on_period_ms_ = new uint16_t[num_blinks_+1];//+(num_blinks_-1)]		//if we also want pass the pause period of individual delays inbetween active time than use [count+(count-1)] as array length
	off_period_ms_ = timePeriod;
	blink_count_ = 0;
//	off_period_ms_ = on_period_ms_[0];
//	uint16_t params[elements];
	va_start(ParamList, timePeriod);

	DEBUG_E("num_blinks_ is %d", num_blinks_);
	DEBUG_E("off_period_ms_ is %d", off_period_ms_);
	if(num_blinks_ > 0)
	{
		for(uint16_t i = 0; i < num_blinks_; i++){
//			DEBUG_W("Inside FOR loop");
			on_period_ms_[i] = va_arg(ParamList, uint32_t);
			DEBUG_E("on_period_ms_[%d] is %d", i, on_period_ms_[i]);//i, params[i]);
		}
	}
	else
	{
		on_period_ms_[0] = va_arg(ParamList, uint32_t);
		DEBUG_E("on_period_ms_[%d] is %d", 0, on_period_ms_[0]);//i, params[i]);
		blink_count_ = 1;
	}

	va_end(ParamList);

	is_on_ = true;

	return false;
}


//bool buzzer(uint8_t count, uint16_t timePeriod, ...){
//	va_list ParamList;
////	first elements is the number of times you want it to run, second is the inactive time time period, rest are the time periods of on state
//	uint16_t timePeriodArray[count+1];//+(count-1)]		//if we also want pass the pause period of individual delays inbetween active time than use [count+(count-1)] as array length
//	timePeriodArray[0] = timePeriod;
//	va_start(ParamList, timePeriod);
////	uint16_t params[elements];
//
//	DEBUG_E("count is %d", count);
//	DEBUG_E("timePeriodArray[0] is %d", timePeriodArray[0]);
//	for(uint16_t i = 1; i < count; i++){
////		DEBUG_W("Inside FOR loop");
//		timePeriodArray[i] = va_arg(ParamList, uint32_t);
//		DEBUG_E("timePeriodArray[%d] is %d", i, timePeriodArray[i]);//i, params[i]);
//	}
//	va_end(ParamList);
//
//	return false;
//}

//void eventHandle::(){
//
//}
