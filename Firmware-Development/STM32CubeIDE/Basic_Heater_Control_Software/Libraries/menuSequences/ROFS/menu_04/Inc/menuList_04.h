#pragma once

#ifndef INC_MENULIST_04_H_
#define INC_MENULIST_04_H_

#include "stm32f0xx_hal_conf.h"
#if MENU_04 == 1

#include "menu.h"
// #include "mainApp.hpp"
// #include "controls.h"
#include "menu_04.h"


///-----------------------------Creating Menu Items List-----------------------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <NextItems>, <Items> }
///----------------------------------------------------------------------------------

/* by default use address of <prev_node> as <Next_node> if there is no next node to the current item
 * If you want to be able to select a range of values in item, set <NextItems> to 0 and set the <Items> from 0-items range
 */

MenuItem intro04 = {
	introSetup_04,
	introPageSelection_04,
	{{"Water Monitor Control Software", nullptr, nullptr, &mainMenu04, nullptr, 1, 1}}
};
MenuItem mainMenu04 = {
	mainMenuSetup_04,
	mainMenuSelection_04,
	{{"Default LLROFS Home Page", nullptr, nullptr, &fodMenu04, &intro04, 6, 1}},
};
MenuItem fodMenu04 = {
	defaultMenuSetup,
	nullptr,
	{{"1. Fill on Demand (Not Active)", nullptr, nullptr, nullptr, 				&mainMenu04, 2, 6},
	 {"2. Select TDS", 					nullptr, nullptr, &selectTDS, 			&mainMenu04, 3, 6},
	 {"3. Set Warnings", 				nullptr, nullptr, &configureWarning, 	&mainMenu04, 1, 6},
	 {"4. Set Auto Flush", 				nullptr, nullptr, &setAutoFlush, 		&mainMenu04, 2, 6},
	 {"5. DI Saver", 					nullptr, nullptr, &selectDiSaver, 		&mainMenu04, 2, 6},
	 {"6. Booster Pump Control", 		nullptr, nullptr, &selectBoosterPump, 	&mainMenu04, 2, 6}}
};
MenuItem selectTDS = {
	defaultMenuSetup,
	nullptr,
	{{"Mains TDS & Temp Control", nullptr, nullptr, &mainsTDS, &fodMenu04, 3, 3},
	 {"RO TDS & Temp Control"	, nullptr, nullptr, &roTDS, &fodMenu04, 3, 3},
	 {"DI TDS & Temp Control"	, nullptr, nullptr, &diTDS, &fodMenu04, 3, 3}}
};
MenuItem fodOptnsOff04 = {
	fodOffSetup_04,
	fodOFF,
	{{"Press OK to commence the purification filling process", nullptr, nullptr, nullptr, &fodMenu04, 2, 1}}
};

MenuItem haltMenu = {
	introSetup_04,
	haltMenuLoop,
	{{"", nullptr, nullptr, nullptr, nullptr, 0, 1}}
};
///-----------------------------Mains TDS-----------------------------

MenuItem mainsTDS = {
	defaultMenuSetup,
	nullptr,
	{{"Mains TDS Threshold", nullptr, nullptr, &mainsTDSThreshold, &selectTDS, 3, 3},
	 {"Mains Temp Control", nullptr, nullptr, &mainsTDSTemperature, &selectTDS, 3, 3},
	 {"Mains TDS Shutdown", nullptr, nullptr, &mainsTDSShutdown, &selectTDS, 2, 3}}
};
MenuItem mainsTDSThreshold = {
	defaultMenuSetup,
	TDSThresholdLoop,
	{{"Off", nullptr, nullptr, nullptr, &mainsTDS, 0, 3},
	 {"Default 500ppm"	, nullptr, nullptr, nullptr, &mainsTDS, 0, 3},
	 {"Adjustable 50-600ppm"	, nullptr, nullptr, nullptr, &mainsTDS, 0, 3}}
};
MenuItem mainsTDSTemperature = {
	defaultMenuSetup,
	TDSTemperatureLoop,
	{{"Off", nullptr, nullptr, nullptr, &mainsTDS, 0, 3},
	 {"Default 2*C", nullptr, nullptr, nullptr, &mainsTDS, 0, 3},
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
	{{"RO TDS Threshold", nullptr, nullptr, &roTDSThreshold, &selectTDS, 3, 3},
	 {"RO Temp Control"	, nullptr, nullptr, &roTDSTemperature, &selectTDS, 3, 3},
	 {"RO TDS Shutdown"	, nullptr, nullptr, &roTDSShutdown, &selectTDS, 2, 3}}
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
	setRoTdsThreshold,
	{{"RO Dynamic Default 92% Rejection", nullptr, nullptr, nullptr, &roTDSThreshold, 0, 2},
	 {"RO Dynamic Adjustable 50-99%", nullptr, nullptr, nullptr, &roTDSThreshold, 0, 2}}
};
MenuItem roFixedTDSThreshold = {
	defaultMenuSetup,
	setRoTdsThreshold,
	{{"RO Fixed Default 50ppm", nullptr, nullptr, nullptr, &roTDSThreshold, 0, 2},
	 {"RO Fixed Adjustable 1-100ppm", nullptr, nullptr, nullptr, &roTDSThreshold, 0, 2}}
};
MenuItem roTDSTemperature = {
	defaultMenuSetup,
	TDSTemperatureLoop,
	{{"Off", nullptr, nullptr, nullptr, &roTDS, 0, 3},
	 {"Default 2*C"	, nullptr, nullptr, nullptr, &roTDS, 0, 3},
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
	{{"DI TDS Threshold", nullptr, nullptr, &diTDSThreshold, &selectTDS, 3, 3},
	 {"DI Temp Control"	, nullptr, nullptr, &diTDSTemperature, &selectTDS, 3, 3},
	 {"DI TDS Shutdown"	, nullptr, nullptr, &diTDSShutdown, &selectTDS, 2, 3}}
};
MenuItem diTDSThreshold = {
	defaultMenuSetup,
	TDSThresholdLoop,
	{{"Off", nullptr, nullptr, nullptr, &diTDS, 0, 3},
	 {"Default 10ppm", nullptr, nullptr, nullptr, &diTDS, 0, 3},
	 {"Adjustable 1-15ppm", nullptr, nullptr, nullptr, &diTDS, 0, 3}}
};
MenuItem diTDSTemperature = {
	defaultMenuSetup,
	TDSTemperatureLoop,
	{{"Off", nullptr, nullptr, nullptr, &diTDS, 0, 3},
	 {"Default 2*C"	, nullptr, nullptr, nullptr, &diTDS, 0, 3},
	 {"Adjustable 0-10*C"	, nullptr, nullptr, nullptr, &diTDS, 0, 3}}
};
MenuItem diTDSShutdown = {
	defaultMenuSetup,
	TdsShutdown,
	{{"Off (default)", nullptr, nullptr, nullptr, &diTDS, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &diTDS, 0, 2}}
};

///-----------------------------TDS END-----------------------------

MenuItem configureWarning = {
	defaultMenuSetup,
	nullptr,
	{{"Select type to configure warning setting:\n\n"
	  "TDS", nullptr, nullptr, &tdsWarningList, &fodMenu04, 7, 1}}
};
MenuItem tdsWarningList = {
	defaultMenuSetup,
	selectTdsWarningItem,
	{{"1. Mains TDS warning", 		nullptr, nullptr, &SetWarnState, &configureWarning, 2, 7},
	 {"2. Mains Temp warning", 		nullptr, nullptr, &SetWarnState, &configureWarning, 2, 7},
	 {"3. RO Fixed TDS warning", 	nullptr, nullptr, &SetWarnState, &configureWarning, 2, 7},
	 {"4. RO Dynamic TDS warning",	nullptr, nullptr, &SetWarnState, &configureWarning, 2, 7},
	 {"5. RO Temp warning", 		nullptr, nullptr, &SetWarnState, &configureWarning, 2, 7},
	 {"6. DI TDS warning", 			nullptr, nullptr, &SetWarnState, &configureWarning, 2, 7},
	 {"7. DI Temp warning", 		nullptr, nullptr, &SetWarnState, &configureWarning, 2, 7}}
};
MenuItem SetWarnState = {
	defaultMenuSetup,
	configureTdsWarning,
	{{"Disable" , nullptr, nullptr, nullptr, &tdsWarningList, 0, 2},
	 {"Enable"	, nullptr, nullptr, nullptr, &tdsWarningList, 0, 2}}
};

///-----------------------------TDS WARNING SETUP END-----------------------------

MenuItem setAutoFlush = {
	defaultMenuSetup,
	autoFlushOptions,
	{{"Off" , nullptr, nullptr, nullptr, &fodMenu04, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &fodMenu04, 0, 2}}
};
MenuItem selectDiSaver = {
	defaultMenuSetup,
	nullptr,
	{{"Di-Saver On/Off" 	, nullptr, nullptr, &setDiSaverOnOff, &fodMenu04, 2, 2},
	 {"Di-Saver Interval"	, nullptr, nullptr, &setDiSaverInterval, &fodMenu04, 2, 2}}
};
MenuItem setDiSaverInterval = {
	defaultMenuSetup,
	diSaverOptions,
	{{"set Di-Saver Interval:\n"
	  "1. Default to 3 min" , nullptr, nullptr, nullptr, &selectDiSaver, 0, 2},
	 {"2. Adjustable Upto 10 min"	, nullptr, nullptr, nullptr, &selectDiSaver, 0, 2}}
};
MenuItem setDiSaverOnOff = {
	defaultMenuSetup,
	diSaverOnOff,
	{{"Off" , nullptr, nullptr, nullptr, &selectDiSaver, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &selectDiSaver, 0, 2}}
};

///---------------------------- EXTRA FLUSH & DI SAVER SETUP END-----------------------------

MenuItem selectBoosterPump = {
	defaultMenuSetup,
	nullptr,
	{{"Booster Production" 	, nullptr, nullptr, &boostProduction, &fodMenu04, 2, 2},
	 {"Booster Auto Flush"	, nullptr, nullptr, &boostAutoFlush, &fodMenu04, 2, 2}}
};
MenuItem boostProduction = {
	defaultMenuSetup,
	boosterProductionOnOff,
	{{"Off" , nullptr, nullptr, nullptr, &selectBoosterPump, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &selectBoosterPump, 0, 2}}
};
MenuItem boostAutoFlush = {
	defaultMenuSetup,
	boosterAutoFlushOnOff,
	{{"Off" , nullptr, nullptr, nullptr, &selectBoosterPump, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &selectBoosterPump, 0, 2}}
};
///---------------------------- BOOSTER PUMP SETUP END-----------------------------

#endif

#endif /* INC_MENULIST_04_H_ */
