#pragma once

#include "mainApp.hpp"
#include "menu.h"


#define MAX_DEL_TDS_THRESHOLD 50
#define DEFAULT_DEL_TDS_THRESHOLD 10
#define MIN_DEL_TDS_THRESHOLD 5
#define DIFF_DEL_TDS_THRESHOLD 5

#define MAX_TDS_TEMPERATURE 10
#define DEFAULT_TDS_TEMPERATURE 02
#define MIN_TDS_TEMPERATURE 00
#define DIFF_TDS_TEMPERATURE 01

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



void pdsTds_introSetup(void);
bool pdsTds_introPageSelection(uint32_t argChoice, MenuItem* argMenu);
void pdsTds_mainMenuSetup(void);
bool pdsTds_mainMenuSelection(uint32_t argChoice, MenuItem *argMenu);
void pdsTds_navigationTrue(void);
bool delTDSThreshold(uint32_t argChoice, MenuItem *argMenu);
bool delTDSTemperature(uint32_t argChoice, MenuItem *argMenu);
bool delShutdown(uint32_t argChoice, MenuItem *argMenu);
bool pdsTds_haltMenuLoop(uint32_t argChoice, MenuItem *argMenu);

bool delPump(uint32_t argChoice, MenuItem *argMenu);
bool pdsTds_configureTdsWarning(uint32_t argChoice, MenuItem *argMenu);
bool pdsTds_selectTdsWarningItem(uint32_t argChoice, MenuItem *argMenu);
bool resetWarnLoop(uint32_t argChoice, MenuItem *argMenu);
bool TdsThreshAdjVal(MenuItem *argMenu);
bool TDSTempAdjVal(MenuItem *argMenu);
bool pdsTds_isMenuLabelMatch(MenuItem *menu, const MenuItem &targetThreshold, uint8_t index = 0);
bool fodOFF_08(uint32_t argChoice, MenuItem *argMenu);
bool autoFlushOptions08(uint32_t argChoice, MenuItem *argMenu);
bool autoFlushEnable08(bool activateST4);