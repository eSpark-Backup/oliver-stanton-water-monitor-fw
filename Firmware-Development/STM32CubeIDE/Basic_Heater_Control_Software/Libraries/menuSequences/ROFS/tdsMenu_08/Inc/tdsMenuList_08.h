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

MenuItem llTds_intro08 = {
	llTds_introSetup_08,
	llTds_introPageSelection_08,
	{{"Water Monitor Control Software", nullptr, nullptr, &llTds_mainMenu08, nullptr, 1, 1}}
};
MenuItem llTds_mainMenu08 = {
	llTds_mainMenuSetup_08,
	llTds_mainMenuSelection_08,
	{{"Default LLROFS Home Page", nullptr, nullptr, &llTds_purificationMenu08, &llTds_intro08, 5, 1}},
};
MenuItem llTds_purificationMenu08 = {
	defaultMenuSetup,
	nullptr,
	{{"1. Fill on Demand (Not Active)", nullptr, nullptr, nullptr,				&llTds_mainMenu08, 2, 6},
	 {"2. Select TDS (Not Active)",		nullptr, nullptr, nullptr, 				&llTds_mainMenu08, 0, 6},
	 {"3. Set Warnings (Not Active)",	nullptr, nullptr, nullptr, 				&llTds_mainMenu08, 0, 6},
	 {"4. Auto Flush (Not Active)", 	nullptr, nullptr, nullptr, 				&llTds_mainMenu08, 2, 6},
	 {"5. DI Saver (Not Active)", 		nullptr, nullptr, nullptr, 				&llTds_mainMenu08, 0, 6},
	 {"6. Dynamic Waste Ratio Control", nullptr, nullptr, &ctrlWasteRatio,  	&llTds_mainMenu08, 4, 6}}
};

MenuItem llTds_fodOptnsOff08 = {
	defaultMenuSetup,
	autoFillOptions,
	{{"Off", nullptr, nullptr, nullptr, &llTds_purificationMenu08, 0, 2},
	 {"On", nullptr, nullptr, nullptr,  &llTds_purificationMenu08, 0, 2}}
};

MenuItem llTds_setAutoFlush08 = {
	defaultMenuSetup,
	autoFlushOptions,
	{{"Off" , nullptr, nullptr, nullptr, &llTds_purificationMenu08, 0, 2},
	 {"On"	, nullptr, nullptr, nullptr, &llTds_purificationMenu08, 0, 2}}
};

MenuItem ctrlWasteRatio = {
	defaultMenuSetup,
	setWasteRatio,
	{{"Set Waste Control On/Off", 	nullptr, nullptr, &wasteOptnsOff08,		  &llTds_purificationMenu08, 2, 4},
	 {"Set 60 pure/40 waste", 		nullptr, nullptr, nullptr,			 	  &llTds_purificationMenu08, 0, 4},
	 {"Set 50 pure/50 waste", 		nullptr, nullptr, nullptr,			 	  &llTds_purificationMenu08, 0, 4},
	 {"Set Dynamic Ratio", 			nullptr, nullptr, &dynamicWasteRatioMenu, &llTds_purificationMenu08, 1, 4}}
};

MenuItem wasteOptnsOff08 = {
	defaultMenuSetup,
	llTdsWasteSelOpt,
	{{"Disable" , 	nullptr, nullptr, nullptr, &ctrlWasteRatio, 0, 2},
	 {"Enable"	, 	nullptr, nullptr, nullptr, &ctrlWasteRatio, 0, 2}}
};

MenuItem dynamicWasteRatioMenu = {
	llTds_backAndForthSetup_08,
	setDynamicWasteRatio,
	{{"Select Waste ratio (above 40%)", nullptr, nullptr, nullptr, &ctrlWasteRatio, 0, 1}}
};

#endif

#endif /* INC_MENULIST_08_H_ */
