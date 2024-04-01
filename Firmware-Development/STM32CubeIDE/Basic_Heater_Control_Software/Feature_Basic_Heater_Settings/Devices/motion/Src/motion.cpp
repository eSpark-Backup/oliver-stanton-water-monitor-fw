
#include "motion.hpp"
#include "mainApp.hpp"
#include <functional>
#include "callbacks.h"

void MotionSensor::motionDetected()
{
	this->setMotionFlag(1);
}

bool MotionSensor::getMotionWU()
{
	return this->motionWakeUp;
}

bool MotionSensor::getMotionWelcome()
{
	return this->motionWelcome;
}

bool MotionSensor::getMotionFlag()
{
	return this->motionFlag;
}

void MotionSensor::setMotionFlag(bool state)
{
		this->motionFlag=state;
}
void MotionSensor::setup()
{
//	registerCallback(std::bind(&MotionSensor::motionDetected,this));
}

