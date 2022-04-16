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
#define ENTRIES_MAIN 8


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
	uint8_t value2;//additional info

	uint8_t entry;
	uint8_t level;
	uint8_t parent; //parent entry number- NONE if no parent

	uint8_t childFirst; //first child entry number
	uint8_t childLast; //last child entry number

	uint8_t defined;

} MenuItemTypeDef;

//whole menu
typedef struct{

	MenuItemTypeDef items[ENTRIES_MAX+1][LEVELS];
	MenuItemTypeDef current;

}MenuTypeDef;


void menuInit(MenuTypeDef* menu);
void menuItemChangeValue(MenuTypeDef *menu, uint8_t entry, uint8_t level, char *value, uint8_t value2);
void menuItemInit(MenuTypeDef* menu, uint8_t entry, uint8_t level, uint8_t parent, uint8_t childFirst, uint8_t childLast);
void menuClear(MenuTypeDef *menu);

uint8_t menuSwitch(MenuTypeDef *menu, uint8_t direction);

#endif /* INC_MENU_H_ */
