#pragma once

#include "mainApp.hpp"
#include "menu.h"


#define MAX_MAINS_TDS_THRESHOLD 600
#define DEFAULT_MAINS_TDS_THRESHOLD 500
#define MIN_MAINS_TDS_THRESHOLD 50
#define DIFF_MAINS_TDS_THRESHOLD 20

#define MAX_RO_TDS_THRESHOLD 100
#define DEFAULT_DYNAMIC_TDS_THRESHOLD 92
#define DEFAULT_FIXED_TDS_THRESHOLD 50
#define MIN_DYNAMIC_TDS_THRESHOLD 50
#define MIN_FIXED_TDS_THRESHOLD 01
#define DIFF_RO_TDS_THRESHOLD 01

#define MAX_DI_TDS_THRESHOLD 15
#define DEFAULT_DI_TDS_THRESHOLD 10
#define MIN_DI_TDS_THRESHOLD 01
#define DIFF_DI_TDS_THRESHOLD 01

#define MAX_TDS_TEMPERATURE 10
#define DEFAULT_TDS_TEMPERATURE 02
#define MIN_TDS_TEMPERATURE 00
#define DIFF_TDS_TEMPERATURE 01

#define DEFAULT_RESIN_SAVER_INTRVL 03


typedef enum
{
    NO_TIMER,
    STARTUP_TIMER,
    PRODUCTION_TIMER,
    SUBSEQUENT_TIMER
} flushTimerType;

typedef enum
{
    TDS_ACTIVATION,
    DEACTIVATED,
    ACTIVATED,
    READY,
    PAUSE,
    WAIT
} autoflushStatusType;

typedef struct
{
    flushTimerType selectTimer = NO_TIMER;
    autoflushStatusType activityStatus = DEACTIVATED;
    uint8_t cycleCount = 0;
    uint8_t startUpTimer = 10;
    uint8_t productionTimer = 50;
    uint8_t subsequentTimer = 10;
    uint32_t pausedTime = 0;
} autoFlushTimers;

extern autoFlushTimers flushTimer;


void introSetup_02(void);
bool introPageSelection_02(uint32_t argChoice, MenuItem* argMenu);
void mainMenuSetup_02(void);
bool mainMenuSelection_02(uint32_t argChoice, MenuItem *argMenu);
void fodMenuSetup_02(void);
void fodOffSetup_02(void);
void roSaverSetup_02(void);
bool fodOFF_02(uint32_t argChoice, MenuItem *argMenu);
void backAndForthSetup_02(void);
bool TdsSettingsOff(uint32_t argChoice, MenuItem *argMenu);
void navigationTrue(void);
bool TDSThresholdAdjustedValue(MenuItem *argMenu);
bool TDSThresholdLoop(uint32_t argChoice, MenuItem *argMenu);
bool TDSTemperatureLoop(uint32_t argChoice, MenuItem *argMenu);
bool TdsShutdown(uint32_t argChoice, MenuItem *argMenu);
bool setRoTdsThreshold(uint32_t argChoice, MenuItem *argMenu);
bool haltMenuLoop(uint32_t argChoice, MenuItem *argMenu);

bool autoFlushOptions(uint32_t argChoice, MenuItem *argMenu);
bool autoFlushEnable(bool activateST4);
bool configureTdsWarning(uint32_t argChoice, MenuItem *argMenu);
bool selectTdsWarningItem(uint32_t argChoice, MenuItem *argMenu);

bool diSaverOptions(uint32_t argChoice, MenuItem *argMenu);
bool diSaverOnOff(uint32_t argChoice, MenuItem *argMenu);

bool autoFlushLoop(uint32_t argChoice, MenuItem *argMenu);
bool autoFlushTimerLoop(uint32_t argChoice, MenuItem *argMenu);
bool productionTimerLoop(uint32_t argChoice, MenuItem *argMenu);