/*
 * CDC_Serial.cpp
 *
 *  Created on: Dec 15, 2023
 *      Author: Farhan AHmed
 */

#include "main.h"
#include "mainApp.hpp"
#include "string.h"
#include "menu.h"
#include "usbd_cdc_if.h"
#include "CDC_Serial.h"

#define USB_MAX_PACKET_SIZE CDC_DATA_FS_MAX_PACKET_SIZE 

void CDC_Serial::itemsToDisplay(int elements){

}

void CDC_Serial::display(const char* strng)
{   
	char buffer[256] = {0};
	int len = sprintf(buffer, "%s\r\n", strng);					//------  %s\r\n  -----

	int offset = 0;
    uint8_t transmit_result;

    while (offset < len) {
        int bytes_to_send = (len - offset > USB_MAX_PACKET_SIZE) ? USB_MAX_PACKET_SIZE : (len - offset);
        
        do {
            transmit_result = CDC_Transmit_FS((uint8_t *)(buffer + offset), bytes_to_send);
        } while (transmit_result == USBD_BUSY);

        offset += bytes_to_send;
    }
}

void CDC_Serial::display(short ch)
{
	char buffer[20];  // Adjust the buffer size accordingly
	int len = sprintf(buffer, "Your Choice: %d\r\n", ch);
    HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)buffer, len);
}

void CDC_Serial::display(int var, char* n)
{
	char buffer[50];  // Adjust the buffer size accordingly
	int len = sprintf(buffer, "%s", n);
	HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)buffer, len);
	len = sprintf(buffer, " %d\r\n", var);
    HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)buffer, len);
}
