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
	{{"Default LLROFS Home Page", nullptr, nullptr, &tdsChoice, &llTds_intro08, 2, 1}}
};

MenuItem tdsChoice = {
	ctrlWasteRatioToggleSetup,
	nullptr,
	{{"Settings", nullptr, nullptr, &llTds_purificationMenu08, &llTds_mainMenu08, 7, 2},
	 {"Flow Meter Menu", nullptr, nullptr, &flowMeterMenu,  &llTds_mainMenu08, 3, 2}}
};


MenuItem flowMeterMenu ={
	flowMeterMenuSetup,
	nullptr,
	{{"Mains Input Flow", nullptr, nullptr, &mainsInputFlow, &tdsChoice, 3, 3},
	 {"Pure Output Flow", nullptr, nullptr, &pureOutputFlow,  &tdsChoice, 3, 3},
	 {"Waste Output Flow", nullptr, nullptr, &wasteOutputFlow,  &tdsChoice, 3, 3}}
};

MenuItem mainsInputFlow = {
	mainsInputSetup,
	nullptr,
	{{"Live Flow", nullptr, nullptr, &liveFlow, &flowMeterMenu, 1, 3},
	{"Overall Flow (since service)", nullptr, nullptr, &overAllFlowSinceService,  &flowMeterMenu, 1, 3},
	{"Overall Flow (since new)", nullptr, nullptr, &overAllFlowSinceNew,  &flowMeterMenu, 1, 3}}
};

MenuItem pureOutputFlow = {
	pureOutputSetup,
	nullptr,
	{{"Live Flow", nullptr, nullptr, &liveFlow, &flowMeterMenu, 1, 3},
	{"Overall Flow (since service)", nullptr, nullptr, &overAllFlowSinceService,  &flowMeterMenu, 1, 3},
	{"Overall Flow (since new)", nullptr, nullptr, &overAllFlowSinceNew,  &flowMeterMenu, 1, 3}}
};

MenuItem wasteOutputFlow = {
	wasteOutputSetup,
	nullptr,
	{{"Live Flow", nullptr, nullptr, &liveFlow, &flowMeterMenu, 1, 3},
	{"Overall Flow (since service)", nullptr, nullptr, &overAllFlowSinceService,  &flowMeterMenu, 1, 3},
	{"Overall Flow (since new)", nullptr, nullptr, &overAllFlowSinceNew,  &flowMeterMenu, 1, 3}}
};

MenuItem liveFlow = {
	llTds_mainMenuSetup_08,
	liveFlowLoop,
	{{"", nullptr, nullptr, nullptr, &flowMeterMenu, 0, 1}}
};
MenuItem overAllFlowSinceService = {
	llTds_mainMenuSetup_08,
	overAllFlowSinceServiceLoop,
	{{"", nullptr, nullptr, nullptr, &flowMeterMenu, 0, 1}}
};
MenuItem overAllFlowSinceNew = {
	llTds_mainMenuSetup_08,
	overAllFlowSinceNewLoop,
	{{"", nullptr, nullptr, nullptr, &flowMeterMenu, 0, 1}}
};



MenuItem llTds_purificationMenu08 = {
	defaultMenuSetup, 
	nullptr,
	{{"1. Fill on Demand (Not Active)", nullptr, nullptr, nullptr,					&llTds_mainMenu08, 2, 7},
	 {"2. Select TDS (Not Active)",		nullptr, nullptr, nullptr, 					&llTds_mainMenu08, 0, 7},
	 {"3. Set Warnings (Not Active)",	nullptr, nullptr, nullptr, 					&llTds_mainMenu08, 0, 7},
	 {"4. Auto Flush (Not Active)", 	nullptr, nullptr, nullptr, 					&llTds_mainMenu08, 2, 7},
	 {"5. DI Saver (Not Active)", 		nullptr, nullptr, nullptr, 					&llTds_mainMenu08, 0, 7},
	 {"6. Dynamic Waste Ratio Control", nullptr, nullptr, &ctrlDynamicWasteRatio,  	&llTds_mainMenu08, 4, 7},
	 {"7. Waste Ratio Control",			nullptr, nullptr, &ctrlWasteRatioToggle,  	&llTds_mainMenu08, 2, 7}}
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


MenuItem ctrlWasteRatioToggle = {
	ctrlWasteRatioToggleSetup,
	nullptr,
	{{"On" , 	nullptr, nullptr, &wasteRatioAlerts, &llTds_purificationMenu08, 2, 2},
	 {"Off"	, 	nullptr, nullptr, &wasteRatioOff, &llTds_purificationMenu08, 1, 2}}
};

MenuItem wasteRatioAlerts= {
	wasteRatioAlertsSetup,
	nullptr,
	{{"1. Set Alerts" , nullptr, nullptr, &alertsToggle, &ctrlWasteRatioToggle, 2, 2},
	 {"2. Set Alerts and Shutdown", nullptr, nullptr, &alertsAndShutdownToggle, &ctrlWasteRatioToggle, 2, 2}}
};

MenuItem alertsToggle = {
	ctrlWasteRatioToggleSetup,
	nullptr,
	{{"Enable Alerts" , 	enableAlerts, nullptr, nullptr, &wasteRatioAlerts, 0, 2},
	 {"Disable Alerts"	, 	disableAlerts , nullptr, nullptr, &wasteRatioAlerts, 0, 2}}
};

MenuItem alertsAndShutdownToggle = {
	ctrlWasteRatioToggleSetup,
	nullptr,
	{{"Enable Alerts and Shutdown" , 	enableAlertsAndShutDown, nullptr, nullptr, &wasteRatioAlerts, 0, 2},
	 {"Disable Alerts and Shutdown"	, 	disableAlertsAndShutDown, nullptr, nullptr, &wasteRatioAlerts, 0, 2}}
};

MenuItem wasteRatioOff = {
	wasteRatioOffSetup,
	nullptr,
	{{"Waste Ratio Control Turned Off (No notifciations/alerts will be dislayed)" , 	nullptr, nullptr, nullptr, &ctrlWasteRatioToggle, 0, 2}}
};


MenuItem ctrlDynamicWasteRatio = {
	defaultMenuSetup,
	setWasteRatio,
	{{"Set Dynamic Waste Control On/Off", 	nullptr, nullptr, &dynamicWasteOptnsOff08,	&llTds_purificationMenu08, 2, 4},
	 {"Set 60 pure/40 waste", 				nullptr, nullptr, nullptr,			 	  	&llTds_purificationMenu08, 0, 4},
	 {"Set 50 pure/50 waste", 				nullptr, nullptr, nullptr,			 	  	&llTds_purificationMenu08, 0, 4},
	 {"Set Dynamic Ratio", 					nullptr, nullptr, &dynamicWasteRatioMenu, 	&llTds_purificationMenu08, 1, 4}}
};

MenuItem dynamicWasteOptnsOff08 = {
	defaultMenuSetup,
	llTdsWasteSelOpt,
	{{"Disable" , 	nullptr, nullptr, nullptr, &ctrlDynamicWasteRatio, 0, 2},
	 {"Enable"	, 	nullptr, nullptr, nullptr, &ctrlDynamicWasteRatio, 0, 2}}
};

MenuItem dynamicWasteRatioMenu = {
	llTds_backAndForthSetup_08,
	setDynamicWasteRatio,
	{{"Select Waste ratio (above 40%)", nullptr, nullptr, nullptr, &ctrlDynamicWasteRatio, 0, 1}}
};

#endif

#endif /* INC_MENULIST_08_H_ */
