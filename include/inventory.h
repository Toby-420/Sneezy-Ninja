#ifndef INVENTORY_H
#define INVENTORY_H

#include "defines.h"

/*
Initialises the amount of items in the inventory to 0
*/

void initializeInventory(Inventory *inventory);

/*
Adds an item to the inventory.
Takes the inventory, item ID, name, quantity of item, and price of item
*/

void addItem(Inventory *inventory, int itemID, const char *name, int quantity, int price);

/*
Shows all items in the inventory
*/

void displayInventory(Inventory *inventory);

/*
Finds the index in the inventory of the item when given the itemID
*/

int findItemIndex(Inventory *inventory, int itemID);

/*
Removes the given item from the given inventory
*/

void removeItem(Inventory *inventory, int itemID);

/*
Updates the quantity of the item in the given inventory
*/

void updateItemQuantity(Inventory *inventory, int itemID, int new_quantity);


#endif