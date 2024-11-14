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

#endif /* INC_MENULIST_02_H_ */
