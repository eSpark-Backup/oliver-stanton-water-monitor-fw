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
#elif (MENU_03 || MENU_04)
	#include "controls03/Inc/controls03.h"
#elif MENU_05
	#include "controls05/Inc/controls05.h"
#else
	#include "controls01/Inc/controls01.h"
#endif

extern tempState perpStatus;
#endif /* INC_CONTROLSEQUENCE_H_ */
