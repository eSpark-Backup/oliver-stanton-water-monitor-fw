#pragma once

#include "mainApp.hpp"
#include "menu.h"
#include "rofsCore.h"

bool autoFlushLoop(uint32_t argChoice, MenuItem *argMenu);
bool autoFlushTimerLoop(uint32_t argChoice, MenuItem *argMenu);
bool productionTimerLoop(uint32_t argChoice, MenuItem *argMenu);