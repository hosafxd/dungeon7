// MONSTER.h
#ifndef MONSTER_H
#define MONSTER_H

#include "ITEM.h"

typedef struct Monster {
    char *name;
    int health;
    int strength;
    int baseAttack;
    Item *inventory;
    int maxHealth; // Add this line if not present
} Monster;

Monster *loadMonsters(int *numMonsters);
void freeMonster(Monster *monster);
Monster *createMonster(const char *name, int health, int strength, int baseAttack);
#endif // MONSTER_H
