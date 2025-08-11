#pragma once



#ifdef __cplusplus
extern "C" {
#endif
	
#include "wbus.h"

#ifdef __cplusplus
}
#endif

#include "main.h"
#include "controls_LL_TCHS.h"
#include "menu.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
  IDLE = 1,
	CMD_ID,
	CMD_ERR_LS,
	CMD_ERR_DEL,
	CMD_TEST,
	CMD_PH, /* park heating */
	CMD_SH, /* supplemental heating */
	CMD_VENT, /* Ventilation */
	CMD_CHECK,
	CMD_STOP,
	CMD_MONITOR,
	CMD_MONITOR_SINGLE,
	CMD_EEPROM_RD,
	CMD_EEPROM_WR
} wbtool_cmd; 

extern wbtool_cmd heaterCmd;


void heaterLoop();
void sendHeaterCommand(wbtool_cmd heaterCommand , uint8_t heaterOnTimeMin);

