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

//matrix size
#define MENU_LEVELS 3
#define MENU_ENTRIES_MAX 10

#define MENU_ENTRIES_MAIN 8 //entries in main menu - becouse main have no parent and dont know what is last element.


//END OF USER COFIGURATION


#define MENU_NONE 255


#define MENU_ENTER 0
#define MENU_EXIT 1
#define MENU_UP 2
#define MENU_DOWN 3

#define MENU_ERROR 0
#define MENU_OK 1




//each menu position
typedef struct {

	char value[MENU_ITEM_VALUE_SIZE]; //value for display
	uint8_t value2;//additional info
	uint8_t value3;

	uint8_t entry;
	uint8_t level;
	uint8_t parent; //parent entry number- MENU_NONE if no parent

	uint8_t childFirst; //first child entry number
	uint8_t childLast; //last child entry number

} MenuItemTypeDef;



//whole menu
typedef struct{

	MenuItemTypeDef items[MENU_ENTRIES_MAX][MENU_LEVELS];
	MenuItemTypeDef current;
	uint8_t levels;
	uint8_t maxEntries;
	uint8_t mainEntries;

}MenuTypeDef;

void menuInit(MenuTypeDef *menu, uint8_t levels, uint8_t maxEntries, uint8_t mainEntries);
void menuResetCurrent(MenuTypeDef* menu);
void menuItemChangeValue(MenuTypeDef *menu, uint8_t entry, uint8_t level, char *value, uint8_t value2, uint8_t value3);
void menuItemInit(MenuTypeDef* menu, uint8_t entry, uint8_t level, uint8_t parent, uint8_t childFirst, uint8_t childLast);
//void menuClear(MenuTypeDef *menu);

uint8_t menuSwitch(MenuTypeDef *menu, uint8_t direction);

#endif /* INC_MENU_H_ */
