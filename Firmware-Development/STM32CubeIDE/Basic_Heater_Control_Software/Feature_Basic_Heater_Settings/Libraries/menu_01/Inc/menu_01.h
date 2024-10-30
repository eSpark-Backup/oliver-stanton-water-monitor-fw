#pragma once

#include "mainApp.hpp"
#include "menu.h"

void introSetup(void);
bool introPageSelection(uint32_t argChoice, MenuItem* argMenu);

void mainMenuSetup(void);
bool mainMenuSelection(uint32_t argChoice, MenuItem *argMenu);

void roSaverSetup(void);
bool setRoSaverThresh(uint32_t argChoice, MenuItem *argMenu);
bool roSaverThreshAdj(uint32_t argChoice, MenuItem *argMenu);
bool roSaverThreshDefault(uint32_t argChoice, MenuItem *argMenu);
bool roSaverOptions(uint32_t argChoice, MenuItem *argMenu);
bool roSaverOn(uint32_t argChoice, MenuItem *argMenu);
bool roSaverOff(uint32_t argChoice, MenuItem *argMenu);

void fodOffSetup(void);
bool fodOFF(uint32_t argChoice, MenuItem *argMenu);

void fodMenuSetup(void);
bool fodON(uint32_t argChoice, MenuItem *argMenu);

void batConsumptionTableSetup(void);
bool BatLowMenuOption(uint32_t argChoice, MenuItem *argMenu);
bool BatLowShutdown(uint32_t argChoice, MenuItem *argMenu);
bool BatLowShutdownOn(uint32_t argChoice, MenuItem *argMenu);
bool BatLowShutdownOff(uint32_t argChoice, MenuItem *argMenu);
bool BatPowerMonitor(uint32_t argChoice, MenuItem *argMenu);
bool BatPowerMonitorOn(uint32_t argChoice, MenuItem *argMenu);
bool BatPowerMonitorOff(uint32_t argChoice, MenuItem *argMenu);

void batIndexSetup(void);
bool batIndexSelection(uint32_t argChoice, MenuItem *argMenu);

void backAndForthSetup(void);
