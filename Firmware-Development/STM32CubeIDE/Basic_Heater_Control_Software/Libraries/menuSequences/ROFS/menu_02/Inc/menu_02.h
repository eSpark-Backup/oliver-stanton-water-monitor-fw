#pragma once

#include "mainApp.hpp"
#include "menu.h"

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
} autoFlushTimers;


bool autoFlushLoop(uint32_t argChoice, MenuItem *argMenu);
bool autoFlushTimerLoop(uint32_t argChoice, MenuItem *argMenu);