/*
 * menu.h
 *
 *  Created on: 14 kwi 2022
 *      Author: bartosz
 */

#ifndef INC_MENU_H_
#define INC_MEN

#include <stdint.h>
#include <string.h>

//USER CONFIGURATION

//item value size
#define MENU_ITEM_VALUE_SIZE 4

#define LEVELS 3
#define ENTRIES_MAX 8



//LEVEL 0
#define CLOCK_ENTRY 0
#define CLOCK_LEVEL 0

#define BAROMETER_ENTRY 1
#define BAROMETER_LEVEL 0

#define HIGROMETER_ENTRY 2
#define HIGROMETER_LEVEL 0

#define TEMPERATURE_ENTRY 3
#define TEMPERATURE_LEVEL 0

#define ALTITUDE_ENTRY 4
#define ALTITUDE_LEVEL 0

#define AZIMUTH_ENTRY 5
#define AZIMUTH_LEVEL 0

#define CHRONO_ENTRY 6
#define CHRONO_LEVEL 0

#define SETTINGS_ENTRY 7
#define SETTINGS_LEVEL 0


//LEVEL 1
#define CLOCK_DATE_ENTRY 0
#define CLOCK_DATE_LEVEL 1

#define CLOCK_SECONDHAND_ENTRY 1
#define CLOCK_SECONDHAND_LEVEL 1

#define CHRONO_STARTSTOP_ENTRY 2
#define CHRONO_STARTSTOP_LEVEL 1

#define CHRONO_RESET_ENTRY 3
#define CHRONO_RESET_LEVEL 1

#define SETTINGS_CLOCK_ENTRY 4
#define SETTINGS_CLOCK_LEVEL 1

#define SETTINGS_CALENDAR_ENTRY 5
#define SETTINGS_CALENDAR_LEVEL 1

#define SETTINGS_REFERENCE_ENTRY 6
#define SETTINGS_CORRECTION_LEVEL 1

//LEVEL 3
#define SETTINGS_CLOCK_HOURMINUTE_ENTRY 0
#define SETTINGS_CLOCK_HOURMINUTE_LEVEL 2

#define SETTINGS_CLOCK_SECONDS_ENTRY 1
#define SETTINGS_CLOCK_SECONDS_LEVEL 2

#define SETTINGS_CALENDAR_DAYMONTH_ENTRY 2
#define SETTINGS_CALENDAR_DAYMONTH_LEVEL 2

#define SETTINGS_CALENDAR_YEAR_ENTRY 3
#define SETTINGS_CALENDAR_YEAR_LEVEL 2



//END OF USER COFIGURATION


#define NONE 255


#define ENTER 0
#define EXIT 1
#define UP 2
#define DOWN 3

#define ERROR 0
#define OK 1



//each menu position
typedef struct {

	char value[MENU_ITEM_VALUE_SIZE]; //value for display

	uint8_t entry;
	uint8_t level;
	uint8_t parent; //parent entry number- NONE if no parent

	uint8_t childFirst; //first child entry number
	uint8_t childLast; //last child entry number

	uint8_t defined;

} MenuItemTypeDef;

//whole menu
typedef struct{

	MenuItemTypeDef items[ENTRIES_MAX][LEVELS];
	MenuItemTypeDef current;

}MenuTypeDef;


void menuInit(MenuTypeDef* menu);
void menuItemChangeValue(MenuItemTypeDef* item, char* value);
void menuItemInit(MenuTypeDef* menu, uint8_t entry, uint8_t level, uint8_t parent, uint8_t childFirst, uint8_t childLast);


uint8_t menuSwitch(MenuTypeDef *menu, uint8_t direction);

#endif /* INC_MENU_H_ */
