/*
 * LL_TCHS_Menu.h
 *
 *  Created on: June 12, 2025
 *      Author: Abdul Ahad
 */

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


void heaterIntroSetup();
void heaterSettOptns();
void heaterTogg();
void heaterOf();
bool heaterOfLoop(uint32_t argChoice, MenuItem *argMenu);
void deliveryHomeSetup(void);
bool deliveryHomeLoop(uint32_t argChoice, MenuItem *argMenu);


void mainMenuFuncHeater();
