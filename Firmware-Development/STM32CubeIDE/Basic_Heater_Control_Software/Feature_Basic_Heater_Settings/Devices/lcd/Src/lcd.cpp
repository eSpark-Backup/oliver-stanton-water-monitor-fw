
#include "lcd.h"
#include "main.h"
#include "mainApp.hpp"
#include "usb_device.h"
#include "stm32f0xx_hal.h"
const uint8_t ROW_16[] = {0x00, 0x40, 0x10, 0x50};
const uint8_t ROW_20[] = {0x00, 0x40, 0x14, 0x54};

LCD_PORT_TYPE ports[] = {D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port};
LCD_PIN_TYPE pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};


/************************************** Function definitions **************************************/

/**
 * Create new Lcd_HandleTypeDef and initialize the Lcd
 */
Lcd_HandleTypeDef LCD::create(
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

	init(&lcd);

	return lcd;
}

/**
 * Initialize 16x2-lcd without cursor
 */
void LCD::init(Lcd_HandleTypeDef * lcd)
{
	if(lcd->mode == LCD_4_BIT_MODE)
	{
		writeCommand(lcd, 0x33);
		writeCommand(lcd, 0x32);
		writeCommand(lcd, FUNCTION_SET | OPT_N);				// 4-bit mode
	}
	else
		writeCommand(lcd, FUNCTION_SET | OPT_DL | OPT_N);


	writeCommand(lcd, CLEAR_DISPLAY);						// Clear screen
	writeCommand(lcd, DISPLAY_ON_OFF_CONTROL | OPT_D);		// Lcd-on, cursor-off, no-blink
	writeCommand(lcd, ENTRY_MODE_SET | OPT_INC);			// Increment cursor
}


/**
 * Write a number on the current position
 */
void LCD::putInt(int number)
{
	char buffer[11];
	sprintf(buffer, "%d", number);
	this->putString(buffer);
}

/**
 * Write a string on the current position
 */
void LCD::putString( char const* String)
{
	for(uint8_t i = 0; i < strlen(String); i++)
	{
		writeData(&this->lcd, String[i]);
	}
}

/**
 * Set the cursor position
 */
void LCD::setCursor( uint8_t row, uint8_t col)
{
	#ifdef LCD20xN
	writeCommand(&this->lcd, SET_DDRAM_ADDR + ROW_20[row] + col);
	#endif

	#ifdef LCD16xN
	writeCommand(&this->lcd, SET_DDRAM_ADDR + ROW_16[row] + col);
	#endif
}

/**
 * Clear the screen
 */
void LCD::clear() {
	writeCommand(&this->lcd, CLEAR_DISPLAY);
}

void LCD::defineChar(Lcd_HandleTypeDef * lcd, uint8_t code, uint8_t bitmap[]){
	writeCommand(lcd, SETCGRAM_ADDR + (code << 3));
	for(uint8_t i=0;i<8;++i){
		writeData(lcd, bitmap[i]);
	}

}

void LCD::start()
{
	this->lcd = create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
	this->setCursor(1,4);
	this->putString("Basic Heat");
	this->setCursor(2,0);
	this->putString("Control Software");
}

/************************************** Private function definition **************************************/

/**
 * Write a byte to the command register
 */
void LCD::writeCommand(Lcd_HandleTypeDef * lcd, uint8_t command)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, LCD_COMMAND_REG);		// Write to command register

	if(lcd->mode == LCD_4_BIT_MODE)
	{
		write(lcd, (command >> 4), LCD_NIB);
		write(lcd, command & 0x0F, LCD_NIB);
	}
	else
	{
		write(lcd, command, LCD_BYTE);
	}

}

/**
 * Write a byte to the data register
 */
void LCD::writeData(Lcd_HandleTypeDef * lcd, uint8_t data)
{
	HAL_GPIO_WritePin(lcd->rs_port, lcd->rs_pin, LCD_DATA_REG);			// Write to data register

	if(lcd->mode == LCD_4_BIT_MODE)
	{
		write(lcd, data >> 4, LCD_NIB);
		write(lcd, data & 0x0F, LCD_NIB);
	}
	else
	{
		write(lcd, data, LCD_BYTE);
	}

}

/**
 * Set len bits on the bus and toggle the enable line
 */
void LCD::write(Lcd_HandleTypeDef * lcd, uint8_t data, uint8_t len)
{
	for(uint8_t i = 0; i < len; i++)
	{
		HAL_GPIO_WritePin(lcd->data_port[i], lcd->data_pin[i], (GPIO_PinState)((data >> i) & 0x01));
	}

	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin,(GPIO_PinState) 1);
	DELAY(1);
	HAL_GPIO_WritePin(lcd->en_port, lcd->en_pin,(GPIO_PinState) 0); 		// Data receive on falling edge
}


