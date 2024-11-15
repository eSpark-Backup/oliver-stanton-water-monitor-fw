#pragma once 

#ifndef _ROFSCORE_H_
#define _ROFSC0RE_H_

#include "mainApp.hpp"

typedef enum
{
    NO_TIMER,
    STARTUP_TIMER,
    SUBSEQUENT_TIMER
} flushTimerType;

typedef struct
{
    flushTimerType selectTimer = NO_TIMER;
    uint8_t startUpTimer = 0;
    uint8_t subsequentTimer = 0;
    uint8_t pausedTime = 0;
} autoFlushTimers;

extern autoFlushTimers flushTimer;

#endif