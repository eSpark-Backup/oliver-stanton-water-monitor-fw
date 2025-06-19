/*
 * LL_TCHS_MenuList.h
 *
 *  Created on: June 12, 2025
 *      Author: Abdul Ahad
 */

#pragma once

#ifndef _LL_TCHS_H_
#define _LL_TCHS_H_




#include "stm32f0xx_hal_conf.h"

#if LL_TCHS_MENU == 1

#include "LL_TCHS_Menu.h"
#include "menu.h"
// #include "mainApp.hpp"
// #include "controls.h"





MenuItem heaterIntro = {
	heaterIntroSetup,
	nullptr,
	{{"Heater Settings", nullptr, nullptr, &heaterSettingsOptns, nullptr, 2, 1}}
};

MenuItem heaterSettingsOptns = {
	heaterSettOptns,
	nullptr,
	{{"Heater", nullptr, nullptr, &heaterToggle, &heaterSettingsOptns, 2, 2},
	{"Frost Prevention", nullptr, nullptr, &frostPreventToggle, &heaterToggle, 2, 2}}
};


MenuItem heaterToggle = {
	heaterTogg,
	nullptr,
	{{"Heater On", nullptr, nullptr, &deliveryHome, &heaterSettingsOptns, 1, 2},
	{"Heater Off", nullptr, nullptr, &heaterOff, &heaterSettingsOptns, 1, 2}}
};


MenuItem heaterOff = {
	heaterOf,
	heaterOfLoop,
	{{"Heater Turned OFF", nullptr, nullptr, nullptr, &heaterToggle, 1, 1}}
};


MenuItem deliveryHome = {
    deliveryHomeSetup,
    deliveryHomeLoop,
    {{"Delivery Home Screen ", nullptr, nullptr, nullptr, &heaterToggle, 1, 1}}
};

MenuItem frostPreventToggle = {
    frostPreventToggleSetup,
    nullptr,
    {{"frost Prevent On", nullptr, nullptr, &frostPreventOn, &heaterSettingsOptns, 3, 2},
    {"frost Prevent Off", nullptr, nullptr, &frostPreventOff, &heaterSettingsOptns, 1, 2}}
};


MenuItem frostPreventOff= {
    frostPreventOffSetup,
    frostPreventOffLoop,
    {{"frost Prevent Off", nullptr, nullptr, nullptr, &frostPreventToggle, 1, 1}}
};

MenuItem frostPreventOn= {
    frostPreventOnSetup,
    nullptr,
    {{"Set Frost Prevention Temperature Threshold", nullptr, nullptr, &frostPreventTempThresh, &frostPreventToggle, 1, 3},
    {"Set Frost Prevention Temperature Timer", nullptr, nullptr, &frostPreventTimer, &frostPreventToggle, 1, 3},
    {"Set Temperature Format", nullptr, nullptr, &TempFormat, &frostPreventToggle, 2, 3 }}
};

MenuItem frostPreventTimer = {
    frostPreventTimerSetup,
    frostPreventTimerLoop,
    {{"UP and Down keys to increase/decrease time", nullptr, nullptr, nullptr, &frostPreventOn, 1, 1}}
};

MenuItem TempFormat = {
    TempFormatSetup,
    nullptr,
    {{"Celsius (°C)", celsiusSetup, nullptr, nullptr, &frostPreventOn, 1, 2},
    {"Fahrenheit (°F)", fahrenheitSetup, nullptr, nullptr, &frostPreventOn, 1, 2}}
};
MenuItem frostPreventTempThresh = {
    frostPreventTempThreshSetup,
    frostPreventTempThreshLoop,
    {{"UP and Down keys to increase/decrease Temperature Threshold", nullptr, nullptr, nullptr, &frostPreventOn, 1, 1}}
};





















#endif


#endif /* _LL_TCHS_H_*/