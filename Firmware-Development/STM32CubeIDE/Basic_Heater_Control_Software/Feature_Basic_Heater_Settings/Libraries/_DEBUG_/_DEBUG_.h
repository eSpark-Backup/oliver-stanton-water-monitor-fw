/*
 * _DEBUG_.h
 *
 *  Created on: Jan 3, 2024
 *      Author: Farhan AHmed
 */


/* There are total of 6 levels of log in embedded
 * Trace:	to find one part of a function specifically.
 * Debug:
 * Info:
 * Warn:
 * Error:
 * Fatal:
 */


#ifndef DEBUG___DEBUG__H_
#define DEBUG___DEBUG__H_

#include "SEGGER_RTT.h"

/*--------------debug-------------------*/
//#define _DEBUG_										//un-comment these three lines for receiving DEBUG Messages
//#define USE_FULL_ASSERT 1
#define	LOG_ERROR	'E'
#define	LOG_WARNING	'W'
#define	LOG_INFO	'I'
#define	LOG_HEADING	'H'

int DEBUG_RTT(char level, const char *sFormat, ...);
//int DEBUG_(const char * sFormat, ...);
//int DEBUG_E(const char * sFormat, ...);
//int DEBUG_W(const char * sFormat, ...);
//int DEBUG_I(const char * sFormat, ...);
//int DEBUG_A(const char * sFormat, ...);
//int DEBUG_I(const char * sFormat, ...);
//int DEBUG_I(const char * sFormat, ...);




#endif /* DEBUG___DEBUG__H_ */
