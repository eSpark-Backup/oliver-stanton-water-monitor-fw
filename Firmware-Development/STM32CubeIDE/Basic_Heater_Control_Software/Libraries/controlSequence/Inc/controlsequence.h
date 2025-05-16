/*
 * controlsequence.h
 *
 *  	Created on: JAN 02, 2025
 *	  Author: Nehal Abdul Rehman
 */

#ifndef INC_CONTROLSEQUENCE_H_
#define INC_CONTROLSEQUENCE_H_


#if MENU_01
	#include "controls01/Inc/controls01.h"
#elif MENU_02
	#include "controls02/Inc/controls02.h"
#elif (MENU_03 || MENU_04)
	#include "controls03/Inc/controls03.h"
#elif MENU_05
	#include "controls05/Inc/controls05.h"
#elif MENU_07
	#include "controls07/Inc/controls07.h"
#elif LLPDS_TDS_MENU
	#include "pdsTdsControls/Inc/pdsTdsControls.h"
#elif LLTDS_MENU_08
	#include "tdsControls08/Inc/tdsControls08.h"
#else
	#include "controls01/Inc/controls01.h"
#endif

#endif /* INC_CONTROLSEQUENCE_H_ */
