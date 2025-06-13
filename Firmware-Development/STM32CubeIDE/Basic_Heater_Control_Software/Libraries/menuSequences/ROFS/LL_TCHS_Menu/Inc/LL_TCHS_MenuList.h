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
    nullptr,
    nullptr,
    {{"frost Prevent On", nullptr, nullptr, nullptr, nullptr, 1, 2},
    {"frost Prevent Off", nullptr, nullptr, nullptr, nullptr, 1, 2}}
};



















#endif


#endif /* _LL_TCHS_H_*/