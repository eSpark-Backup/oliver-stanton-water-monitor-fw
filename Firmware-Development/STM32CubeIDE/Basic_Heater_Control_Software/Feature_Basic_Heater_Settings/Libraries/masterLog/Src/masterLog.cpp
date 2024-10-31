/*
 * masterLog.cpp
 *
 *  Created on: OCT 31st, 2024
 *      Author: Nehal Abdul Rehman
 */

#include "masterLog.h"
#include "mainApp.hpp"
#include <cstdarg>

void masterLog::logToMaster(const char* inputLog, ...)
{
    const int logSize = 1024;
    const int timeStampSize = 50;
    char log[logSize];

    // find time from RTC
    const char *timestamp = "Time stamp: xx/xx/xx xx:xx - ";

    va_list args;
    va_start(args, inputLog);
    vsnprintf(log, logSize, inputLog, args);
    va_end(args);

    char finalLog[logSize + timeStampSize];
    snprintf(finalLog, logSize + timeStampSize, "%s%s", timestamp, log);

    // save to SD card function
}