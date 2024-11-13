/*
 * motion.h
 *
 */

#pragma once

class MotionSensor{
private:
	bool motionWakeUp=true;
	bool motionWelcome=true;
	bool motionFlag=false;
public:
	void motionDetected();
	bool getMotionWU();
	bool getMotionWelcome();
	bool getMotionFlag();
	void setMotionFlag(bool state);
	void setup();
};


