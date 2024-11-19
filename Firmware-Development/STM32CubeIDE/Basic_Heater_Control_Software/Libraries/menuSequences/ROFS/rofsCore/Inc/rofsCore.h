#pragma once 

#ifndef _ROFSCORE_H_
#define _ROFSC0RE_H_

#include "mainApp.hpp"


#define SECONDS_TO_MS(x) ((x) * 1000)              // converts seconds to milliseconds
#define MIN_TO_MS(x) ((x) * SECONDS_TO_MS(60))     // converts minutes to milliseconds
#define FOD_AUTORUN_DELAY_PERIOD SECONDS_TO_MS(60) // 60secs
#define MEM_SAVER_PAUSE_TIMEOUT MIN_TO_MS(15)      // 15mins or 900secs
#define AUTO_FLUSH_TIMEOUT MIN_TO_MS(20)           // 20 mins or 1200secs
#define AUTO_FLUSH_FINISH_5M MIN_TO_MS(05)


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
    uint8_t cycleCount = 1;
    uint8_t startUpTimer = 10;
    uint8_t productionTimer = 50;
    uint8_t subsequentTimer = 10;
    uint8_t pausedTime = 0;
} autoFlushTimers;

extern autoFlushTimers flushTimer;

#endif