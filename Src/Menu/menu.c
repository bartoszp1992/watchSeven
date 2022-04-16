#include <Menu/menu.h>

void menuInit(MenuTypeDef *menu) {

	menu->current = menu->items[0][0];
}

void menuClear(MenuTypeDef *menu) {
	memset(menu, 0, sizeof(menu));
}

/**
 * @brief  Change stored menu item value
 *
 * @note   None
 *
 * @param  Menu item handler
 *
 * @param value
 *
 * @retval None
 */
void menuItemChangeValue(MenuTypeDef *menu, uint8_t entry, uint8_t level,
		char *value, uint8_t value2) {

	for (uint8_t i = 0; i <= MENU_ITEM_VALUE_SIZE; i++) {

		menu->items[entry][level].value[i] = value[i];
	}

	menu->items[entry][level].value2 = value2;

}

/**
 * @brief  initializes menu item
 *
 * @note   Must be repeated for every menu item
 *
 * @param  Menu handler
 *
 * @param absolute number of item
 *
 * @param entry- position in menu
 *
 * @param level- position in level- 0 for main
 *
 * @param parent- number of parent item in it's own(upper) level- NONE if theres no parent
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
	menu->items[entry][level].defined = 1;

}

uint8_t menuSwitch(MenuTypeDef *menu, uint8_t direction) {

	if (direction == UP) {//nie działa

		if ((menu->current.level == 0 && menu->current.entry > 0)
				|| (menu->current.level != 0
						&& menu->current.entry
								> menu->items[menu->current.parent][menu->current.level
										+ 1].childFirst)) {

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
			return OK;
		} else {
			return ERROR;
		}

	} else if (direction == DOWN) {

		if ((menu->current.level == 0
				&& menu->items[menu->current.entry + 1][menu->current.level].defined
						== 1)
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
			return OK;

		} else {
			return ERROR;
		}

	} else if (direction == ENTER && menu->current.childFirst != NONE) {

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
		return OK;
	} else if (direction == EXIT && menu->current.parent != NONE) {

		menu->current.parent--; //automaticalli set to 255(NONE) if was zero
		menu->current.level--;
		menu->current.entry = 0;
		menu->current.childFirst =
				menu->items[menu->current.entry][menu->current.level].childFirst;
		menu->current.childLast =
				menu->items[menu->current.entry][menu->current.level].childLast;

		memcpy(menu->current.value,
				menu->items[menu->current.entry][menu->current.level].value,
				sizeof(menu->current.value));
		menu->current.value2 =
				menu->items[menu->current.entry][menu->current.level].value2;
		return OK;
	} else {
		return ERROR;
	}
}
