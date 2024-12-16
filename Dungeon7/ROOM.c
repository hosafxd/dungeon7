// ROOM.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ROOM.h"

// Forward declaration of helper functions
static Room *findRoomByNameInternal(Room **rooms, int numRooms, const char *name);

Room **initRooms(int *numRooms) {
    // Open the rooms data file
    FILE *file = fopen("data/rooms.txt", "r");
    if (!file) {
        perror("Failed to open rooms data file");
        exit(EXIT_FAILURE);
    }

    // Read the number of rooms
    fscanf(file, "%d\n", numRooms);
    Room **rooms = (Room **)malloc(sizeof(Room *) * (*numRooms));

    // Read room data
    for (int i = 0; i < *numRooms; i++) {
        rooms[i] = (Room *)malloc(sizeof(Room));

        char name[50];
        fgets(name, sizeof(name), file);
        name[strcspn(name, "\n")] = '\0'; // Remove newline
        rooms[i]->name = strdup(name);

        // Read description until '###' marker
        char description[1024] = "";
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            if (strcmp(line, "###\n") == 0 || strcmp(line, "###") == 0) {
                break;
            }
            strcat(description, line);
        }
        description[strcspn(description, "\n")] = '\0'; // Remove trailing newline
        rooms[i]->description = strdup(description);

        rooms[i]->numDoors = 0;
        rooms[i]->monster = NULL;
        rooms[i]->items = NULL;

        // Initialize direction pointers
        rooms[i]->up = NULL;
        rooms[i]->down = NULL;
        rooms[i]->left = NULL;
        rooms[i]->right = NULL;

        rooms[i]->visited = 0;

        for (int d = 0; d < MAX_DOORS; d++) {
            rooms[i]->doors[d] = NULL;
        }
    }

    fclose(file);

    // After rooms are loaded, set up directional connections
    Room *humanShelf = findRoomByNameInternal(rooms, *numRooms, "HUMAN SHELF");
    Room *sloth = findRoomByNameInternal(rooms, *numRooms, "SLOTH");
    Room *gluttony = findRoomByNameInternal(rooms, *numRooms, "GLUTTONY");
    Room *pride = findRoomByNameInternal(rooms, *numRooms, "PRIDE");
    Room *envy = findRoomByNameInternal(rooms, *numRooms, "ENVY");
    Room *wrath = findRoomByNameInternal(rooms, *numRooms, "WRATH");
    Room *lust = findRoomByNameInternal(rooms, *numRooms, "LUST");
    Room *greed = findRoomByNameInternal(rooms, *numRooms, "GREED");

    // Set directional connections based on your ASCII map:
    // HUMAN SHELF
    humanShelf->up = NULL;
    humanShelf->down = sloth;
    humanShelf->left = NULL;
    humanShelf->right = NULL;

    // SLOTH
    sloth->up = humanShelf;
    sloth->down = gluttony;
    sloth->left = NULL;
    sloth->right = NULL;

    // GLUTTONY
    gluttony->up = sloth;
    gluttony->down = pride;
    gluttony->left = NULL;
    gluttony->right = NULL;

    // PRIDE
    pride->up = gluttony;
    pride->down = NULL;
    pride->left = envy;
    pride->right = lust;

    // ENVY
    envy->up = NULL;
    envy->down = wrath;
    envy->left = NULL;
    envy->right = pride;

    // WRATH
    wrath->up = envy;
    wrath->down = NULL;
    wrath->left = NULL;
    wrath->right = NULL;

    // LUST
    lust->up = NULL;
    lust->down = greed;
    lust->left = pride;
    lust->right = NULL;

    // GREED
    greed->up = lust;
    greed->down = NULL;
    greed->left = NULL;
    greed->right = NULL;

    return rooms;
}

Room **initEmptyRooms(int numRooms) {
    Room **rooms = (Room **)malloc(sizeof(Room*) * numRooms);
    if (!rooms) {
        perror("Failed to allocate memory for empty rooms");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numRooms; i++) {
        rooms[i] = (Room *)malloc(sizeof(Room));
        if (!rooms[i]) {
            perror("Failed to allocate memory for a room");
            exit(EXIT_FAILURE);
        }

        rooms[i]->name = NULL;
        rooms[i]->description = NULL;
        rooms[i]->numDoors = 0;
        rooms[i]->monster = NULL;
        rooms[i]->items = NULL;
        rooms[i]->up = NULL;
        rooms[i]->down = NULL;
        rooms[i]->left = NULL;
        rooms[i]->right = NULL;
        rooms[i]->visited = 0;

        for (int d = 0; d < MAX_DOORS; d++) {
            rooms[i]->doors[d] = NULL;
        }
    }

    return rooms;
}

void freeRooms(Room **rooms, int numRooms) {
    for (int i = 0; i < numRooms; i++) {
        free(rooms[i]->name);
        free(rooms[i]->description);
        freeItems(rooms[i]->items);
        freeMonster(rooms[i]->monster);
        free(rooms[i]);
    }
    free(rooms);
}

Room *findRoomByName(Room **rooms, int numRooms, const char *name) {
    return findRoomByNameInternal(rooms, numRooms, name);
}

static Room *findRoomByNameInternal(Room **rooms, int numRooms, const char *name) {
    for (int i = 0; i < numRooms; i++) {
        if (strcmp(rooms[i]->name, name) == 0) {
            return rooms[i];
        }
    }
    return NULL;
}
