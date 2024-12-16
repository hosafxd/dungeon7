// ITEM.h
#ifndef ITEM_H
#define ITEM_H

typedef struct Item {
    char *name;
    int healthBoost;
    int attackBoost;
    int strengthBoost;
    struct Item *next;
} Item;

// Function prototypes
Item *removeItemByName(Item **itemList, const char *name);
Item *loadItems();
void freeItems(Item *item);
Item *createItem(const char *name, int healthBoost, int attackBoost, int strengthBoost);
Item *addItemToList(Item *list, Item *item);

#endif // ITEM_H
