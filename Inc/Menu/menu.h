/*
 * menu.h
 *
 *  Created on: 14 kwi 2022
 *      Author: bartosz
 */

#ifndef INC_MENU_H_
#define INC_MEN

#include <stdint.h>

//USER CONFIGURATION

//number of posiitons
#define MENU_ITEMS_NUMBER 20

//value size
#define MENU_ITEM_VALUE_SIZE 4

//menu items absolute numbers- MENU_ITEM_x y  - x- name, y number
#define CLOCK 0
#define BAROMETER 1
#define HIGROMETER 2
#define TEMPERATURE 3
#define ALTITUDE 4
#define AZIMUTH 5
#define CHRONO 6
#define SETTINGS 7
#define CLOCK_DATE 8
#define CLOCK_SECONDHAND 9
#define CHRONO_STARTSTOP 10
#define CHRONO_RESET 11
#define SETTINGS_CLOCK 12
#define SETTINGS_CALENDAR 13
#define SETTINGS_BAROMETERREF 14
#define SETTINGS_TEMPERATURECOR 15
#define SETTINGS_CLOCK_HOURMINUTE 16
#define SETTINGS_CLOCK_SECONDHAND 17
#define SETTINGS_CALENDAR_DAY_MONTH 18
#define SETTINGS_CALENDAR_YEAR 19

//END OF USER COFIGURATION

#define NONE 255

#define ENTRY 0
#define LEVEL 1


//each menu position
typedef struct {

	char value[MENU_ITEM_VALUE_SIZE]; //value for display

	uint8_t id[2]; //map position - entry and level numbers

	uint8_t parent; //parent entry number

} MenuItemTypeDef;

typedef struct{
	MenuItemTypeDef items[MENU_ITEMS_NUMBER];
}MenuTypeDef;

//current menu item buffer
MenuItemTypeDef menuCurrentItem;


void menuItemChangeValue(MenuItemTypeDef* item, char* value);
void menuItemInit(MenuTypeDef* menu, uint8_t number, uint8_t entry, uint8_t level, uint8_t parent);


void menuSwitch(void);

#endif /* INC_MENU_H_ */
