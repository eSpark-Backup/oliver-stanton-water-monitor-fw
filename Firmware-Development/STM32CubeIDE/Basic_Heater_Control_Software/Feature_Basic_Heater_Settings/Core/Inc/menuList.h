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



///-----------------------------------Creating Menu Items List-------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <ParentItems>, <Items> }
MenuItem fodOptnsOff[] = {
    {"Press OK to commence the purification filling process", 	PureFill, fodMenu, fodOptns, 2, 1},
};
MenuItem fodOptns[] = {
    {"On", 					fodON, nullptr, 	fodMenu, 2, 2},
    {"Off", 				nullptr, fodOptnsOff, 	fodMenu, 2, 2},
};
MenuItem ro_saver[] = {
    {"This",				nullptr, nullptr, 	fodMenu, 2, 2},
    {"That",				nullptr, nullptr, 	fodMenu, 2 ,2},
};
MenuItem fodMenu[] = {
    {"Fill on Demand", 		nullptr, fodOptns, mainMenu, 1, 2},
    {"ro_saver_tds_thres",	nullptr, ro_saver, mainMenu, 1, 2},
};
MenuItem mainMenu[] = {
    {"Default LLROFS Home Page", nullptr, fodMenu, nullptr, 0, 1},
};
MenuItem Intro[] = {
	{"Heater Control Software", nullptr, mainMenu, nullptr, 0, 1}
};



#endif /* INC_MENULIST_H_ */
