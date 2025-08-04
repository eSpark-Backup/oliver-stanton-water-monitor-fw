
#include "heater_wb.h"

#define UART_HANDLE huart1

wb_info_t i;
wb_errors_t e;
int8_t err = 0;
int8_t test = 0, tim=1, dev = 0, sensor = 0; 
float tval = 1.0f;
char opt;
char text[1024];
unsigned char eeprom_data_wr[2];



wbtool_cmd heaterCmd = IDLE;

int heaterInit()
{
    /* UART INITIALIZATION*/

    /* Open comunication to W-Bus device */	
	// wbus_open(&wbus, dev);
	// if (err) {
	//  	printf("Error opening W-Bus\n");
	// 	return -1;
	// }
}

void sendHeaterCommand(wbtool_cmd heaterCommand)
{
    heaterCmd=heaterCommand;
}


void commandExecute(wbtool_cmd heaterCmd)
{
    switch (heaterCmd)
    {
        case IDLE:
          printf("IDLE\n");
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
                printf("%s\n", text);
              }
            } else {
              printf("wbus_get_wbinfo() failed\n");
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
                  printf("%x ", eeprom_data[a]);
                } else {
                  break;
                   }
              }
            }		        
                break;
        case CMD_ERR_LS:
            /* read error codes  */
            wbus_errorcodes_read(UART_HANDLE, &e);
            {
              int a;
              
              for (a=0; a<e.nErr; a++)
              {
                wbus_errorcode_print(text, &e, a);
                printf("%s\n", text);
              }
            }
            break;
    
        case CMD_ERR_DEL:
            /* Delete error codes */
            err = wbus_errorcodes_clear(UART_HANDLE);
            break;
        case CMD_TEST:
            printf("Testing device %d for %d seconds with value %f\n", test, tim, tval);
            err = wbus_test(UART_HANDLE, test, tim, tval);
            break;
        case CMD_PH:
            err = wbus_turnOn(UART_HANDLE, WBUS_PH, 30);
            break;
        case CMD_SH:
            err = wbus_turnOn(UART_HANDLE, WBUS_SH, 30);
            break;
        case CMD_VENT:
            err = wbus_turnOn(UART_HANDLE, WBUS_VENT, 30);
            break;
        case CMD_STOP:
            err = wbus_turnOff(UART_HANDLE);
            break;
        case CMD_MONITOR:
            {
            int i;
            wb_sensor_t s;
            for (i=0; i<20; i++)
            {
              wbus_sensor_read(UART_HANDLE, &s, i);
              wbus_sensor_print(text, &s);
              printf("sensor[%d] %s\n", i, text);
            }
            }
            break;
        case CMD_MONITOR_SINGLE:
            {
            wb_sensor_t s;
            wbus_sensor_read(UART_HANDLE, &s, sensor);
            wbus_sensor_print(text, &s);
            printf("sensor[%d] %s\n", sensor, text);
            }
            break;
        default:
            break;
            
    }

}


int heaterLoop()
{
    commandExecute(heaterCmd);
}

// int main(int argc, char **argv)
// {

// 	while (!err) {
// 		switch (heaterCmd) {
// 		case CMD_CHECK:
// 		case CMD_PH:
// 			err = wbus_check(wbus, WBUS_PH);
// 			break;
// 		case CMD_SH:
// 			err = wbus_check(wbus, WBUS_SH);
// 			break;
// 		case CMD_VENT:
// 			err = wbus_check(wbus, WBUS_VENT);
// 			break;
// 		default:
// 			err = -1;
// 			break;
// 		}
// 		if (err) {
// 		  printf("stop\n");
// 		  break;
// 		} 
// 		{
// 		  wb_sensor_t s;
// 		  wbus_sensor_read(wbus, &s, 3);
// 		  wbus_sensor_print(text, &s);
// 		  printf("%s\n", text);
// 		  wbus_sensor_read(wbus, &s, 5);
// 		  wbus_sensor_print(text, &s);
// 		  printf("%s\n", text);
// 		  wbus_sensor_read(wbus, &s, 7);
// 		  wbus_sensor_print(text, &s);
// 		  printf("%s\n", text);
// 		}
// 		printf(" running...\n");
// 		machine_sleep(1);
// 	}
	
// 	/* Close W-Bus */
// 	wbus_close(wbus);
	
// 	return err;
// }
