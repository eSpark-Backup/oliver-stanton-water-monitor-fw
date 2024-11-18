#pragma once

#ifndef INC_MENULIST_02_H_
#define INC_MENULIST_02_H_

#include "menu.h"
#include "menu_02.h"


///-----------------------------Creating Menu Items List-----------------------------
// Items in Menu
// { <Label>, <func pointer> , <Next_node>, <Prev_node>, <NextItems>, <Items> }
///----------------------------------------------------------------------------------

/* by default use address of <prev_node> as <Next_node> if there is no next node to the current item
 * If you want to be able to select a range of values in item, set <NextItems> to 0 and set the <Items> from 0-items range
 */

MenuItem autoFlushOptns = {
	nullptr,
	autoFlushLoop,
	{{"Enable", nullptr, nullptr, nullptr, &fodMenu, 0, 5},
	 {"Disable", nullptr, nullptr, nullptr, &fodMenu, 0, 5},
     {"Initial Flush Timer", nullptr, nullptr, &autoFlushTimer, &fodMenu, 2, 5},
	 {"Production Timer", nullptr, nullptr, &productionTimer, &fodMenu, 2, 5},
     {"Subsequent Flush Timer", nullptr, nullptr, &autoFlushTimer, &fodMenu, 2, 5}}
};

MenuItem autoFlushTimer = {
	nullptr,
	autoFlushTimerLoop,
	{{"Default 10 minutes", nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2},
	 {"Adjustable 5-60 minutes"	, nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2}}
};

MenuItem productionTimer = {
	nullptr,
	productionTimerLoop,
	{{"Default 50 minutes", nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2},
	 {"Adjustable 50-150 minutes"	, nullptr, nullptr, nullptr, &autoFlushOptns, 0, 2}}
};

#endif /* INC_MENULIST_02_H_ */
