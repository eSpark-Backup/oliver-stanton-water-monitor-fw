
#include "heater_wb.h"


#define UART_HANDLE huart3

wb_info_t i;
wb_errors_t e;
int8_t err = 0;
int8_t test = 0, dev = 0, sensor = 0;
uint8_t heaterOnMins = 1; 
float tval = 1.0f;
char opt;
char text[1024];
unsigned char eeprom_data_wr[2];
bool heaterRunning= false;

wbtool_cmd heaterCmd = IDLE;


void sendHeaterCommand(wbtool_cmd heaterCommand , uint8_t heaterOnTimeMin)
{
    heaterCmd=heaterCommand;
    heaterOnMins= heaterOnTimeMin;
}

void printCurrentCommand()
{
    switch (heaterCmd)
    {
        case IDLE:
            menu.Display_Log("[Command] IDLE\n");
            break;

        case CMD_ID:
            menu.Display_Log("[Command] CMD_ID (Read Heater ID)\n");
            break;

        case CMD_EEPROM_RD:
            menu.Display_Log("[Command] CMD_EEPROM_RD (Read EEPROM)\n");
            break;

        case CMD_ERR_LS:
            menu.Display_Log("[Command] CMD_ERR_LS (List Error Codes)\n");
            break;

        case CMD_ERR_DEL:
            menu.Display_Log("[Command] CMD_ERR_DEL (Delete Error Codes)\n");
            break;

        case CMD_TEST:
            menu.Display_Log("[Command] CMD_TEST (Run Test Mode)\n");
            break;

        case CMD_PH:
            menu.Display_Log("[Command] CMD_PH (Park Heater )\n");
            break;

        case CMD_SH:
            menu.Display_Log("[Command] CMD_SH (Start Heater)\n");
            break;

        case CMD_VENT:
            menu.Display_Log("[Command] CMD_VENT (Ventilation Mode)\n");
            break;

        case CMD_STOP:
            menu.Display_Log("[Command] CMD_STOP (Stop Heater)\n");
            break;

        case CMD_MONITOR:
            menu.Display_Log("[Command] CMD_MONITOR (Live Monitoring)\n");
            break;

        case CMD_MONITOR_SINGLE:
            menu.Display_Log("[Command] CMD_MONITOR_SINGLE (Single Monitor Reading)\n");
            break;

        default:
            menu.Display_Log("[Command] UNKNOWN (0x%02X)\n", heaterCmd);
            break;
    }
}


void commandExecute(void)
{
    switch (heaterCmd)
    {
        case IDLE:
          break;
        case CMD_ID:
            /* Retrieve device indenty info */
            err = wbus_get_wbinfo(UART_HANDLE, &i);
            if (err == 0)
            {
              int a;
              for (a=0; a<WB_IDENT_LINES; a++)
              {
                wbus_ident_print(text, &i, a);
                menu.Display_Log("%s\n", text);
              }
            } else {
              menu.Display_Log("wbus_get_wbinfo() failed\n");
            }
            heaterCmd = IDLE;
            break;
        case CMD_EEPROM_RD:
            {
              int a;
              unsigned char eeprom_data[2];
    
              for (a=0; a<32; a++)
              {
                err = wbus_eeprom_read(UART_HANDLE, a, eeprom_data);
                if (err == 0) {
                  menu.Display_Log("%x ", eeprom_data[a]);
                } else {
                    heaterCmd = IDLE;

                  break;
                }
              }
            }
            heaterCmd = IDLE;
            break;
        case CMD_ERR_LS:
            /* read error codes  */
            wbus_errorcodes_read(UART_HANDLE, &e);
            {
              int a;
              
              for (a=0; a<e.nErr; a++)
              {
                wbus_errorcode_print(text, &e, a);
                menu.Display_Log("%s\n", text);
              }
            }
            heaterCmd = IDLE;
            break;
    
        case CMD_ERR_DEL:
            /* Delete error codes */
            err = wbus_errorcodes_clear(UART_HANDLE);
            heaterCmd = IDLE;
            break;
        case CMD_TEST:
            menu.Display_Log("Testing device %d for 1 sec with value %f\n", test , tval);
            err = wbus_test(UART_HANDLE, test, 1, tval); //1 second
            heaterCmd = IDLE;
            break;
        case CMD_PH:
            err = wbus_turnOn(UART_HANDLE, WBUS_PH, heaterOnMins);
            heaterRunning= true;
            break;
        case CMD_SH:
            err = wbus_turnOn(UART_HANDLE, WBUS_SH, heaterOnMins);
            heaterRunning= true;
            break;
        case CMD_VENT:
            err = wbus_turnOn(UART_HANDLE, WBUS_VENT, heaterOnMins);
            heaterRunning= true;
            break;
        case CMD_STOP:
            err = wbus_turnOff(UART_HANDLE);
            heaterCmd = IDLE;
            break;
        case CMD_MONITOR:
            {
            int i;
            wb_sensor_t s;
            for (i=0; i<20; i++)
            {
              wbus_sensor_read(UART_HANDLE, &s, i);
              wbus_sensor_print(text, &s);
              menu.Display_Log("sensor[%d] %s\n", i, text);
            }
            }
            heaterCmd = IDLE;
            break;
        case CMD_MONITOR_SINGLE:
            {
            wb_sensor_t s;
            wbus_sensor_read(UART_HANDLE, &s, sensor);
            wbus_sensor_print(text, &s);
            menu.Display_Log("sensor[%d] %s\n", sensor, text);
            }
            heaterCmd = IDLE;
            break;
        default:
            break;
            
    }

}

int heaterUnderOperation()
{
  	switch (heaterCmd) {
		case CMD_PH:
			err = wbus_check(UART_HANDLE, WBUS_PH);
			break;
		case CMD_SH:
			err = wbus_check(UART_HANDLE, WBUS_SH);
			break;
		case CMD_VENT:
			err = wbus_check(UART_HANDLE, WBUS_VENT);
			break;
		default:
			err = -1;
			break;
		}
		if (err) {
		  // menu.Display_Log("stop\n");
      heaterCmd = IDLE;
      heaterRunning= false;
		  return err;
		} 
		{
		  wb_sensor_t s;
		  wbus_sensor_read(UART_HANDLE, &s, 3);
		  wbus_sensor_print(text, &s);
		  menu.Display_Log("%s\n", text);
		  wbus_sensor_read(UART_HANDLE, &s, 5);
		  wbus_sensor_print(text, &s);
		  menu.Display_Log("%s\n", text);
		  wbus_sensor_read(UART_HANDLE, &s, 7);
		  wbus_sensor_print(text, &s);
		  menu.Display_Log("%s\n", text);
		}
		menu.Display_Log(" HEATER UNDER OPERATION\n");
	
	return err;
}


void heaterLoop()
{
    if(!heaterRunning){
      commandExecute();
    }
    else{
      heaterUnderOperation();
    }
    // printCurrentCommand();
}



