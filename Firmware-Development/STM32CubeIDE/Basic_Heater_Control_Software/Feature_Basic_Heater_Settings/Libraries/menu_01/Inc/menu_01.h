#pragma once

#include "mainApp.hpp"
#include "menu.h"

void introSetup(void);
bool introPageSelection(uint32_t argChoice, MenuItem* argMenu);
void mainMenuSetup(void);
bool mainMenuSelection(uint32_t argChoice, MenuItem *argMenu);
void batIndexSetup(void);
bool batIndexSelection(uint32_t argChoice, MenuItem *argMenu);
void fodMenuSetup(void);
void fodOffSetup(void);
void roSaverSetup(void);
void backAndForthSetup(void);
void batConsumptionTableSetup(void);

