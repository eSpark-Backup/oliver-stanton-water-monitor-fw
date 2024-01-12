/*
 * eventHandle.hpp
 *
 *  Created on: Dec 27, 2023
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef INC_EVENTHANDLE_HPP_
#define INC_EVENTHANDLE_HPP_

//#include <iostream>
#include <stdarg.h>
#include "main.h"
#include "mainApp.hpp"
#include "controls.h"

//extern TIM_HandleTypeDef htim1;
//TIM_HandleTypeDef *PWMTimer;


/*
 *  			Class for handling outputs for time based events
 *  define functions which will enable/disable outputs for a certain period of time
 *  define functions for toggling output for a ceratin period of time
 */

class eventHandle{
public:
	eventHandle(GPIO_TypeDef* port, uint16_t pin, bool is_pwm_)
        : port_(port), pin_(pin), on_period_ms_(0), off_period_ms_(0), num_blinks_(0), blink_count_(0), is_on_(false), state_(false), is_pwm_(is_pwm_) {

        // Initialize GPIO pin
        if(!is_pwm_)
        {
			GPIO_InitTypeDef GPIO_InitStruct = {0};
			GPIO_InitStruct.Pin = pin_;
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
			HAL_GPIO_Init(port_, &GPIO_InitStruct);
        }
        // Start the timer
//        HAL_TIM_Base_Start_IT(&htim1); // Assuming you're using TIM2

        // Set up the timer interrupt
//        HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
//        HAL_NVIC_EnableIRQ(TIM2_IRQn);
        DEBUG_W("eventHandle obj constructor called");
    }
	~eventHandle(){
		delete[] on_period_ms_;
        DEBUG_W("eventHandle obj destructor called");
	}

	bool setTimer(void);
	bool stopTimer(void);
	bool outputPeriodActive(void);			//turn on an output for certain period of time

	bool Init(int8_t num_blinks_, uint16_t timePeriod, ...);
	void Run(void);
	void TurnOff(void);
	void Toggle(void);
	void ToggleInfinite(void);

//    void Update() {
//        if (blink_count_ < num_blinks_ * 2) {
//            uint32_t current_time = HAL_GetTick();
//            if (is_on_ && (current_time - last_toggle_time_) >= on_period_ms_) {
//                HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET); // Turn off LED
//                is_on_ = false;
//                last_toggle_time_ = current_time;
//            } else if (!is_on_ && (current_time - last_toggle_time_) >= off_period_ms_) {
//                HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET); // Turn on LED
//                is_on_ = true;
//                last_toggle_time_ = current_time;
//                ++blink_count_;
//            }
//        } else {
//            // All blinks completed, stop the timer or handle as needed
////            HAL_TIM_Base_Stop_IT(&htim2);
//        }
//    }

private:
    GPIO_TypeDef* port_;						//GPIO port of output
    uint16_t pin_;								//pin number of output
    uint16_t *on_period_ms_;						//output turn on duration
    uint32_t off_period_ms_;					//output turn off duration
    int8_t num_blinks_;						//How many times you want output to turn on
    uint16_t blink_count_;						//current number of blink
//    uint16_t *timePeriodArray;					//array for storing time periods
    uint32_t last_toggle_time_;					//Tick() when output was toggled
    bool is_pwm_;								//is output controlled by PWM
    bool is_on_;								//is object activated or not
    bool state_;								//obj is currently in runnign or off state
};



#endif /* INC_EVENTHANDLE_HPP_ */


/*
 * P.S.
 * Available timers are		TIM1(ADVANCED, will use for PWM generation purpose (most probably for buzzer))
 * 							TIM6/7(Will use these for setting up counts and such)
 * 							TIM()
 */
