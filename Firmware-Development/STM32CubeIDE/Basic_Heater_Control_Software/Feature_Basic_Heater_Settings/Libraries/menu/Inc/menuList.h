/*
 * menuList.h
 *
 *  Created on: Dec 20, 2023
 *      Author: Farhan Ahmed
 */
#pragma once

#ifndef INC_MENULIST_H_
#define INC_MENULIST_H_

//#include "menu.h"
#include "mainApp.hpp"
#include "controls.h"



///-----------------------------Creating Menu Items List-----------------------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <NextItems>, <Items> }
///----------------------------------------------------------------------------------

/* by default use address of <prev_node> as <Next_node> if there is no next node to the current item
 * If you want to be able to select a range of values in item, set <NextItems> to 0 and set the <Items> from 0-items range
 */

MenuItem intro[] = {
	{"Heater Control Software", 			nullptr, 		mainMenu, 			nullptr, 1, 1}
};
MenuItem mainMenu[] = {
    {"Default LLROFS Home Page", 			nullptr, 		fodMenu, 			nullptr, 2, 1},
};
MenuItem fodMenu[] = {
    {"Fill on Demand On/Off", 				nullptr, 		fodOptns, 			mainMenu, 2, 2},
    {"RO_Saver",							nullptr, 		roSaver, 			mainMenu, 2, 2},
};
	MenuItem fodOptns[] = {
		{"Off", 								nullptr, 		fodOptnsOff, 		fodMenu, 1, 2},
		{"On", 									fodON, 			nullptr, 			fodMenu, 2, 2},
	};
		MenuItem fodOptnsOff[] = {
			{"Press OK to commence the purification filling process", 	fodOFF, fodMenu, fodOptns, 2, 1},
		};
		//MenuItem fodOptnsOn[] = {
		//    {"Off", 								nullptr, 		fodOptnsOff, 		fodMenu, 1, 2},
		//    {"On", 								fodON, 			nullptr, 			fodMenu, 2, 2},
		//};
	MenuItem roSaver[] = {
		{"roSaver on/off",						nullptr, 		roSaverOptns, 	fodMenu, 2, 2},
		{"roSaver_tds_thres",					nullptr, 		roSaverThresh, 	fodMenu, 2 ,2},
	};
		MenuItem roSaverOptns[] = {
			{"On",									roSaverOn, 		nullptr, 			roSaver, 2, 2},
			{"Off",									roSaverOff, 		nullptr, 			roSaver, 2 ,2},
		};
		MenuItem roSaverThresh[] = {
			{"Select default 0ppm",					roSaverThreshDefault, 	roSaver,			roSaver, 2, 2},
			{"Adjustable 1-20 ppm",					nullptr, 				roSaverThreshAdjust, roSaver, 1 ,2},
		};
			MenuItem roSaverThreshAdjust[] = {
				{"Select ppm value from 1-20",			roSaverThreshAdj, 		nullptr, 		roSaver, 0, 20},
			};

MenuItem Battery_Index[]={{"Default LL-DCPMS Home Page", 			nullptr, 		BatMonitorMenu, 			nullptr, 2, 1}};
MenuItem BatMonitorMenu[] = {
    {"Battery Low Power Shutdown", 				nullptr, 		BatLowMenuOptns, 			mainMenu, 2, 2},
    {"Power Consumption Monitor",							nullptr, 		BatPowerConsumptiontable, 			mainMenu, 1, 1},
};
	MenuItem BatLowMenuOptns[] = {
		{"Off", 								nullptr, 		BatLowMenuOFF, 		BatMonitorMenu, 3, 2},
		{"On", 									BatLowShutdownOn, 			BatMonitorMenu, 			BatMonitorMenu, 2, 2},
	};
		MenuItem BatLowMenuOFF[] = {
				{"Are you sure you want to switch this system critical setting off? ", 	nullptr, BatLowMenuOptns, BatLowMenuOptns, 2, 2},
				{"Yes", 								BatLowShutdownOff, 		BatLowMenuOptns, 		BatLowMenuOptns, 2, 1},
				{"No", 								BatLowShutdownOn, 			BatLowMenuOptns, 			BatLowMenuOptns, 1, 1},
			};
	MenuItem BatPowerConsumptiontable[] = {
			   {"Component Power Consumption Table", 								nullptr, 		BatPowerConsumptionOptns, 		BatMonitorMenu, 2, 2},
			};
	MenuItem BatPowerConsumptionOptns[] = {
		{"Off", 								BatPowerMonitorOff, 		BatPowerConsumptiontable, 		BatPowerConsumptiontable,2, 2}, //BatPowerConsumptionOptnsON
		{"On", 									BatPowerMonitorOn, 			nullptr, 			BatPowerConsumptiontable,
				2, 2},
		};
//	MenuItem BatPowerConsumptionOptnsON[] = {
//		{"Pump One (LL.PDS.01) is drawing 13.2 amps, higher than specification, service required", 	nullptr, 		nullptr, 		BatMonitorMenu, 1, 2},
//	};


	MenuItem DeviceStandByScreen[] = {
		   {"Device In StandBy", 								nullptr, 		mainMenu, 		mainMenu, 1, 1},
		};
	MenuItem DeviceShutDown[] = {
			   {"Device Shutdown Screen", 								nullptr, 		mainMenu, 		mainMenu, 1, 1},
			};
	MenuItem BatteryChargingDetected[] = {
			   {"System Battery Charging (%.2f v)", 								nullptr, 		mainMenu, 		mainMenu, 1, 1},
			};
	MenuItem BatteryChargingDisconnected[] = {
			   {"Charger Disconnected", 								nullptr, 		mainMenu, 		mainMenu, 1, 1},
			};
#endif /* INC_MENULIST_H_ */


//-------------------------------P. S.----------------------------------
//---------These menu structs need to be declared in menu.h file--------
//----------------------------------------------------------------------
