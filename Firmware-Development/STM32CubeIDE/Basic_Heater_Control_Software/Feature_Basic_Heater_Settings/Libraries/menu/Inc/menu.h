#pragma once

#include "string.h"
#include "mainApp.hpp"
#include "_DEBUG_.h"
#include "switches.hpp"
#include <initializer_list>

struct MenuItem;
struct subMenu
{
	const char* label;          		//Name of the menu Element
    void (*setup)(void);                //Function invoked once to init
	bool (*DoWork)(uint32_t, MenuItem*); // Function invoked when element is selected
	const MenuItem* next;       		//SubMenu which it points to
    const MenuItem* prev;       		//Parent Menu
    uint8_t num_childmenus;         	//No. of child menus
//    uint8_t num_parentmenuItems;		//No. of items in parent menu
    uint8_t num_items;					//No. of items in current menu
};

struct MenuItem
{
	void (*oneTimeSetup)(void);
	bool (*loopDoWork)(uint32_t, MenuItem *);
	subMenu Items[];
};


class Menu{
public:
	Menu(){
		DEBUG_RTT(LOG_WARNING, "Menu obj constructor called");
	}
	~Menu(){
		DEBUG_RTT(LOG_WARNING, "Menu obj destructor called");
	}
	void menuSetup();
	void Display_Log(const char* log, ...);
	void changeMenu(MenuItem *menu);
	void displayMenu(const MenuItem *menu, int elements);
	void navigateMenu(MenuItem *currentMenu, bool checkReturn);

public:
	bool jumpToNextItem_ = false;

private:
	uint8_t numChild_;
	uint8_t numItems_;
	uint8_t choice_;	

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

void getButtonStates();
void resetButtonStates();
void setButtonStates(std::initializer_list<SWITCHES> buttons, bool enableSelected);
void defaultMenuSetup(void);


extern Switches ButtonE;		// Enter/OK button obj
extern Switches ButtonB;			//Back button obj
extern Switches ButtonU;			//UP button obj
extern Switches ButtonD;			//Down button obj
extern Switches ButtonR;			//RODI button obj

//-----------declare MenuItem Structs defined in menuList.h down below ----------------
extern struct MenuItem intro, mainMenu, fodMenu, fodOptns, roSaver, fodOptnsOff, roSaverOptns, roSaverThresh,
roSaverThreshAdjust, Battery_Index, BatMonitorMenu, BatLowMenuOptns, BatLowMenuOFF, BatPowerConsumptionOptns,
BatPowerConsumptiontable, BatPowerConsumptionOptnsON, DeviceStandByScreen, DeviceShutDown, BatteryChargingDetected, BatteryChargingDisconnected;
//-------------------------------------------------------------------------------------

extern struct MenuItem intro05, mainMenu05, fodMenu05, fodOptnsOff05, mainsTDSThreshold, mainsTDSTemperature,
mainsTDSShutdown, roTDS, roTDSThreshold, roDynamicTDSThreshold, roFixedTDSThreshold, roTDSTemp, roTDSShutdown,
diTDS, diTDSThreshold, diTDSTemperature, diTDSShutdown;
