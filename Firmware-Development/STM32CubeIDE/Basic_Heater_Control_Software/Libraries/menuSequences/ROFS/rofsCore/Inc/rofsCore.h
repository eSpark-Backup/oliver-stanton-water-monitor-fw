#pragma once 

#ifndef _ROFSCORE_H_
#define _ROFSC0RE_H_

#include "mainApp.hpp"

typedef enum
{
    NO_TIMER,
    STARTUP_TIMER,
    PRODUCTION_TIMER,
    SUBSEQUENT_TIMER
} flushTimerType;

typedef struct
{
    flushTimerType selectTimer = NO_TIMER;
    uint8_t startUpTimer = 10;
    uint8_t productionTimer = 50;
    uint8_t subsequentTimer = 10;
    uint8_t pausedTime = 0;
} autoFlushTimers;

extern autoFlushTimers flushTimer;

#endif