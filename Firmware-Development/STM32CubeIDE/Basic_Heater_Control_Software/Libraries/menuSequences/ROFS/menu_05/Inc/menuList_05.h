#pragma once

#ifndef INC_MENULIST_05_H_
#define INC_MENULIST_05_H_

#include "menu.h"
// #include "mainApp.hpp"
// #include "controls.h"
#include "menu_05.h"


///-----------------------------Creating Menu Items List-----------------------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <NextItems>, <Items> }
///----------------------------------------------------------------------------------

/* by default use address of <prev_node> as <Next_node> if there is no next node to the current item
 * If you want to be able to select a range of values in item, set <NextItems> to 0 and set the <Items> from 0-items range
 */

MenuItem intro05 = {
	introSetup_05,
	introPageSelection_05,
	{{"Water Monitor Control Software", nullptr, nullptr, &mainMenu05, nullptr, 1, 1}}
};
MenuItem mainMenu05 = {
	mainMenuSetup_05,
	mainMenuSelection_05,
	{{"Default LLROFS Home Page", nullptr, nullptr, &fodMenu05, &intro05, 2, 1}},
};
MenuItem fodMenu05 = {
	defaultMenuSetup,
	nullptr,
	{{"Fill on Demand (Not Active)", nullptr, nullptr, nullptr, &mainMenu05, 2, 2},
	 {"Select TDS", nullptr, nullptr, &selectTDS, &mainMenu05, 3, 2}}
};
MenuItem selectTDS = {
	defaultMenuSetup,
	nullptr,
	{{"Mains TDS & Temp Control", nullptr, nullptr, &mainsTDS, &mainMenu05, 3, 3},
	 {"RO TDS & Temp Control"	, nullptr, nullptr, &roSaver, &mainMenu05, 3, 3},
	 {"DI TDS & Temp Control"	, nullptr, nullptr, &diTDS, &mainMenu05, 3, 3}}
};
MenuItem fodOptnsOff05 = {
	fodOffSetup_05,
	fodOFF,
	{{"Press OK to commence the purification filling process", nullptr, nullptr, nullptr, &fodMenu05, 2, 1}}
};

MenuItem haltMenu = {
	introSetup_05,
	haltMenuLoop,
	{{"", nullptr, nullptr, nullptr, nullptr, 0, 1}}
};
///-----------------------------Mains TDS-----------------------------

MenuItem mainsTDS = {
	defaultMenuSetup,
	nullptr,
	{{"Mains TDS Threshold", nullptr, nullptr, &mainsTDSThreshold, &mainMenu05, 3, 3},
	 {"Mains Temp Control", nullptr, nullptr, &mainsTDSTemperature, &mainMenu05, 3, 3},
	 {"Mains TDS Shutdown", nullptr, nullptr, &mainsTDSShutdown, &mainMenu05, 2, 3}}
};
MenuItem mainsTDSThreshold = {
	navigationTrue,
	TDSThresholdLoop,
	{{"Off", nullptr, nullptr, nullptr, &mainsTDS, 0, 3},
	 {"Default 500ppm"	, nullptr, nullptr, nullptr, &mainsTDS, 0, 3},
	 {"Adjustable 50-600ppm"	, nullptr, nullptr, nullptr, &mainsTDS, 0, 3}}
};
MenuItem mainsTDSTemperature = {
	navigationTrue,
	TDSTemperatureLoop,
	{{"Off", nullptr, nullptr, nullptr, &mainsTDS, 0, 3},
	 {"(Mains) Default 2*C", nullptr, nullptr, nullptr, &mainsTDS, 0, 3},
	 {"Adjustable 0-10*C"	, nullptr, nullptr, nullptr, &mainsTDS, 0, 3}}
};
MenuItem mainsTDSShutdown = {
	defaultMenuSetup,
	TdsShutdown,
	{{"Off (default)", nullptr, nullptr, nullptr, &mainsTDS, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &mainsTDS, 0, 2}}
};

///-----------------------------RO TDS-----------------------------

MenuItem roTDS = {
	defaultMenuSetup,
	nullptr,
	{{"RO TDS Threshold", nullptr, nullptr, &roTDSThreshold, &mainMenu05, 3, 3},
	 {"RO Temp Control"	, nullptr, nullptr, &roTDSTemperature, &mainMenu05, 3, 3},
	 {"RO TDS Shutdown"	, nullptr, nullptr, &roTDSShutdown, &mainMenu05, 2, 3}}
};
MenuItem roTDSThreshold = {
	defaultMenuSetup,
	TDSThresholdLoop,
	{{"Off", nullptr, nullptr, nullptr, &roTDS, 0, 3},
	 {"Dynamic TDS Theshold"	, nullptr, nullptr, &roDynamicTDSThreshold, &roTDS, 2, 3},
	 {"Fixed TDS Theshold"	, nullptr, nullptr, &roFixedTDSThreshold, &roTDS, 2, 3}}
};
MenuItem roDynamicTDSThreshold = {
	defaultMenuSetup,
	RoTdsThreshold,
	{{"RO Dynamic Default 92% Rejection", nullptr, nullptr, nullptr, &roTDSThreshold, 0, 2},
	 {"RO Dynamic Adjustable 50-99%", nullptr, nullptr, nullptr, &roTDSThreshold, 0, 2}}
};
MenuItem roFixedTDSThreshold = {
	defaultMenuSetup,
	RoTdsThreshold,
	{{"RO Fixed Default 50ppm", nullptr, nullptr, nullptr, &roTDSThreshold, 0, 2},
	 {"RO Fixed Adjustable 1-100ppm", nullptr, nullptr, nullptr, &roTDSThreshold, 0, 2}}
};
MenuItem roTDSTemperature = {
	navigationTrue,
	TDSTemperatureLoop,
	{{"Off", nullptr, nullptr, nullptr, &roTDS, 0, 3},
	 {"(RO) Default 2*C"	, nullptr, nullptr, nullptr, &roTDS, 0, 3},
	 {"Adjustable 0-10*C"	, nullptr, nullptr, nullptr, &roTDS, 0, 3}}
};
MenuItem roTDSShutdown = {
	defaultMenuSetup,
	TdsShutdown,
	{{"Off (default)", nullptr, nullptr, nullptr, &roTDS, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &roTDS, 0, 2}}
};

///-----------------------------DI TDS-----------------------------

MenuItem diTDS = {
	defaultMenuSetup,
	nullptr,
	{{"DI TDS Threshold", nullptr, nullptr, &diTDSThreshold, &mainMenu05, 3, 3},
	 {"DI Temp Control"	, nullptr, nullptr, &diTDSTemperature, &mainMenu05, 3, 3},
	 {"DI TDS Shutdown"	, nullptr, nullptr, &diTDSShutdown, &mainMenu05, 2, 3}}
};
MenuItem diTDSThreshold = {
	navigationTrue,
	TDSThresholdLoop,
	{{"Off", nullptr, nullptr, nullptr, &diTDS, 0, 3},
	 {"Default 10ppm", nullptr, nullptr, nullptr, &diTDS, 0, 3},
	 {"Adjustable 1-15ppm", nullptr, nullptr, nullptr, &diTDS, 0, 3}}};
MenuItem diTDSTemperature = {
	navigationTrue,
	TDSTemperatureLoop,
	{{"Off", nullptr, nullptr, nullptr, &diTDS, 0, 3},
	 {"(DI) Default 2*C"	, nullptr, nullptr, nullptr, &diTDS, 0, 3},
	 {"Adjustable 0-10*C"	, nullptr, nullptr, nullptr, &diTDS, 0, 3}}
};
MenuItem diTDSShutdown = {
	defaultMenuSetup,
	TdsShutdown,
	{{"Off (default)", nullptr, nullptr, nullptr, &diTDS, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &diTDS, 0, 2}}
};

///-----------------------------TDS END-----------------------------

#endif /* INC_MENULIST_05_H_ */
