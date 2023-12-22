#pragma once

#include "string.h"
#include "lcd.h"
#include "mainApp.hpp"
#include "CDC_Serial.h"


struct MenuItem {
    const char* label;          //Name of the menu Element
    bool (*DoWork)(void);     //Function invoked when element is selected
    const MenuItem* next;       //SubMenu which it points to
    const MenuItem* prev;       //Parent Menu
//    short num_childmenus;         //No. of child menu
    short num_parentmenuItems;		//No. of items in parent menu
    short num_items;			//No. of items in current menu
};
extern struct MenuItem mainMenu[], fodMenu[], ro_saver[], fodOptns[];


class Menu{
public:
	int numberOfItems;
//	void BuildMenu(int *currentNode);//const struct MenuItem *currentNode);// void (*DoWork)(void) , struct MenuItem *prevNode, struct MenuItem *nextNode,short num_submenus);
	void displayString(char* strng);
	void displayVar(short var);

	void displayMenu(const MenuItem* menu, int elements);
	void navigateMenu(const char* optn, MenuItem* currentMenu);

//	void createMenu(const char** menu);
//	void arrowDown();
//	void arrowUp();
//	void displayArrow();
//	void next();
//	void back();
//	void printFlame();
//	void pumpConnection();
//	void flameIconCallback(TIM_HandleTypeDef* htim);
};
