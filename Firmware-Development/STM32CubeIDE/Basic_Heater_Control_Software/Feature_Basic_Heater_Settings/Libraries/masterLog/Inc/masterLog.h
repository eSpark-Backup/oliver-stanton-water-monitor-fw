/*
 * masterLog.h
 *
 *  Created on: OCT 31st, 2024
 *      Author: Nehal Abdul Rehman
 */

#ifndef INC_MASTERLOG_H_
#define INC_MASTERLOG_H_

#include "stm32f0xx_hal.h"

class masterLog
{

public:
	void logToMaster(const char *inputLog, ...);

};


#endif /* INC_MASTERLOG_H_ */
