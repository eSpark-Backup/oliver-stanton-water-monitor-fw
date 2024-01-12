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
 * If you want to be able to select a range of values in item set <NextItemss> to 0 and set the <Items> from 0-items range
 */

MenuItem Intro[] = {
	{"Heater Control Software", 			nullptr, 		mainMenu, 			nullptr, 1, 1}
};
MenuItem mainMenu[] = {
    {"Default LLROFS Home Page", 			nullptr, 		fodMenu, 			nullptr, 2, 1},
};
MenuItem fodMenu[] = {
    {"Fill on Demand On/Off", 				nullptr, 		fodOptns, 			mainMenu, 2, 2},
    {"RO_Saver",							nullptr, 		ro_saver, 			mainMenu, 2, 2},
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
		//    {"On", 									fodON, 			nullptr, 			fodMenu, 2, 2},
		//};
	MenuItem ro_saver[] = {
		{"RO_Saver on/off",						nullptr, 		ro_saver_Optns, 	fodMenu, 2, 2},
		{"ro_saver_tds_thres",					nullptr, 		ro_saver_thresh, 	fodMenu, 2 ,2},
	};
		MenuItem ro_saver_Optns[] = {
			{"On",									ro_saver_On, 		nullptr, 			ro_saver, 2, 2},
			{"Off",									ro_saver_Off, 		nullptr, 			ro_saver, 2 ,2},
		};
		MenuItem ro_saver_thresh[] = {
			{"Select default 0ppm",					ro_saver_thresh_default, 	ro_saver,			ro_saver, 2, 2},
			{"Adjustable 1-20 ppm",					nullptr, 		ro_saver_thresh_adjust, 	ro_saver, 1 ,2},
		};
			MenuItem ro_saver_thresh_adjust[] = {
				{"Select ppm value from 1-20",			ro_saver_thresh_adj, 		nullptr, 			ro_saver, 0, 20},
			};



#endif /* INC_MENULIST_H_ */


//-------------------------------P. S.----------------------------------
//---------These menu structs need to be declared in menu.h file-------------
//----------------------------------------------------------------------
