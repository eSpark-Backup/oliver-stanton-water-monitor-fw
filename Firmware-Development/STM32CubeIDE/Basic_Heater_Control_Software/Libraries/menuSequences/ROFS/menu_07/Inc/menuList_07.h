#pragma once

#ifndef INC_MENULIST_07_H_
#define INC_MENULIST_07_H_

#include "stm32f0xx_hal_conf.h"
#if MENU_07 == 1

#include "menu.h"
// #include "mainApp.hpp"
// #include "controls.h"
#include "menu_07.h"


///-----------------------------Creating Menu Items List-----------------------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <NextItems>, <Items> }
///----------------------------------------------------------------------------------

/* by default use address of <prev_node> as <Next_node> if there is no next node to the current item
 * If you want to be able to select a range of values in item, set <NextItems> to 0 and set the <Items> from 0-items range
 */

MenuItem intro07 = {
	introSetup_07,
	introPageSelection_07,
	{{"Water Monitor Control Software", nullptr, nullptr, &mainMenu07, nullptr, 1, 1}}
};
MenuItem mainMenu07 = {
	mainMenuSetup_07,
	mainMenuSelection_07,
	{{"Default LLROFS Home Page", nullptr, nullptr, &fodMenu07, &intro07, 3, 1}},
};
MenuItem fodMenu07 = {
	defaultMenuSetup,
	nullptr,
	{{"Fill on Demand", nullptr, nullptr, &setAutoFill, &mainMenu07, 0, 3},
	{"Set AutoFlush On/Off", nullptr, nullptr, &setAutoFlush, &mainMenu07, 0, 3},
	{"Select Pressure Setting", nullptr, nullptr, &selectPressureSetting, &mainMenu07, 5, 3}}
};
MenuItem selectPressureSetting = {
	defaultMenuSetup,
	nullptr,
	{{"Set Pressure Control On/Off",       nullptr, nullptr, &presCtrlOptns, &fodMenu07, 2, 5},
	 {"Pressure Sensor Calibration",  	   nullptr, nullptr, &presCalibrateOptns, &fodMenu07, 2, 5},
	 {"Pressure Sensor Alerts & Shutdown", nullptr, nullptr, &presAlertOptns, &fodMenu07, 3, 5},
	 {"Boosted Filling Production On/Off", nullptr, nullptr, &pFillingProdOptns, &fodMenu07, 2, 5},
	 {"Boosted autoflush On/Off", 		   nullptr, nullptr, &pFlushProdOptns, &fodMenu07, 2, 5},
	 {"Set System Type", 			  nullptr, nullptr, &presSystemTypeOptns, &fodMenu07, 2, 5}}
};
MenuItem fodOptnsOff07 = {
	fodOffSetup_07,
	fodOFF,
	{{"Press OK to commence the purification filling process", nullptr, nullptr, nullptr, &fodMenu07, 2, 1}}
};

///-----------------------------PRESSURE SETTINGS START-----------------------------

MenuItem presCalibrateOptns = {
	defaultMenuSetup,
	pressureSnsrcalibration,
	{{"Display calibrated readings (Read Only)" , 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2},
	 {"Recalibrate"	, 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2}}
};
MenuItem presAlertOptns = {
	defaultMenuSetup,
	presAlertOpt,
	{{"ON - Alerts Only", 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 3},
	 {"ON - Alerts & Shutdown", nullptr, nullptr, nullptr, &selectPressureSetting, 0, 3},
	 {"OFF", 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 3}}
};
MenuItem pFillingProdOptns = {
	defaultMenuSetup,
	boostedFillingOpt,
	{{"Disable" , 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2},
	 {"Enable"	, 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2}}
};
MenuItem pFlushProdOptns = {
	defaultMenuSetup,
	boostedFlushOpt,
	{{"Disable" , 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2},
	 {"Enable"	, 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2}}
};
MenuItem presCtrlOptns = {
	defaultMenuSetup,
	presCtrlOpt,
	{{"Disable" , 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2},
	 {"Enable"	, 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2}}
};
MenuItem presSystemTypeOptns = {
	defaultMenuSetup,
	presSystemTypeOpt,
	{{"Vehicle" , 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2},
	 {"Static"	, 	nullptr, nullptr, nullptr, &selectPressureSetting, 0, 2}}
};

///-----------------------------PRESSURE SETTINGS END-----------------------------


MenuItem setAutoFlush = {
	defaultMenuSetup,
	autoFlushOptions,
	{{"Disable" , 	nullptr, nullptr, nullptr, &fodMenu07, 0, 2},
	 {"Enable"	, 	nullptr, nullptr, nullptr, &fodMenu07, 0, 2}}
};

MenuItem setAutoFill = {
	defaultMenuSetup,
	autoFillOptions,
	{{"Disable" , 	nullptr, nullptr, nullptr, &fodMenu07, 0, 2},
	 {"Enable"	, 	nullptr, nullptr, nullptr, &fodMenu07, 0, 2}}
};

#endif

#endif /* INC_MENULIST_07_H_ */
