

//------------------Will be implemented once LCD is finalized-----------------------

//#pragma once
//
//#ifndef LCD_H_
//#define LCD_H_
//
//#include "stm32f0xx_hal.h"
//#include "string.h"
//#include "stdio.h"
//#include "main.h"
//
//#define RS_Pin 			LCD_RS_Pin
//#define RS_GPIO_Port 	LCD_RS_GPIO_Port
//#define EN_Pin			LCD_EN_Pin
//#define EN_GPIO_Port 	LCD_EN_GPIO_Port
//#define D4_Pin 			DB4_Pin
//#define D4_GPIO_Port 	DB4_GPIO_Port
//#define D5_Pin 			DB5_Pin
//#define D5_GPIO_Port	DB5_GPIO_Port
//#define D6_Pin 			DB6_Pin
//#define D6_GPIO_Port 	DB6_GPIO_Port
//#define D7_Pin			DB7_Pin
//#define D7_GPIO_Port 	DB7_GPIO_Port
//
//// #define LCD20xN 		// For 20xN LCDs
//#define LCD16xN			// For 16xN LCDs
//
//// For row start addresses
//extern const uint8_t ROW_16[];
//extern const uint8_t ROW_20[];
//
///************************************** Command register **************************************/
//#define CLEAR_DISPLAY 				0x01
//
//#define RETURN_HOME 				0x02
//
//#define ENTRY_MODE_SET 				0x04
//#define OPT_S						0x01		// Shift entire display to right
//#define OPT_INC 					0x02		// Cursor increment
//
//#define DISPLAY_ON_OFF_CONTROL 		0x08
//#define OPT_D						0x04		// Turn on display
//#define OPT_C						0x02		// Turn on cursor
//#define OPT_B 						0x01		// Turn on cursor blink
//
//#define CURSOR_DISPLAY_SHIFT 		0x10		// Move and shift cursor
//#define OPT_SC 						0x08
//#define OPT_RL 						0x04
//
//#define FUNCTION_SET 				0x20
//#define OPT_DL 						0x10		// Set interface data length
//#define OPT_N						0x08		// Set number of display lines
//#define OPT_F 						0x04		// Set alternate font
//#define SETCGRAM_ADDR 				0x040
//#define SET_DDRAM_ADDR 				0x80		// Set DDRAM address
//
//
///************************************** Helper macros **************************************/
//#define DELAY(X) HAL_Delay(X)
//
//
///************************************** LCD defines **************************************/
//#define LCD_NIB 		4
//#define LCD_BYTE 		8
//#define LCD_DATA_REG 	(GPIO_PinState)1
//#define LCD_COMMAND_REG (GPIO_PinState)0
//
//
///************************************** LCD typedefs **************************************/
//#define LCD_PORT_TYPE GPIO_TypeDef*
//#define LCD_PIN_TYPE uint16_t
//
//typedef enum {
//	LCD_4_BIT_MODE,
//	LCD_8_BIT_MODE
//} LCD_MODE_TYPEDEF;
//
//
//typedef struct {
//	LCD_PORT_TYPE * data_port;
//	LCD_PIN_TYPE * data_pin;
//
//	LCD_PORT_TYPE rs_port;
//	LCD_PIN_TYPE rs_pin;
//
//	LCD_PORT_TYPE en_port;
//	LCD_PIN_TYPE en_pin;
//
//	LCD_MODE_TYPEDEF mode;
//
//} Lcd_HandleTypeDef;
//
//
///************************************** Public functions **************************************/
//class LCD{
//private:
//	static void writeData(Lcd_HandleTypeDef * lcd, uint8_t data);
//	static void writeCommand(Lcd_HandleTypeDef * lcd, uint8_t command);
//	static void write(Lcd_HandleTypeDef * lcd, uint8_t data, uint8_t len);
//public:
//	Lcd_HandleTypeDef lcd;
//	void sequence();
//	void sequence(bool motion);
//	void init(Lcd_HandleTypeDef * lcd);
//	void putInt( int number);
//	void putString( char const* string);
//	void setCursor( uint8_t row, uint8_t col);
//	Lcd_HandleTypeDef create(
//		LCD_PORT_TYPE port[], LCD_PIN_TYPE pin[],
//		LCD_PORT_TYPE rs_port, LCD_PIN_TYPE rs_pin,
//		LCD_PORT_TYPE en_port, LCD_PIN_TYPE en_pin, LCD_MODE_TYPEDEF mode);
//	void defineChar( uint8_t code, uint8_t const bitmap[]);
//	void clear();
//	void start();
//	void blinkText(char *msgtxt, int col, int row);
//	void createMenu(const char** menu);
//	void arrowDown();
//	void arrowUp();
//	void displayArrow();
//	void fireIcon(uint8_t r, uint8_t column, uint8_t code);
//};
//
//
//#endif /* LCD_H_ */
