#pragma once

#ifndef INC_MENULIST_01_H_
#define INC_MENULIST_01_H_

#include "menu.h"
// #include "mainApp.hpp"
// #include "controls.h"
#include "menu_01.h"
#include "menuList_02.h"

///-----------------------------Creating Menu Items List-----------------------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <NextItems>, <Items> }
///----------------------------------------------------------------------------------

/* by default use address of <prev_node> as <Next_node> if there is no next node to the current item
 * If you want to be able to select a range of values in item, set <NextItems> to 0 and set the <Items> from 0-items range
 */

MenuItem intro = {
	introSetup,
	introPageSelection,
	{{"Water Monitor Control Software", nullptr, nullptr, &mainMenu, nullptr, 1, 1}}
};
MenuItem mainMenu = {
	mainMenuSetup,
	mainMenuSelection,
	{{"Default LLROFS Home Page", nullptr, nullptr, &fodMenu, &intro, 2, 1}},
};
MenuItem fodMenu = {
	defaultMenuSetup,
	nullptr,
	{{"Fill on Demand On/Off", nullptr, nullptr, &fodOptns, &mainMenu, 2, 3},
	 {"Auto Flush Settings", nullptr, nullptr, &autoFlushOptns, &mainMenu, 4, 3},
	 {"RO_Saver", nullptr, nullptr, &roSaver, &mainMenu, 2, 3}}
};
MenuItem fodOptns = {
	nullptr,
	fodON,
	{{"Off", nullptr, nullptr, &fodOptnsOff, &fodMenu, 1, 2},
	 {"On", nullptr, nullptr, nullptr, &fodMenu, 2, 2}}
};
MenuItem fodOptnsOff = {
	fodOffSetup,
	fodOFF,
	{{"Press OK to commence the purification filling process", nullptr, nullptr, nullptr, &fodMenu, 2, 1}}
};
MenuItem roSaver = {
	defaultMenuSetup,
	nullptr,
	{{"roSaver on/off", nullptr, nullptr, &roSaverOptns, &fodMenu, 2, 2},
	 {"roSaver_tds_thres", nullptr, nullptr, &roSaverThresh, &fodMenu, 2, 2}}
};
MenuItem roSaverOptns = {
	nullptr,
	roSaverOptions,
	{{"On", nullptr, nullptr, nullptr, &roSaver, 2, 2},
	 {"Off", nullptr, nullptr, nullptr, &roSaver, 2, 2}}
};
MenuItem roSaverThresh = {
	nullptr,
	setRoSaverThresh,
	{{"Select default 0ppm", nullptr, nullptr, nullptr, &roSaver, 2, 2},
	 {"Adjustable 1-20 ppm", nullptr, nullptr, &roSaverThreshAdjust, &roSaver, 1, 2}}
};
MenuItem roSaverThreshAdjust = {
	backAndForthSetup,
	roSaverThreshAdj,
	{{"Select ppm value from 1-20", nullptr, nullptr, nullptr, &roSaver, 0, 1}}};
MenuItem Battery_Index = {
	batIndexSetup,
	batIndexSelection,
	{{"Default LL-DCPMS Home Page", nullptr, nullptr, &BatMonitorMenu, &intro, 2, 1}}
};
MenuItem BatMonitorMenu = {
	defaultMenuSetup,
	nullptr,
	{{"Battery Low Power Shutdown", nullptr, nullptr, &BatLowMenuOptns, &mainMenu, 2, 2},
	 {"Power Consumption Monitor", nullptr, nullptr, &BatPowerConsumptiontable, &mainMenu, 1, 2}}
};
MenuItem BatLowMenuOptns = {
	nullptr,
	BatLowMenuOption,
	{{"Off", nullptr, nullptr, &BatLowMenuOFF, &BatMonitorMenu, 3, 2},
	 {"On", nullptr, nullptr, &BatMonitorMenu, &BatMonitorMenu, 2, 2}}
};
MenuItem BatLowMenuOFF = {
	nullptr,
	BatLowShutdown,
	{{"Are you sure you want to switch this system critical setting off? ",
	  nullptr, nullptr,  nullptr, &BatMonitorMenu, 2, 3},
	 {"Yes", nullptr, nullptr,  nullptr, &BatMonitorMenu, 2, 3},
	 {"No", nullptr, nullptr,  nullptr, &BatMonitorMenu, 1, 3}},
};
MenuItem BatPowerConsumptiontable = {
	batConsumptionTableSetup,
	nullptr,
	{{"Component Power Consumption Table", nullptr, nullptr, &BatPowerConsumptionOptns, &BatMonitorMenu, 2, 1}}};
MenuItem BatPowerConsumptionOptns = {
	defaultMenuSetup,
	BatPowerMonitor,
	{{"Off", nullptr, nullptr, nullptr, &BatPowerConsumptiontable, 2, 2},
	 {"On", nullptr, nullptr, nullptr, &BatPowerConsumptiontable, 2, 2}}};
//	MenuItem BatPowerConsumptionOptnsON = {
//		{{"Pump One (LL.PDS.01) is drawing 13.2 amps, higher than specification, service required", 	nullptr, 		nullptr, 		BatMonitorMenu, 1, 2}},
//	};

MenuItem DeviceStandByScreen = {
	batConsumptionTableSetup,
	nullptr,
	{{"Device In StandBy", nullptr, nullptr, nullptr, &mainMenu, 0, 1}}
};
MenuItem DeviceShutDown = {
	batConsumptionTableSetup,
	nullptr,
	{{"Device Shutdown Screen", nullptr, nullptr, nullptr, &mainMenu, 0, 1}}
};
MenuItem BatteryChargingDetected = {
	batConsumptionTableSetup,
	nullptr,
	{{"System Battery Charging (%.2f v)", nullptr, nullptr, nullptr, &mainMenu, 0, 1}}
};
MenuItem BatteryChargingDisconnected = {
	nullptr,
	nullptr,
	{{"Charger Disconnected", nullptr, nullptr, nullptr, &mainMenu, 0, 1}}
};
#endif /* INC_MENULIST_01_H_ */

//-------------------------------P. S.----------------------------------
//---------These menu structs need to be declared in menu.h file--------
//----------------------------------------------------------------------
