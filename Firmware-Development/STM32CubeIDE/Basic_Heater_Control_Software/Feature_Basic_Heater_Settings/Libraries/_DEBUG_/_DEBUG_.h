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

// Define the buffer size
#define LOG_BUFFER_SIZE 256

#define DEBUG_AHTASHAM 0


int DEBUG_RTT(char level, const char *sFormat, ...);

#if DEBUG_AHTASHAM == 1
// Define the macro to use its own implementation
    #define DEBUG_LOG(menu, format, ...) \
        do { \
            char logBuffer[LOG_BUFFER_SIZE]; \
            snprintf(logBuffer, LOG_BUFFER_SIZE, format, ##__VA_ARGS__); \
            menu.Display_Log(logBuffer); \
        } while (0)

#else
// Define the macro to use DEBUG_RTT
    #define DEBUG_LOG(menu, format, ...) \
        do { \
            DEBUG_RTT('I', format, ##__VA_ARGS__); \
        } while (0)
#endif
//int DEBUG_(const char * sFormat, ...);
//int DEBUG_E(const char * sFormat, ...);
//int DEBUG_W(const char * sFormat, ...);
//int DEBUG_I(const char * sFormat, ...);
//int DEBUG_A(const char * sFormat, ...);
//int DEBUG_I(const char * sFormat, ...);
//int DEBUG_I(const char * sFormat, ...);




#endif /* DEBUG___DEBUG__H_ */
