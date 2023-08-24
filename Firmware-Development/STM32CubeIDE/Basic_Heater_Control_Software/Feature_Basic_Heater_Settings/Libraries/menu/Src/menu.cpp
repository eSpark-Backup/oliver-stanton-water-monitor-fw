#include "menu.h"
#include "string.h"
#include "lcd.h"
#include "string.h"

extern LCD lcdObj;
static int Arrow=0;
struct level normalM, heaterM, heaterSet, frostM, frostSet,frostPumpM,frostPumpSet, *currentM;

const char* heaterN[]={"On","Off",NULL};
const char* frostN[]={"On","Off",NULL};
const char* frostPumpN[]={"On","Off",NULL};

void Menu::createMenu(const char** menu)
{
	lcdObj.clear();
	this->numberOfItems=0;
	int w=0;
	while(menu[this->numberOfItems] != NULL)
	{
	        this->numberOfItems++;
	}
	while(w<this->numberOfItems)
	{
		lcdObj.cursor(w,1);
		lcdObj.string(menu[w]);
		w++;
	}
	displayArrow();
}

void Menu::arrowDown()
{
	if(Arrow==(this->numberOfItems)-1)
			Arrow=0;
		else
			Arrow++;
	displayArrow();
}

void Menu::arrowUp()
{
	if(Arrow==0)
		Arrow=(this->numberOfItems)-1;
	else
		Arrow--;
	displayArrow();
}

void Menu::displayArrow()
{
	for(int i=0;i<this->numberOfItems;i++)
	{
		lcdObj.cursor(i,0);
		lcdObj.string(" ");
	}
	lcdObj.cursor(Arrow,0);
	lcdObj.string(">");
}

void Menu::next()
{
	switch(Arrow){
	case 0:
		createMenu(heaterN);
		break;
	case 1:
		createMenu(frostN);
		break;
	case 2:
		createMenu(frostPumpN);
		break;
	default:
		lcdObj.clear();
		break;
	}
}


