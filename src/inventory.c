#include <string.h>
#include "../include/defines.h"

void initializeInventory(Inventory *inventory) {
  inventory->numItems = 0;
}

void addItem(Inventory *inventory, int itemID, const char *name, int quantity, int price) {
  if (inventory->numItems < MAX_ITEMS) {
    Item newItem = {itemID, "", quantity, price};
    strncpy(newItem.name, name, sizeof(newItem.name) - 1);
    inventory->items[inventory->numItems++] = newItem;
    // printf("Item added successfully.\n");
  } else {
    // printf("Inventory is full. Cannot add more items.\n");
  }
}

void displayInventory(Inventory *inventory) {
  // printf("Inventory:\n");
  // printf("%-10s | %-20s | %-8s | %-10s\n", "Item ID", "Name", "Quantity", "Price");
  // printf("-----------------------------------------------\n");
  for (int i = 0; i < inventory->numItems; i++) {
    // printf("%-10d | %-20s | %-8d | %-10.2f\n", inventory->items[i].itemID, inventory->items[i].name, inventory->items[i].quantity, inventory->items[i].price);
  }
}

int findItemIndex(Inventory *inventory, int itemID) {
  for (int i = 0; i < inventory->numItems; i++) {
    if (inventory->items[i].itemID == itemID) {
      return i;
  	}
  }
  return -1;
}

void removeItem(Inventory *inventory, int itemID) {
  int index = findItemIndex(inventory, itemID);
  if (index != -1) {
    for (int i = index; i < inventory->numItems - 1; ++i) {
      inventory->items[i] = inventory->items[i + 1];
    }
    inventory->numItems--;
    // printf("Item removed successfully.\n");
  } else {
    // printf("Item not found in inventory.\n");
  }
}

void updateItemQuantity(Inventory *inventory, int itemID, int new_quantity) {
  int index = findItemIndex(inventory, itemID);
  if (index != -1) {
    inventory->items[index].quantity = new_quantity;
    // printf("Quantity updated successfully.\n");
  } else {
    // printf("Item not found in inventory.\n");
  }
}
