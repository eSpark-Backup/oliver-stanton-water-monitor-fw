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



void CDC_Serial::displayString(const char* strng){
	char buffer[100];
	int len = sprintf(buffer, "%s\r\n", strng);
    HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)buffer, len);
}

void CDC_Serial::displayChoice(short ch){
	char buffer[20];  // Adjust the buffer size accordingly
	int len = sprintf(buffer, "Your Choice: %d\r\n", ch);
    HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)buffer, len);
}

void CDC_Serial::displayVar(short var, char* n){
	char buffer[50];  // Adjust the buffer size accordingly
	int len = sprintf(buffer, "Variable is: %s ", n);
	HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)buffer, len);
	len = sprintf(buffer, " %d\r\n", var);
    HAL_Delay(1);    CDC_Transmit_FS((uint8_t *)buffer, len);
}
