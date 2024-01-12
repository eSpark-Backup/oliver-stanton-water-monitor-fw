/*
 * _DEBUG_.h
 *
 *  Created on: Jan 3, 2024
 *      Author: Farhan AHmed
 */

#ifndef DEBUG___DEBUG__H_
#define DEBUG___DEBUG__H_

#include "SEGGER_RTT.h"

int DEBUG_(const char * sFormat, ...);
int DEBUG_E(const char * sFormat, ...);
int DEBUG_W(const char * sFormat, ...);
int DEBUG_I(const char * sFormat, ...);
int DEBUG_A(const char * sFormat, ...);
//int DEBUG_I(const char * sFormat, ...);
//int DEBUG_I(const char * sFormat, ...);




#endif /* DEBUG___DEBUG__H_ */
