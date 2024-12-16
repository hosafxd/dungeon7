// MONSTER.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MONSTER.h"

Monster *loadMonsters(int *numMonsters) {
    // Read monsters from monsters.txt and return an array of monsters
    FILE *file = fopen("data/monsters.txt", "r");
    if (!file) {
        perror("Failed to open monsters data file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d\n", numMonsters);
    Monster *monsters = (Monster *)malloc(sizeof(Monster) * (*numMonsters));

    for (int i = 0; i < *numMonsters; i++) {
        char name[50];
        int health, strength, baseAttack;

        fgets(name, sizeof(name), file);
        name[strcspn(name, "\n")] = '\0'; // Remove newline
        fscanf(file, "%d %d %d\n", &health, &strength, &baseAttack);

        monsters[i] = *createMonster(name, health, strength, baseAttack);
        monsters[i].inventory = NULL;
    }

    fclose(file);
    return monsters;
}

Monster *createMonster(const char *name, int health, int strength, int baseAttack) {
    Monster *monster = (Monster *)malloc(sizeof(Monster));
    monster->name = strdup(name);
    monster->health = health;
    monster->strength = strength;
    monster->baseAttack = baseAttack;
    monster->inventory = NULL;
    monster->maxHealth = health; // Set initial maxHealth
    return monster;
}
void freeMonster(Monster *monster) {
    if (monster) {
        free(monster->name);
        freeItems(monster->inventory);
        free(monster);
    }
}
