#pragma once

#include "mainApp.hpp"
#include "menu.h"


#define DEFAULT_MAINS_TDS_THRESHOLD     500
#define DEFAULT_DYNAMIC_TDS_THRESHOLD    92
#define DEFAULT_FIXED_TDS_THRESHOLD      50
#define DEFAULT_DI_TDS_THRESHOLD         10
#define DEFAULT_TDS_TEMPERATURE          02


void introSetup_07(void);
bool introPageSelection_07(uint32_t argChoice, MenuItem* argMenu);
void mainMenuSetup_07(void);
bool mainMenuSelection_07(uint32_t argChoice, MenuItem *argMenu);
void fodMenuSetup_07(void);
void fodOffSetup_07(void);
void roSaverSetup_07(void);
void backAndForthSetup_07(void);
void navigationTrue(void);
bool calibratePressure(uint32_t argChoice, MenuItem *argMenu);
bool presCtrlOpt(uint32_t argChoice, MenuItem *argMenu);
bool presSystemTypeOpt(uint32_t argChoice, MenuItem *argMenu);
