#pragma once

#ifndef INC_MENULIST__H_
#define INC_MENULIST__H_

#include "stm32f0xx_hal_conf.h"
#if LLPDS_TDS_MENU == 1

#include "menu.h"
// #include "mainApp.hpp"
// #include "controls.h"
#include "pdsTdsMenu.h"


///-----------------------------Creating Menu Items List-----------------------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <NextItems>, <Items> }
///----------------------------------------------------------------------------------

/* by default use address of <prev_node> as <Next_node> if there is no next node to the current item
 * If you want to be able to select a range of values in item, set <NextItems> to 0 and set the <Items> from 0-items range
 */

MenuItem pdsTds_intro = {
	pdsTds_introSetup,
	pdsTds_introPageSelection,
	{{"Water Monitor Control Software", nullptr, nullptr, &pdsTds_mainMenu, nullptr, 1, 1}}
};
MenuItem pdsTds_mainMenu = {
	pdsTds_mainMenuSetup,
	pdsTds_mainMenuSelection,
	{{"Default LL-DCPMS Home Page", nullptr, nullptr, &pdsTds_Menu, &intro, 3, 1}}
};
MenuItem pdsTds_Menu = {
	defaultMenuSetup,
	resetWarnLoop,
	{{"1. Delivery TDS & Temperature Control", nullptr, nullptr, &pdsTds_delTDS, &pdsTds_mainMenu, 5, 3},
	 {"2. Reset Warnings", nullptr, nullptr, nullptr, &pdsTds_mainMenu, 0, 3},
	 {"3. Pump Control", nullptr, nullptr, &pumpCntrlOptns, &pdsTds_mainMenu, 3, 3}}
};
MenuItem pdsTds_haltMenu = {
	pdsTds_introSetup,
	pdsTds_haltMenuLoop,
	{{"", nullptr, nullptr, nullptr, nullptr, 0, 1}}
};
///-----------------------------Delivery TDS-----------------------------

MenuItem pdsTds_delTDS = {
	defaultMenuSetup,
	nullptr,
	{{"Delivery TDS Threshold", nullptr, nullptr, &pdsTds_delTDSThreshold, &pdsTds_Menu, 3, 5},
	 {"Temp Threshold for Delivery Water Output", nullptr, nullptr, &pdsTds_delTDSTemperature, &pdsTds_Menu, 3, 5},
	 {"Delivery TDS Shutdown", nullptr, nullptr, &pdsTds_delTDSShutdown, &pdsTds_Menu, 2, 5},
	 {"Delivery Temperature Shutdown", nullptr, nullptr, &pdsTds_delTempShutdown, &pdsTds_Menu, 2, 5},
	 {"Cold Weather Dynamics", nullptr, nullptr, &pdsTds_coldWeatherDynamics, &pdsTds_Menu, 2, 5}}
};
MenuItem pdsTds_delTDSThreshold = {
	defaultMenuSetup,
	delTDSThreshold,
	{{"Off", nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 3},
	 {"Default 10ppm"	, nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 3},
	 {"Adjustable 0-50ppm"	, nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 3}}
};
MenuItem pdsTds_delTDSTemperature = {
	defaultMenuSetup,
	delTDSTemperature,
	{{"Off", nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 3},
	 {"Default 2*C", nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 3},
	 {"Adjustable 0-10*C"	, nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 3}}
};
MenuItem pdsTds_delTDSShutdown = {
	defaultMenuSetup,
	delShutdown,
	{{"Off (default)", nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 2}}
};
MenuItem pdsTds_delTempShutdown = {
	defaultMenuSetup,
	delShutdown,
	{{"Off (default)", nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 2}}
};
MenuItem pdsTds_coldWeatherDynamics = {
	defaultMenuSetup,
	delShutdown,
	{{"Off (default)", nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &pdsTds_delTDS, 0, 2}}
};
///-----------------------------Delivery Pump-----------------------------

MenuItem pumpCntrlOptns = {
	defaultMenuSetup,
	delPump,
	{{"Off", nullptr, nullptr, nullptr, &pdsTds_Menu, 0, 3},
	 {"On", nullptr, nullptr, nullptr, &pdsTds_Menu, 0, 3},
	 {"Remote Control Disabled", nullptr, nullptr, nullptr, &pdsTds_Menu, 0, 3}}
};

#endif

#endif /* INC_MENULIST__H_ */
