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


void introSetup_04(void);
bool introPageSelection_04(uint32_t argChoice, MenuItem* argMenu);
void mainMenuSetup_04(void);
bool mainMenuSelection_04(uint32_t argChoice, MenuItem *argMenu);
void fodMenuSetup_04(void);
void fodOffSetup_04(void);
void roSaverSetup_04(void);
void backAndForthSetup_04(void);
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