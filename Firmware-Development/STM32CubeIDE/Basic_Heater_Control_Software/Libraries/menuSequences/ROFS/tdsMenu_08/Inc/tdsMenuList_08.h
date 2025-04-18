#pragma once

#ifndef INC_TDSMENULIST_08_H_
#define INC_TDSMENULIST_08_H_

#include "stm32f0xx_hal_conf.h"
#if LLTDS_MENU_08 == 1

#include "menu.h"
// #include "mainApp.hpp"
// #include "controls.h"
#include "tdsMenu_08.h"


///-----------------------------Creating Menu Items List-----------------------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <NextItems>, <Items> }
///----------------------------------------------------------------------------------

/* by default use address of <prev_node> as <Next_node> if there is no next node to the current item
 * If you want to be able to select a range of values in item, set <NextItems> to 0 and set the <Items> from 0-items range
 */

MenuItem intro02 = {
	introSetup_02,
	introPageSelection_02,
	{{"Water Monitor Control Software", nullptr, nullptr, &mainMenu02, nullptr, 1, 1}}
};
MenuItem mainMenu02 = {
	mainMenuSetup_02,
	mainMenuSelection_02,
	{{"Default LLROFS Home Page", nullptr, nullptr, &fodMenu02, &intro02, 5, 1}},
};
MenuItem fodMenu02 = {
	defaultMenuSetup,
	nullptr,
	{{"1. Fill on Demand (Not Active)", nullptr, nullptr, nullptr, 				&mainMenu02, 2, 5},
	 {"2. Select TDS", 					nullptr, nullptr, &selectTDS, 			&mainMenu02, 3, 5},
	 {"3. Set Warnings", 				nullptr, nullptr, &configureWarning, 	&mainMenu02, 1, 5},
	 {"4. Auto Flush", 					nullptr, nullptr, &autoFlushOptns, 		&mainMenu02, 4, 5},
	 {"5. DI Saver", 					nullptr, nullptr, &selectDiSaver, 		&mainMenu02, 2, 5}}
};
MenuItem selectTDS = {
	defaultMenuSetup,
	nullptr,
	{{"Mains TDS & Temp Control", nullptr, nullptr, &mainsTDS, &fodMenu02, 3, 3},
	 {"RO TDS & Temp Control"	, nullptr, nullptr, &roTDS, &fodMenu02, 3, 3},
	 {"DI TDS & Temp Control"	, nullptr, nullptr, &diTDS, &fodMenu02, 3, 3}}
};
MenuItem fodOptnsOff02 = {
	fodOffSetup_02,
	fodOFF_02,
	{{"Press OK to commence the purification filling process", nullptr, nullptr, nullptr, &fodMenu02, 2, 1}}
};

MenuItem haltMenu = {
	introSetup_02,
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
	  "TDS", nullptr, nullptr, &tdsWarningList, &fodMenu02, 7, 1}}
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

MenuItem selectDiSaver = {
	defaultMenuSetup,
	nullptr,
	{{"Di-Saver On/Off" 	, nullptr, nullptr, &setDiSaverOnOff, &fodMenu02, 2, 2},
	 {"Di-Saver Interval"	, nullptr, nullptr, &setDiSaverInterval, &fodMenu02, 2, 2}}
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

MenuItem autoFlushOptns = {
	defaultMenuSetup,
	nullptr,
	{{"Set AutoFlush On/Off", 	nullptr, nullptr, &setAutoFlush,		&fodMenu02, 2, 4},
	{"Initial Flush Timer", 	nullptr, nullptr, &initialFlushTimer, 	&fodMenu02, 2, 4},
	{"Production Timer", 		nullptr, nullptr, &productionTimer, 	&fodMenu02, 2, 4},
	{"Subsequent Flush Timer", 	nullptr, nullptr, &subsequentFlushTimer,&fodMenu02, 2, 4}}
};
MenuItem setAutoFlush = {
	defaultMenuSetup,
	autoFlushOptions,
	{{"Disable" , 	nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2},
	 {"Enable"	, 	nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2}}
};
MenuItem initialFlushTimer = {
	defaultMenuSetup,
	autoFlushTimerLoop,
	{{"Set Initial flush cycle time:\n\n"
	 "Default to 10 minutes",		 	nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2},
	{"Adjustable 5-60 minutes",			nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2}}
};
MenuItem subsequentFlushTimer = {
	defaultMenuSetup,
	autoFlushTimerLoop,
	{{"Set Subsequent flush cycle time:\n\n"
	 "Default to 10 minutes",		nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2},
	{"Adjustable 5-60 minutes",		nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2}}
};
MenuItem productionTimer = {
	defaultMenuSetup,
	productionTimerLoop,
	{{"Set Production time between cycles:\n\n"
	 "Default to 50 minutes", 		nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2},
	{"Adjustable 50-150 minutes", 	nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2}}
};

///---------------------------- AUTO FLUSH SETUP END-----------------------------

#endif

#endif /* INC_MENULIST_02_H_ */
