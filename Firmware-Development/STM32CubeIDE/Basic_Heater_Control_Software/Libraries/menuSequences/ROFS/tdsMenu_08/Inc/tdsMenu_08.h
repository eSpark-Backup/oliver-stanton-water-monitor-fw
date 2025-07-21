#pragma once

#include "mainApp.hpp"
#include "menu.h"


#define MAX_WASTE_PER  100
#define MIN_WASTE_PER  40
#define DIFF_WASTE_PER 10

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


void llTds_introSetup_08(void);
bool llTds_introPageSelection_08(uint32_t argChoice, MenuItem* argMenu);
void llTds_mainMenuSetup_08(void);
bool llTds_mainMenuSelection_08(uint32_t argChoice, MenuItem *argMenu);
void llTds_fodMenuSetup_08(void);
bool llTds_fodOFF_08(uint32_t argChoice, MenuItem *argMenu);
void llTds_backAndForthSetup_08(void);
void llTds_navigationTrue(void);
void ctrlWasteRatioToggleSetup(void);
void wasteRatioOffSetup(void);
void wasteRatioAlertsSetup(void);
void enableAlerts(void);
void disableAlerts(void);
void enableAlertsAndShutDown(void);
void disableAlertsAndShutDown(void);
void flowMeterMenuSetup(void);
void mainsInputSetup(void);
void pureOutputSetup(void);
void wasteOutputSetup(void);
bool liveFlowLoop(uint32_t argChoice, MenuItem *argMenu);
bool overAllFlowSinceServiceLoop(uint32_t argChoice, MenuItem *argMenu);
bool overAllFlowSinceNewLoop(uint32_t argChoice, MenuItem *argMenu);





bool llTdsWasteSelOpt(uint32_t argChoice, MenuItem *argMenu);
bool setWasteRatio(uint32_t argChoice, MenuItem *argMenu);
bool setDynamicWasteRatio(uint32_t argChoice, MenuItem *argMenu);
bool autoFlushOptions08(uint32_t argChoice, MenuItem *argMenu);
bool autoFlushEnable08(bool activateST4);