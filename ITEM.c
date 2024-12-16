#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ITEM.h"

Item *loadItems() {
    FILE *file = fopen("data/items.txt", "r");
    if (!file) {
        perror("Failed to open items data file");
        exit(EXIT_FAILURE);
    }
Item *removeItemByName(Item **itemList, const char *name) {
    Item **current = itemList;
    while (*current) {
        if (strcmp((*current)->name, name) == 0) {
            Item *found = *current;
            *current = (*current)->next;
            found->next = NULL;
            return found;
        }
        current = &((*current)->next);
    }
    return NULL; // Not found
}

    Item *itemList = NULL;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        // Parse one line of the form: NAME HEALTH ATTACK STRENGTH
        char name[50];
        int healthBoost, attackBoost, strengthBoost;

        // If the line doesn't match the expected format, skip it
        if (sscanf(line, "%49s %d %d %d", name, &healthBoost, &attackBoost, &strengthBoost) != 4) {
            continue; // malformed line or empty line
        }

        Item *item = createItem(name, healthBoost, attackBoost, strengthBoost);
        itemList = addItemToList(itemList, item);
    }

    fclose(file);
    return itemList;
}

Item *createItem(const char *name, int healthBoost, int attackBoost, int strengthBoost) {
    Item *item = (Item *)malloc(sizeof(Item));
    item->name = strdup(name);
    // Convert item name to lowercase for consistency
    for (char *p = item->name; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
    item->healthBoost = healthBoost;
    item->attackBoost = attackBoost;
    item->strengthBoost = strengthBoost;
    item->next = NULL;
    return item;
}

Item *removeItemByName(Item **itemList, const char *name) {
    Item **current = itemList;
    while (*current) {
        if (strcmp((*current)->name, name) == 0) {
            Item *found = *current;
            *current = (*current)->next;
            found->next = NULL;
            return found;
        }
        current = &((*current)->next);
    }
    return NULL; // Not found
}

Item *addItemToList(Item *list, Item *item) {
    item->next = list;
    return item;
}

void freeItems(Item *item) {
    while (item) {
        Item *next = item->next;
        free(item->name);
        free(item);
        item = next;
    }
}
