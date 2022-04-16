#include <Menu/menu.h>



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
void menuItemChangeValue(MenuItemTypeDef* item, char* value){

	for(uint8_t i = 0; i<= MENU_ITEM_VALUE_SIZE; i++){
		item->value[i] = value[i];
	}


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
void menuItemInit(MenuTypeDef* menu, uint8_t number, uint8_t entry, uint8_t level, uint8_t parent){
	menu->items[number].id[ENTRY] = entry;
	menu->items[number].id[LEVEL] = level;
	menu->items[number].parent = parent;
}
