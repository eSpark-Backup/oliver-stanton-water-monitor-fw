#pragma once

#include "mainApp.hpp"
#include "menu.h"

void introSetup_05(void);
bool introPageSelection_05(uint32_t argChoice, MenuItem* argMenu);
void mainMenuSetup_05(void);
bool mainMenuSelection_05(uint32_t argChoice, MenuItem *argMenu);
void fodMenuSetup_05(void);
void fodOffSetup_05(void);
void roSaverSetup_05(void);
void backAndForthSetup_05(void);
bool TdsSettingsOff(uint32_t argChoice, MenuItem *argMenu);
bool TDSThresholdLoop(uint32_t argChoice, MenuItem *argMenu);
bool TDSTemperatureLoop(uint32_t argChoice, MenuItem *argMenu);
bool TdsShutdown(uint32_t argChoice, MenuItem *argMenu);
