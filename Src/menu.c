/*
 * menu.c
 * MULTILEVEL MENU HANDLING LIBRARY
 *
 *
 * 	Created on: 15 apr 2022
 *      Author: Bartosz
 *      varsion: 1.1
 *
 *      changelog:
 *      1.1 - changed directive prefix as MENU
 *      1.2 - automatically updating current item
 *
 */

#include <menu.h>

/**
 * @brief  Initialize menu
 *
 * @note   Sets current as positon 0, 0
 *
 * @param  Menu item handler
 *
 * @retval None
 */
void menuInit(MenuTypeDef *menu) {

	menu->current = menu->items[0][0];
}

/**
 * @brief  Change values in menu entry
 *
 * @note   None
 *
 * @param  Menu item handler
 *
 * @param entry number
 *
 * @param level number
 *
 * @param string value
 *
 * @param int value
 *
 * @retval None
 */
void menuItemChangeValue(MenuTypeDef *menu, uint8_t entry, uint8_t level,
		char *value, uint8_t value2) {

	for (uint8_t i = 0; i <= MENU_ITEM_VALUE_SIZE; i++) {

		menu->items[entry][level].value[i] = value[i];
	}

	menu->items[entry][level].value2 = value2;

	//automatically refresh current item
	if (menu->current.entry == entry && menu->current.level == level) {
		memcpy(menu->current.value,
				menu->items[menu->current.entry][menu->current.level].value,
				sizeof(menu->current.value));
		menu->current.value2 =
				menu->items[menu->current.entry][menu->current.level].value2;
	}

}

/**
 * @brief  Initialize menu item
 *
 * @note   Sets structure of each postion
 *
 * @param  Menu item handler
 *
 * @param item entry
 *
 * @param item level
 *
 * @param item parent entry- MENU_NONE if no parent
 *
 * @param item first child entry number, MENU_NONE if no child
 *
 * @param item last child entry number, ENU_NONE if no child
 *
 * @retval None
 */
void menuItemInit(MenuTypeDef *menu, uint8_t entry, uint8_t level,
		uint8_t parent, uint8_t childFirst, uint8_t childLast) {
	menu->items[entry][level].entry = entry;
	menu->items[entry][level].level = level;
	menu->items[entry][level].parent = parent;
	menu->items[entry][level].childFirst = childFirst;
	menu->items[entry][level].childLast = childLast;

}

/**
 * @brief  Switch current element of menu
 *
 * @note   current element stores actual selected item
 *
 * @param  Menu item handler
 *
 * @param switch direction
 *
 * @retval OK if switched, ERROR if not switched(no parent, no child, no above, or no under)
 */
uint8_t menuSwitch(MenuTypeDef *menu, uint8_t direction) {

	if (direction == MENU_UP) { //nie działa

		if ((menu->current.level == 0 && menu->current.entry > 0)
				|| (menu->current.level != 0
						&& menu->current.entry
								> menu->items[menu->current.parent][menu->current.level
										- 1].childFirst)) {

			menu->current.entry--;
			memcpy(menu->current.value,
					menu->items[menu->current.entry][menu->current.level].value,
					sizeof(menu->current.value));
			menu->current.value2 =
					menu->items[menu->current.entry][menu->current.level].value2;

			menu->current.parent =
					menu->items[menu->current.entry][menu->current.level].parent;
			menu->current.childFirst =
					menu->items[menu->current.entry][menu->current.level].childFirst;
			menu->current.childLast =
					menu->items[menu->current.entry][menu->current.level].childLast;
			return MENU_OK;
		} else {
			return MENU_ERROR;
		}

	} else if (direction == MENU_DOWN) {

		if ((menu->current.level == 0
				&& menu->current.entry < MENU_ENTRIES_MAIN - 1)
				|| (menu->current.level != 0
						&& menu->current.entry
								< menu->items[menu->current.parent][menu->current.level
										- 1].childLast)) {

			menu->current.entry++;

			memcpy(menu->current.value,
					menu->items[menu->current.entry][menu->current.level].value,
					sizeof(menu->current.value));
			menu->current.value2 =
					menu->items[menu->current.entry][menu->current.level].value2;

			menu->current.parent =
					menu->items[menu->current.entry][menu->current.level].parent;
			menu->current.childFirst =
					menu->items[menu->current.entry][menu->current.level].childFirst;
			menu->current.childLast =
					menu->items[menu->current.entry][menu->current.level].childLast;
			return MENU_OK;

		} else {
			return MENU_ERROR;
		}

	} else if (direction == MENU_ENTER && menu->current.childFirst != MENU_NONE) {

		menu->current.parent = menu->current.entry;
		menu->current.entry = menu->current.childFirst;
		menu->current.level++;

		menu->current.childFirst =
				menu->items[menu->current.entry][menu->current.level].childFirst;
		menu->current.childLast =
				menu->items[menu->current.entry][menu->current.level].childLast;
		memcpy(menu->current.value,
				menu->items[menu->current.entry][menu->current.level].value,
				sizeof(menu->current.value));
		menu->current.value2 =
				menu->items[menu->current.entry][menu->current.level].value2;
		return MENU_OK;
	} else if (direction == MENU_EXIT && menu->current.parent != MENU_NONE) {

		menu->current.entry = menu->current.parent;
		menu->current.parent--; //automaticalli set to 255(MENU_NONE) if was zero
		menu->current.level--;
		menu->current.childFirst =
				menu->items[menu->current.entry][menu->current.level].childFirst;
		menu->current.childLast =
				menu->items[menu->current.entry][menu->current.level].childLast;

		memcpy(menu->current.value,
				menu->items[menu->current.entry][menu->current.level].value,
				sizeof(menu->current.value));
		menu->current.value2 =
				menu->items[menu->current.entry][menu->current.level].value2;
		return MENU_OK;
	} else {
		return MENU_ERROR;
	}
}
