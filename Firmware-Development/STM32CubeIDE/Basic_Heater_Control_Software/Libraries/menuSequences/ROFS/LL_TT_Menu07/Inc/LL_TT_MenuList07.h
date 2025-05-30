#pragma once

#ifndef _LL_TT_MENULIST07_H_
#define _LL_TT_MENULIST07_H_

#include "stm32f0xx_hal_conf.h"
#if LL_TT_MENU_07 == 1

#include "menu.h"
// #include "mainApp.hpp"
// #include "controls.h"
#include "LL_TT_Menu07.h"


///-----------------------------Creating Menu Items List-----------------------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <NextItems>, <Items> }
///----------------------------------------------------------------------------------

/* by default use address of <prev_node> as <Next_node> if there is no next node to the current item
 * If you want to be able to select a range of values in item, set <NextItems> to 0 and set the <Items> from 0-items range
 */

MenuItem remote_TT_intro07 = {
	remote_TT_introSetup07,
	remote_TT_introPageSelection07,
	{{"Water Monitor Control Software", nullptr, nullptr, &remote_TT_mainMenu07, nullptr, 1, 1}}
};
MenuItem remote_TT_mainMenu07 = {
	remote_TT_mainMenuSetup07,
	remote_TT_mainMenuSelection07,
	{{"Remote Fob Menu", nullptr, nullptr, nullptr, &remote_TT_intro07, 0, 1}}
};

#endif

#endif /* _LL_TT_MENULIST07_H_ */
