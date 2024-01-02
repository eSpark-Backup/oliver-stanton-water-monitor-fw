/*
 * timeHandle.cpp
 *
 *  Created on: Dec 27, 2023
 *      Author: Farhan Ahmed
 */

#include "eventHandle.hpp"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // TIM2 overflow event occurred
        // Your code here
    }
}


bool eventHandle::setTimer(void){
//	HAL_TIM_Base_Start_IT();
}

bool eventHandle::stopTimer(void){

}

bool eventHandle::outputPeriodActive(void){
	setTimer();
	return 0;
}

//void timeHandle::(){
//
//}
