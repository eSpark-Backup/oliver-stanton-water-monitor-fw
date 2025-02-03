/*
 * controlsequence.h
 *
 *  	Created on: JAN 02, 2025
 *	  Author: Nehal Abdul Rehman
 */

#ifndef INC_CONTROLSEQUENCE_H_
#define INC_CONTROLSEQUENCE_H_


#if MENU_01 == 1
	#include "controls01/Inc/controls01.h"
#elif MENU_03 == 1
	#include "controls03/Inc/controls03.h"
#elif MENU_05 == 1
	#include "controls05/Inc/controls05.h"
#else

#endif

#endif /* INC_CONTROLSEQUENCE_H_ */
