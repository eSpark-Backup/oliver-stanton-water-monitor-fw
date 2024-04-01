/*
 * flagHandler.h
 *
 *  Created on: Dec 15, 2023
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef INC_FLAGHANDLE_H_
#define INC_FLAGHANDLE_H_

#include "main.h"
#include "mainApp.hpp"
#include "flash.h"
#include "_DEBUG_.h"



class FlagHandle{
public:
//	flagHandle(){}
	static FlagHandle& getInstance() {
		static FlagHandle instance; 			// Guaranteed to be destroyed, instantiated on first use
		return instance;
	}

    void flashRetrieve(void);										//retrieves settings from flash memory
	void flashCommit(void);											//saves settings into flash memory
	void update(void);												//updates the status of input/output controls
	bool timeElapsed(uint8_t sensor, uint32_t elapsedTime);			//used for time based settings
	bool flagStatus(uint8_t op, uint8_t sensor);					//returns the status of status flags for input/output controls
	bool setFlag(uint8_t op, uint8_t sensor, uint8_t state);		//sets setting flag for particular feature
	void flagsInit(void);

private:
	FlagHandle(){
		DEBUG_RTT(LOG_WARNING, "flagHandler obj constructor called");
	}
	~FlagHandle(){
		DEBUG_RTT(LOG_WARNING, "flagHandler obj destructor called");
	}

protected:
	typedef union							//register BitField
	{
		uint64_t controlFlags;
	   struct {
	   uint64_t _8              :1;			//LSB
	   uint64_t Prop_Mixing     :1;
	   uint64_t Auto_Flush      :1;
	   uint64_t Pump_1          :1;
	   uint64_t Pump_2          :1;
	   uint64_t Main_Heater     :1;
	   uint64_t Aux_Heater      :1;
	   uint64_t Pressure_Sensor :1;
	   uint64_t Flow_Meter_1    :1;
	   uint64_t Flow_Meter_2    :1;
	   uint64_t Flow_Meter_3    :1;
	   uint64_t Pure_CYCLE      :1;
	   uint64_t Auto_Fill       :1;
	   uint64_t Resin_Saver     :1;
	   uint64_t Mem_Saver       :1;
	   uint64_t Level_Sensor    :1;
	   uint64_t Buzzer          :1;
	   uint64_t WLED          	:1;
	   } status;
	   struct {
	   uint64_t 		        :32;
	   uint64_t Prop_Mixing     :1;
	   uint64_t        			:1;
	   uint64_t Auto_Flush      :1;
	   uint64_t Pump_1          :1;
	   uint64_t Pump_2          :1;
	   uint64_t Main_Heater     :1;
	   uint64_t Aux_Heater      :1;
	   uint64_t Flow_Meter_1    :1;
	   uint64_t Flow_Meter_2    :1;
	   uint64_t Flow_Meter_3    :1;
	   uint64_t Auto_Fill       :1;
	   uint64_t Resin_Saver     :1;
	   uint64_t Mem_Saver_Pause :1;
	   uint64_t Mem_Saver	    :1;
	   uint64_t FillOnDemand    :1;
	   uint64_t Buzzer          :1;
	   uint64_t WLED			:1;
	   uint64_t RGB			    :1;			//MSB
	   } settings;
//	  struct
//	    {
//	     uint32_t Outputs		:10;
//	     uint32_t Inputs		:5;
//	     uint32_t 				:17;
//	    } IO;

	} FlagRegister;

	typedef union
	{
		uint16_t indicationFlags;
		struct {
			uint16_t	flame_Icon			:1;
			uint16_t	tank_Fill_Icon		:1;
		} icons;
		struct {
			uint16_t						:8;
			uint16_t	place_Holder		:8;
		} indication;			//these flags will be used to display items once, we will utilize this in main loop so as to not spam the serial port
	} IndicationRegister;


	FlagRegister FlagsReg;
	IndicationRegister IndicReg;

};

#endif /* INC_FLAGHANDLE_H_ */
