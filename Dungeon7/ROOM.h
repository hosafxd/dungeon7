// ROOM.h
#ifndef ROOM_H
#define ROOM_H

#include "ITEM.h"
#include "MONSTER.h"

#define MAX_DOORS 3

typedef struct Room {
    char *name;
    char *description;
    struct Room *doors[MAX_DOORS]; // You can keep this if you still want reference
    int numDoors;
    struct Monster *monster;
    struct Item *items;

    // Add directional pointers
    struct Room *up;
    struct Room *down;
    struct Room *left;
    struct Room *right;

    int visited;
} Room;

// Function prototypes
Room **initEmptyRooms(int numRooms);

Room **initRooms(int *numRooms);
void freeRooms(Room **rooms, int numRooms);
Room *findRoomByName(Room **rooms, int numRooms, const char *name);

#endif // ROOM_H
