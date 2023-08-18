/*
 * lcd.c
 *
 *  Created on: 10/06/2018
 *      Author: Olivier Van den Eede
 */

#include "lcd.h"
const uint8_t ROW_16[] = {0x00, 0x40, 0x10, 0x50};
const uint8_t ROW_20[] = {0x00, 0x40, 0x14, 0x54};

Lcd_HandleTypeDef lcd;


/************************************** Function definitions **************************************/

/**
 * Create new Lcd_HandleTypeDef and initialize the Lcd
 */
Lcd_HandleTypeDef LCD::lcdCreate(
		LCD_PORT_TYPE port[], LCD_PIN_TYPE pin[],
		LCD_PORT_TYPE rs_port, LCD_PIN_TYPE rs_pin,
		LCD_PORT_TYPE en_port, LCD_PIN_TYPE en_pin, LCD_MODE_TYPEDEF mode)
{
	Lcd_HandleTypeDef lcd;

	lcd.mode = mode;

	lcd.en_pin = en_pin;
	lcd.en_port = en_port;

	lcd.rs_pin = rs_pin;
	lcd.rs_port = rs_port;

	lcd.data_pin = pin;
	lcd.data_port = port;

	lcdInit(&lcd);

	return lcd;
}

/**
 * Initialize 16x2-lcd without cursor
 */
void LCD::lcdInit(Lcd_HandleTypeDef * lcd)
{
	if(lcd->mode == LCD_4_BIT_MODE)
	{
		lcdWriteCommand(lcd, 0x33);
		lcdWriteCommand(lcd, 0x32);
		lcdWriteCommand(lcd, FUNCTION_SET | OPT_N);				// 4-bit mode
	}
	else
		lcdWriteCommand(lcd, FUNCTION_SET | OPT_DL | OPT_N);


	lcdWriteCommand(lcd, CLEAR_DISPLAY);						// Clear screen
	lcdWriteCommand(lcd, DISPLAY_ON_OFF_CONTROL | OPT_D);		// Lcd-on, cursor-off, no-blink
	lcdWriteCommand(lcd, ENTRY_MODE_SET | OPT_INC);			// Increment cursor
}

/**
 * Write a number on the current position
 */
void LCD::lcdInt(Lcd_HandleTypeDef * lcd, int number)
{
	char buffer[11];
	sprintf(buffer, "%d", number);

	lcdString(lcd, buffer);
}

/**
 * Write a string on the current position
 */
void LCD::lcdString(Lcd_HandleTypeDef * lcd, char * string)
{
	for(uint8_t i = 0; i < strlen(string); i++)
	{
		lcdWriteData(lcd, string[i]);
	}
}

/**
 * Set the cursor position
 */
void LCD::lcdCursor(Lcd_HandleTypeDef * lcd, uint8_t row, uint8_t col)
{
	#ifdef LCD20xN
	lcdWriteCommand(lcd, SET_DDRAM_ADDR + ROW_20[row] + col);
	#endif

	#ifdef LCD16xN
	lcdWriteCommand(lcd, SET_DDRAM_ADDR + ROW_16[row] + col);
	#endif
}

/**
 * Clear the screen
 */
void LCD::lcdClear(Lcd_HandleTypeDef * lcd) {
	lcdWriteCommand(lcd, CLEAR_DISPLAY);
}

void LCD::lcdDefineChar(Lcd_HandleTypeDef * lcd, uint8_t code, uint8_t bitmap[]){
	lcdWriteCommand(lcd, SETCGRAM_ADDR + (code << 3));
	for(uint8_t i=0;i<8;++i){
		lcdWriteData(lcd, bitmap[i]);
	}

}

void LCD::lcdSequence()
{
	LCD_PORT_TYPE ports[] = {D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port};
	LCD_PIN_TYPE pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
	lcd = lcdCreate(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
	this->lcdString(&lcd,"qwertyuiopasdfghABCD");
	this->lcdCursor(&lcd,1,0);
	this->lcdString(&lcd,"QWERTYUIOPASDFGH");
	this->lcdCursor(&lcd,2,0);
	this->lcdString(&lcd,"qwertyuiopasdfgh");
	this->lcdCursor(&lcd,3,0);
	this->lcdString(&lcd,"QWERTYUIOPASDFGH");
}

void LCD::lcdSequence(bool motionDetect)
{
	LCD_PORT_TYPE ports[] = {D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port};
	LCD_PIN_TYPE pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
	lcd = lcdCreate(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
	if(motionDetect)
	{
		this->lcdString(&lcd,"Motion Welcome");
		this->lcdCursor(&lcd,1,0);
	}
}

/************************************** Private function definition **************************************/

/**
 * Write a byte to the command register
 */
void LCD::lcdWriteCommand(Lcd_HandleTypeDef * lcd, uint8_t command)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, LCD_COMMAND_REG);		// Write to command register

	if(lcd->mode == LCD_4_BIT_MODE)
	{
		lcdWrite(lcd, (command >> 4), LCD_NIB);
		lcdWrite(lcd, command & 0x0F, LCD_NIB);
	}
	else
	{
		lcdWrite(lcd, command, LCD_BYTE);
	}

}

/**
 * Write a byte to the data register
 */
void LCD::lcdWriteData(Lcd_HandleTypeDef * lcd, uint8_t data)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, LCD_DATA_REG);			// Write to data register

	if(lcd->mode == LCD_4_BIT_MODE)
	{
		lcdWrite(lcd, data >> 4, LCD_NIB);
		lcdWrite(lcd, data & 0x0F, LCD_NIB);
	}
	else
	{
		lcdWrite(lcd, data, LCD_BYTE);
	}

}

/**
 * Set len bits on the bus and toggle the enable line
 */
void LCD::lcdWrite(Lcd_HandleTypeDef * lcd, uint8_t data, uint8_t len)
{
	for(uint8_t i = 0; i < len; i++)
	{
		HAL_GPIO_WritePin(lcd->data_port[i], lcd->data_pin[i], (GPIO_PinState)((data >> i) & 0x01));
	}

	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin,(GPIO_PinState) 1);
	DELAY(1);
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin,(GPIO_PinState) 0); 		// Data receive on falling edge
}
