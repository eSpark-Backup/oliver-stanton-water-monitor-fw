/*
 * flagHandler.h
 *
 *  Created on: Dec 15, 2023
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef INC_FLAGHANDLER_H_
#define INC_FLAGHANDLER_H_

#include "stm32f0xx_hal.h"
#include "main.h"
#include "mainApp.hpp"
//#include "string.h"
#include "flash.h"




//extern flagRegister flagsReg;

class flagHandle{
public:
	flagHandle(){
	}


//	static flagHandle& getInstance() {
//		static flagHandle instance; // Guaranteed to be destroyed, instantiated on first use
//		return instance;
//	}

    void flashRetrieve(void);			//retrieves settings from flash memory
	void flashCommit(void);				//saves settings into flash memory
	void update(void);					//updates the status of input/output controls
	bool timeElapsed(uint8_t sensor, uint32_t elapsedTime);			//used for time based settings
	bool flagStatus(uint8_t op, uint8_t sensor);					//returns the status of status flags for input/output controls
	bool setFlag(uint8_t op, uint8_t sensor, uint8_t state);		//sets setting flag for particular feature
	void flagsInit(void);

private:
//	flagHandle(){}
//	~flagHandle(){}

//	private static flagHandle instance;
//	private flagHandle(){};// : flagsReg(new flagRegister) {}
protected:
	typedef union
	{
		uint32_t flags;
	   struct {
	   uint32_t Resin_Saver     :1;			//LSB
	   uint32_t Prop_Mixing     :1;
	   uint32_t Mem_Saver       :1;
	   uint32_t Auto_Flush      :1;
	   uint32_t Auto_Fill       :1;
	   uint32_t Pump_1          :1;
	   uint32_t Pump_2          :1;
	   uint32_t Buzzer          :1;
	   uint32_t Main_Heater     :1;
	   uint32_t Aux_Heater      :1;
	   uint32_t Pressure_Sensor :1;
	   uint32_t Flow_Meter_1    :1;
	   uint32_t Flow_Meter_2    :1;
	   uint32_t Flow_Meter_3    :1;
	   uint32_t Level_Sensor    :1;
	   uint32_t _8              :1;
	   } status;
	   struct {
	   uint32_t 		        :16;
	   uint32_t Resin_Saver     :1;
	   uint32_t Prop_Mixing     :1;
	   uint32_t Mem_Saver       :1;
	   uint32_t Auto_Flush      :1;
	   uint32_t Auto_Fill       :1;
	   uint32_t Pump_1          :1;
	   uint32_t Pump_2          :1;
	   uint32_t Buzzer          :1;
	   uint32_t Main_Heater     :1;
	   uint32_t Aux_Heater      :1;
	   uint32_t WLED			:1;
	   uint32_t Flow_Meter_1    :1;
	   uint32_t Flow_Meter_2    :1;
	   uint32_t Flow_Meter_3    :1;
	   uint32_t RGB			    :1;
	   uint32_t FillOnDemand    :1;			//MSB
	   } settings;

	//   struct
	//  {
	//   uint32_t 				:5;
	//   uint32_t Pumps			:2;
	//   uint32_t 				:1;
	//   uint32_t Heaters			:2;
	//   uint32_t 				:1;
	//   uint32_t FlowMeter		:3;
	//   uint32_t 				:2;
	//  } nibbles;
//	  struct
//	    {
//	     uint32_t Outputs		:10;
//	     uint32_t Inputs		:5;
//	     uint32_t 				:17;
//	    } IO;

	} flagRegister;

	flagRegister* flagsReg;

};

#endif /* INC_FLAGHANDLER_H_ */
