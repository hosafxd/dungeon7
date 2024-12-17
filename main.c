// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ROOM.h"
#include "ITEM.h"
#include "MONSTER.h"
#include <time.h>
#include <windows.h>
#include <io.h>

#ifdef _WIN32
#endif




typedef struct Player
{
    char name[50];
    int health;
    int maxHealth;
    int strength;
    int baseAttack;
    Item *inventory;
} Player;


// Function prototypes
char *toLowerCase(char *str);
void gameLoop(Player *player, Room **rooms, int numRooms);
void moveCommand(Player *player, Room **currentRoom);
void lookCommand(Player *player, Room **currentRoom);
void inventoryCommand(Player *player,Room *currentRoom);
void pickupCommand(Player *player, Room *currentRoom, char *itemName);
void attackMonster(Player *player, Monster *monster);
int monstersRemain(Room **rooms, int numRooms);
void attackCommand(Player *player, Room *currentRoom);
void listSaves(const char *directory);
void saveGame(Player *player, Room **rooms, int numRooms, Room *currentRoom);
void restoreConnections(Room **rooms, int numRooms);
void loadGame(Player *player, Room ***rooms, int *numRooms, const char *filename, Room **currentRoom);
int monstersRemain(Room **rooms, int numRooms);





void displayHealthBar(Player player)
{
    int barWidth = 20;

    int filledBars = (player.health * barWidth) / player.maxHealth;

    printf("\n %s: [", player.name);
    for (int i = 0; i < barWidth; i++)
    {
        if (i < filledBars)
        {
            printf("#");
        }
        else
        {
            printf(" ");
        }
    }
    printf("] %d/%d       ", player.health, player.maxHealth);
}





void displayMonsterHealthBar(Monster monster)
{
    int barWidth = 20;
    int filledBars = (monster.health * barWidth) / monster.maxHealth;

    printf("%s: [", monster.name);
    for (int i = 0; i < barWidth; i++)
    {
        if (i < filledBars)
        {
            printf("#");
        }
        else
        {
            printf(" ");
        }
    }
    printf("] %d/%d\n", monster.health,  monster.maxHealth );
}







clock_t start; //global variable

int main()
{
    clock_t start = clock(); // to measure the gampeplay time as a record.
    Player player;
    printf("Enter your name: ");
    fgets(player.name, sizeof(player.name), stdin);
    player.name[strcspn(player.name, "\n")] = '\0'; //it replaces the first \n to \0 (null terminator)
    player.health = 50;
    player.maxHealth = 50;
    player.strength = 10;
    player.baseAttack = 10;
    player.inventory = NULL;//initial stats of the player


    int numRooms;
    Room **rooms = initRooms(&numRooms);
    Item *allItems = loadItems();

    // Find the rooms by name
    Room *humanShelf = findRoomByName(rooms, numRooms, "HUMAN SHELF");
    Room *gluttony = findRoomByName(rooms, numRooms, "GLUTTONY");
    Room *envy = findRoomByName(rooms, numRooms, "ENVY");
    Room *lust = findRoomByName(rooms, numRooms, "LUST");
    Room *wrath = findRoomByName(rooms, numRooms, "WRATH");

    // First, clear all rooms' items, to ensure they start empty:
    for (int i = 0; i < numRooms; i++)
    {
        rooms[i]->items = NULL;
    }//no items on the map yet


//ITEM DISTRIBUTION
    Item *hpot2 = removeItemByName(&allItems, "potion");
    if (hpot2)
    {
        hpot2->next = humanShelf->items;
        humanShelf->items = hpot2;
    }

    Item *bat = removeItemByName(&allItems, "bat");
    if (bat)
    {
        bat->next = humanShelf->items;
        humanShelf->items = bat;
    }

    Item *blade = removeItemByName(&allItems, "blade");
    if (blade)
    {
        blade->next = gluttony->items;
        gluttony->items = blade;
    }

    /*  // ENVY -> longblade
      Item *longblade = removeItemByName(&allItems, "longblade");
      if (longblade)
      {
          longblade->next = envy->items;
          envy->items = longblade;
      }
      */
    Item *shield = removeItemByName(&allItems, "shield");
    if (shield)
    {
        shield->next = lust->items;
        lust->items = shield;
    }
    Item *hpot1 = removeItemByName(&allItems, "potion");
    if (hpot1)
    {
        hpot1->next = lust->items;
        lust->items = hpot1;
    }

    Item *hpot3 = removeItemByName(&allItems, "potion");
    if (hpot3)
    {
        hpot3->next = wrath->items;
        wrath->items = hpot3;
    }

    Item *axe = removeItemByName(&allItems, "axe");
    if (axe)
    {
        axe->next = wrath->items;
        wrath->items = axe;
    }

    //-----------------------------









    // A helper function to find a monster by name in the monsters array.
    int findMonsterByName(Monster *monsters, int numMonsters, const char *monsterName)
    {
        for (int i = 0; i < numMonsters; i++)
        {
            if (strcmp(monsters[i].name, monsterName) == 0)
            {
                return i;
            }
        }
        return -1; //return 0??
    }


    int numMonsters;
    Monster *monsters = loadMonsters(&numMonsters);//from MONSTER.c

    for (int i = 0; i < numRooms; i++)
    {
        if (strcmp(rooms[i]->name, "HUMAN SHELF") == 0)
        {
            continue; // Skip HUMAN SHELF
        }

        // önce odanın ismine göre bir monnster name string belirliyoruz
        char monsterName[100];
        snprintf(monsterName, sizeof(monsterName), "Monster of %s", rooms[i]->name);

        // daha sonra isme göre monsterı buluyoruz
        int cmonster = findMonsterByName(monsters, numMonsters, monsterName);
        if (cmonster != -1)
        {
            // Assign this monster to the room
            rooms[i]->monster = createMonster(monsters[cmonster].name,
                                              monsters[cmonster].health,
                                              monsters[cmonster].strength,
                                              monsters[cmonster].baseAttack);
        }

    }
    // Free the temporary monsters array, so it is pointing null
    free(monsters);







    printf("\n------------------------------------------------------------\n");
    printf("                     GAMEPLAY GUIDANCE                      \n");
    printf("------------------------------------------------------------\n");
    printf("Welcome to the world of this adventure! Here are some commands\n");
    printf("you can use to navigate, interact, and survive in the dungeon:\n\n");

    printf("  look          - Examine your surroundings.\n");
    printf("                  This will show you the items, monsters, and exits in the current room.\n\n");


    printf("  move          - Move to an adjacent room.\n");
    printf("                  After typing 'move', you'll be given a list of available directions.\n\n");



    printf("  inventory     - Check the items you currently carry.\n");
    printf("                  Use this to see what you have picked up and potentially drop items.\n");
    printf("                  Your inventory carries up to 3 items, pick your items wisely.\n\n");

    printf("  pickup <item> - Pick up an item from the room.\n");
    printf("                  Example: 'pickup sword' will add the sword to your inventory if available.\n\n");

    printf("  attack        - Engage a monster in combat if one is present.\n");
    printf("                  Tip: Use this after 'look' to decide if you want to fight.\n\n");

    printf("  list     - Lists the saved games.\n\n");


    printf("  save     - Saves the current state of the games.\n");
    printf("             You can later load and continue your adventure.\n\n");

    printf("  load     - Loads a saved game file.\n");
    printf("             Example: 'load save1.sav' \n\n");



    printf("  quit          - Exit the game.\n\n");

    printf("\nTry starting with 'look' to understand what's around you.\n");
    printf("As you explore, remember, only the strongest survive!\n");
    printf("Good luck, and have fun!\n");
    printf("------------------------------------------------------------\n\n");


    printf("\nWelcome, %s!\n", player.name);
    printf("Your starting stats: Health=%d/%d, Attack=%d, Strength=%d\n\n",
           player.health, player.maxHealth, player.baseAttack, player.strength);




    gameLoop(&player, rooms, numRooms);
    freeRooms(rooms, numRooms); //FREE ALLOCATED MEMORY
    freeItems(player.inventory);//FREE ALLOCATED MEMORY
    return 0;
}

char *toLowerCase(char *str)
{
    char *p = str;
    while (*p)
    {
        *p = tolower(*p);
        p++;
    }
    return str;//for consistency i used to lower
}


void gameLoop(Player *player, Room **rooms, int numRooms)
{
    char input[100];
    Room *currentRoom = rooms[0];
    Room *lastRoom = NULL; // before using this, room name and description printed after every commandFIXED



    //-----------------------

    while (1)
    {


        if (!monstersRemain(rooms, numRooms))//monster remain false ise burası çalışır
        {
            // No monsters left
            clock_t end = clock();
            double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

            // Print Victory ASCII Art
            printf("\n");
            printf("____   ____.__        __                       \n");
            printf("\\   \\ /   /|__| _____/  |_  ___________ ___.__.\n");
            printf(" \\   Y   / |  |/ ___\\   __\\/  _ \\_  __ <   |  |\n");
            printf("  \\     /  |  \\  \\___|  | (  <_> )  | \\/\\___  |\n");
            printf("   \\___/   |__|\\___  >__|  \\____/|__|   / ____|\n");
            printf("                   \\/                   \\/     \n");
            printf("\n");
            printf("Victory!\n");
            printf("You completed the game in %.2f seconds. Great job %s!\n", elapsed_time, player->name);


            fflush(stdout); // message is printed before sleeping
            Sleep(6210);    // Wait 6.21 seconds before exiting

            exit(0);   //terminate program
        }








        //if we are in a new room meaning -> player moved
        if (currentRoom != lastRoom)
        {

            printf("You are in %s.\n", currentRoom->name);
            printf("%s\n\n", currentRoom->description);
            lastRoom = currentRoom;
        }


        printf("\nEnter a command: ");
        if (!fgets(input, sizeof(input), stdin))//userdan input alıyoruz
        {

            continue;
        }


        char *command = toLowerCase(input);//input to lowercase
        // command değerini gösterdiği yere; user "input", lowercase karakterlere dönüştürülerek atandı.
        //user input ve built in commandleri karşılaştırarak functionality kazandırıyoruz.
        if (strncmp(command, "move", 4) == 0)
        {
            moveCommand(player, &currentRoom);

        }
        else if (strncmp(command, "look", 4) == 0)
        {
            lookCommand(player, &currentRoom);
        }
        //--------------------


        else if (strncmp(command, "save", 4) == 0)
        {
            saveGame(player, rooms, numRooms, currentRoom);
        }

        else if (strncmp(command, "load", 4) == 0)
        {
            char *filename = command + 5;
            filename[strcspn(filename, "\n")] = '\0';
            loadGame(player, &rooms, &numRooms, filename, &currentRoom);
        }

        else if (strncmp(command, "list", 4) == 0)
        {
            // listSaves fonksiyonu ile bu adresteki dosyaları listeler
            listSaves("C:\\Users\\furka\\Desktop\\cpp\\Dungeon7\\saveload");
        }

        else if (strncmp(command, "attack", 6) == 0)
        {
            attackCommand(player, currentRoom);
        }

        else if (strncmp(command, "inventory", 9) == 0)
        {
            inventoryCommand(player, currentRoom);
        }
        else if (strncmp(command, "pickup", 6) == 0)
        {
            char *itemName = command + 7; // Extract item name
            itemName[strcspn(itemName, "\n")] = '\0'; // Remove newline
            pickupCommand(player, currentRoom, itemName);
        }
        else if (strncmp(command, "quit", 4) == 0)
        {
            printf("Exiting the game. Goodbye!\n");
            break;
        }
        else
        {
            printf("Invalid command.\n\n");//*command için eşleşen bi fonksiyon isimlendirmemiz yoksa
        }

        if (player->health <= 0)
        {
            printf("You have been defeated.\n\n"); //defeat scenerio
            break;
        }
    }
}


void moveCommand(Player *player, Room **currentRoom)
{
    // Print ASCII map snippet based on currentRoom->name

    if (strcmp((*currentRoom)->name, "HUMAN SHELF") == 0)
    {
        printf("        HUMAN SHELF\n"
               "             |\n"
               "           SLOTH\n");
    }
    else if (strcmp((*currentRoom)->name, "SLOTH") == 0)
    {
        printf("        HUMAN SHELF\n"
               "             |\n"
               "           SLOTH\n"
               "             |\n"
               "         GLUTTONY\n");
    }
    else if (strcmp((*currentRoom)->name, "GLUTTONY") == 0)
    {
        printf("           SLOTH\n"
               "             |\n"
               "         GLUTTONY\n"
               "             |\n"
               "           PRIDE\n");
    }
    else if (strcmp((*currentRoom)->name, "PRIDE") == 0)
    {
        printf("         GLUTTONY\n"
               "             |\n"
               "  ENVY-----PRIDE-----LUST\n"
              );
    }
    else if (strcmp((*currentRoom)->name, "ENVY") == 0)
    {
        printf("     ENVY-----PRIDE\n"
               "       |\n"
               "     WRATH\n");
    }
    else if (strcmp((*currentRoom)->name, "WRATH") == 0)
    {
        printf("  ENVY\n"
               "   |\n"
               " WRATH\n");
    }
    else if (strcmp((*currentRoom)->name, "LUST") == 0)
    {
        printf(" PRIDE-----LUST\n"
               "             |\n"
               "           GREED\n");
    }
    else if (strcmp((*currentRoom)->name, "GREED") == 0)
    {
        printf("  LUST\n"
               "   |\n"
               " GREED\n");
    }

    /*printf("\nAvailable directions:\n");
    if ((*currentRoom)->up) printf("up\n");
    if ((*currentRoom)->down) printf("down\n");
    if ((*currentRoom)->left) printf("left\n");
    if ((*currentRoom)->right) printf("right\n");*/

    char direction[10];
    printf("\nEnter a direction (up/down/left/right): ");
    fgets(direction, sizeof(direction), stdin);
    direction[strcspn(direction, "\n")] = '\0'; // remove newline

    for (char *p = direction; *p; p++) *p = tolower((unsigned char)*p);

    Room *nextRoom = NULL;
    if (strcmp(direction, "up") == 0 && (*currentRoom)->up)
    {
        nextRoom = (*currentRoom)->up;
    }
    else if (strcmp(direction, "down") == 0 && (*currentRoom)->down)
    {
        nextRoom = (*currentRoom)->down;
    }
    else if (strcmp(direction, "left") == 0 && (*currentRoom)->left)
    {
        nextRoom = (*currentRoom)->left;
    }
    else if (strcmp(direction, "right") == 0 && (*currentRoom)->right)
    {
        nextRoom = (*currentRoom)->right;
    }
    else
    {
        printf("Invalid direction.\n");
        return;
    }


    *currentRoom = nextRoom;  // MOVE to chosen ROOM
    printf("\n\n");
}



void lookCommand(Player *player, Room **currentRoom)
{
    printf("You look around the room.\n");

    // List items
    Item *item = (*currentRoom)->items;
    if (item)
    {
        printf("Items in the room:\n");
        while (item)
        {
            printf("- %s (Health %+d, Attack %+d, Strength %+d)\n",
                   item->name,
                   item->healthBoost,
                   item->attackBoost,
                   item->strengthBoost);
            item = item->next;
        }
    }
    else
    {
        printf("There are no items here.\n");
    }



    // Doors (Directions)
    printf("Doors leading to other rooms:\n");
    if ((*currentRoom)->up)
    {
        printf("- Up: %s\n", (*currentRoom)->up->name);
    }
    if ((*currentRoom)->down)
    {
        printf("- Down: %s\n", (*currentRoom)->down->name);
    }
    if ((*currentRoom)->left)
    {
        printf("- Left: %s\n", (*currentRoom)->left->name);
    }
    if ((*currentRoom)->right)
    {
        printf("- Right: %s\n", (*currentRoom)->right->name);
    }

    // Monster
    if ((*currentRoom)->monster && (*currentRoom)->monster->health > 0)
    {
        Monster *m = (*currentRoom)->monster;
        printf("\nA %s is here.\n", m->name);
        printf("Monster Stats: Health=%d, Attack=%d, Strength=%d\n", m->health, m->baseAttack, m->strength);


    }
}



void inventoryCommand(Player *player, Room *currentRoom)
{
    printf("Your inventory:\n");
    Item *item = player->inventory;
    int count = 0;
    while (item)
    {
        printf("%d. %s (Health %+d, Attack %+d, Strength %+d)\n", ++count, item->name,
               item->healthBoost, item->attackBoost, item->strengthBoost);
        item = item->next;
    }
    if (count == 0)
    {
        printf("Your inventory is empty.\n");
        return;
    }

    printf("Enter the number of the item to interact with or 0 to return: ");
    int choice;
    scanf("%d", &choice);
    getchar(); // Consume newline character

    if (choice > 0 && choice <= count)
    {
        // Find the chosen item
        Item **current = &(player->inventory);
        for (int i = 1; i < choice; i++)
        {
            current = &((*current)->next);
        }

        Item *selectedItem = *current;

        // if healthpot or another item
        if (strcmp(selectedItem->name, "potion") == 0)
        {
            // Ask user if they want to use it
            printf("Would you like to use your health potion? (yes/no): ");
            char input[10];
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0'; // Remove newline

            for (char *p = input; *p; p++) *p = tolower((unsigned char)*p);

            if (strcmp(input, "yes") == 0)
            {
                // Use the potion
                int healAmount = 15;
                player->health += healAmount;
                if (player->health > player->maxHealth)
                {
                    player->health = player->maxHealth;
                }
                printf("You used the potion. Your health is now %d/%d.\n", player->health, player->maxHealth);

                // Remove the potion from inventory
                *current = selectedItem->next;
                free(selectedItem->name);
                free(selectedItem);

            }
            else
            {
                printf("You decided not to use the health potion.\n");
            }
        }



        else
        {
            //non-healthpot items
            printf("Do you want to drop this item? (yes/no): ");
            char input[10];
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0'; // remove newline

            for (char *p = input; *p; p++) *p = tolower((unsigned char)*p);

            if (strcmp(input, "yes") == 0)
            {
                // Remove item from inventory
                Item *toDrop = *current;
                *current = (*current)->next;

                // Remove item effects
                player->maxHealth -= toDrop->healthBoost;
                player->baseAttack -= toDrop->attackBoost;
                player->strength -= toDrop->strengthBoost;

                // Adjust current health if necessary
                if (player->health > player->maxHealth)
                {
                    player->health = player->maxHealth;
                }

                printf("You dropped %s.\n", toDrop->name);
                printf("Your stats have been updated: Health=%d/%d, Attack=%d, Strength=%d\n",
                       player->health, player->maxHealth, player->baseAttack, player->strength);

                // Add the dropped item back to the current room
                toDrop->next = currentRoom->items;
                currentRoom->items = toDrop;

                // since it goes back into the room dont free the items
            }
            else
            {
                printf("You decided not to drop the item.\n");
            }
        }

    }
    else if (choice != 0)
    {
        printf("Invalid choice.\n");
    }
}




void pickupCommand(Player *player, Room *currentRoom, char *itemName)
{
    //item odada var mı
    Item **currentItem = &(currentRoom->items);
    while (*currentItem && strcmp((*currentItem)->name, itemName) != 0)
    {
        currentItem = &((*currentItem)->next);
    }

    if (*currentItem == NULL)
    {
        printf("Item not found in the room.\n");
        return;
    }

    // Check inventory capacity
    int count = 0;
    Item *temp = player->inventory;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    if (count >= 3)
    {
        printf("Your inventory is full.\n");
        return;
    }

    // Remove item from room and add to player inventory
    Item *itemToPickup = *currentItem;
    *currentItem = (*currentItem)->next; // Remove from room

    itemToPickup->next = player->inventory; // Add to player inventory
    player->inventory = itemToPickup;



    // Check if the item is a healthpot
    if (strcmp(itemToPickup->name, "potion") == 0)
    {
        // Healthpot: No immediate stat updates, just store it
        printf("You picked up %s. It will be usable from your inventory.\n", itemToPickup->name);
    }
    else//if item is non-healthpotion
    {


        player->maxHealth += itemToPickup->healthBoost;

        player->health += itemToPickup->healthBoost;

        player->baseAttack += itemToPickup->attackBoost;
        player->strength += itemToPickup->strengthBoost;

        //currentHealth cannot exceed maxHealth (though it shouldn't now)
        if (player->health > player->maxHealth)
        {
            player->health = player->maxHealth;
        }

        printf("You picked up %s.\n", itemToPickup->name);
        printf("Your stats have been updated: Health=%d/%d, Attack=%d, Strength=%d\n",
               player->health, player->maxHealth, player->baseAttack, player->strength);//her item pickupladığımızda statları yazdır
    }
}




void attackMonster(Player *player, Monster *monster)
{
    printf("You engage in combat with %s!\n", monster->name);
    fflush(stdout);
    Sleep(421); //pause for a moment
    srand(time(NULL));

    displayHealthBar(*player);
    displayMonsterHealthBar(*monster);

    while (player->health > 0 && monster->health > 0)
    {


        // Player's turn
        int playerDamage = player->baseAttack;
        monster->health -= playerDamage;
        printf("\nYou attack the %s for %d damage. Monster health: %d\n",
               monster->name, playerDamage, monster->health);
        fflush(stdout);
        Sleep(421);

        if (monster->health <= 0)
        {
            printf("\nYou have defeated the %s!\n", monster->name);
            printf("You gain +3 health and strength as bonus!\n");
            player->strength+=3;
            player->health+=3;
            printf("\nHealth=%d/%d, Attack=%d, Strength=%d\n",
                   player->health, player->maxHealth, player->baseAttack, player->strength);
            break;
        }



        //dodge chance of the player while monster's turn
        int dodgeChance = player->strength;
        int dodgeRoll = rand() % 100;

        if (dodgeRoll < dodgeChance)
        {
            printf("You dodged the attack!\n");

            displayHealthBar(*player);
            displayMonsterHealthBar(*monster);

            fflush(stdout);
            Sleep(421);
        }
        else
        {
            int monsterDamage = monster->baseAttack;
            player->health -= monsterDamage;

            printf("The %s attacks you for %d damage. Your health: %d\n",
                   monster->name, monsterDamage, player->health);

            if (player->health < 0)
                player->health = 0;

            displayHealthBar(*player);

            if (monster->health < 0)
            {
                monster->health = 0;


            }
            displayMonsterHealthBar(*monster);


            fflush(stdout);
            Sleep(421);

            if (player->health <= 0)
            {
                printf("You have been defeated by the %s! Better luck next time!\n", monster->name);
                fflush(stdout);
                Sleep(6210);
                break;
            }
        }
        // dodge chance of the monster
        int dodgeChance2 = monster->strength;
        int dodgeRoll2 = rand() % 120;

        if (dodgeRoll2 < dodgeChance2)
        {
            printf("%s dodged your attack!\n",monster->name);
            displayHealthBar(*player);
            displayMonsterHealthBar(*monster);
            fflush(stdout);
            Sleep(421);
        }
    }



}




void attackCommand(Player *player, Room *currentRoom)
{
    if (currentRoom->monster && currentRoom->monster->health > 0)
    {
        // Monster present and alive, engage in combat
        attackMonster(player, currentRoom->monster);
    }
    else
    {
        // No monster here or monster already defeated
        printf("There are no monsters in sight...\n");
    }
}




void saveGame(Player *player, Room **rooms, int numRooms, Room *currentRoom)
{
    char filepath[256];
    int saveIndex = 1;

    do
    {
        snprintf(filepath, sizeof(filepath), "C:\\Users\\furka\\Desktop\\cpp\\Dungeon7\\saveload\\save%d.sav", saveIndex++);
    }
    while (_access(filepath, 0) == 0);    // Check if the file exists

    FILE *f = fopen(filepath, "w");
    if (!f)
    {
        printf("Failed to open file for saving.\n");
        return;
    }

    // Save player stats
    fprintf(f, "PlayerName %s\n", player->name);
    fprintf(f, "PlayerHealth %d %d\n", player->health, player->maxHealth);
    fprintf(f, "PlayerStats %d %d\n", player->strength, player->baseAttack);
    fprintf(f, "PlayerRoom %s\n", currentRoom->name);

    // Save inventory
    Item *it = player->inventory;
    int itemCount = 0;
    while (it)
    {
        itemCount++;
        it = it->next;
    }
    fprintf(f, "PlayerInventoryCount %d\n", itemCount);

    it = player->inventory;
    while (it)
    {
        fprintf(f, "PlayerInventoryItem %s %d %d %d\n",
                it->name, it->healthBoost, it->attackBoost, it->strengthBoost);
        it = it->next;
    }

    // Save rooms and their contents
    fprintf(f, "RoomCount %d\n", numRooms);
    for (int i = 0; i < numRooms; i++)
    {
        Room *room = rooms[i];
        fprintf(f, "Room %s\n", room->name);

        // Save monster data
        if (room->monster && room->monster->health)
        {
            fprintf(f, "Monster %s %d %d %d %d\n",
                    room->monster->name,
                    room->monster->health,
                    room->monster->maxHealth,
                    room->monster->strength,
                    room->monster->baseAttack);
        }

        // Save items in the room
        Item *roomItem = room->items;
        int roomItemCount = 0;
        while (roomItem)
        {
            roomItemCount++;
            roomItem = roomItem->next;
        }
        fprintf(f, "RoomItemCount %d\n", roomItemCount);

        roomItem = room->items;
        while (roomItem)
        {
            fprintf(f, "RoomItem %s %d %d %d\n",
                    roomItem->name, roomItem->healthBoost, roomItem->attackBoost, roomItem->strengthBoost);
            roomItem = roomItem->next;
        }
    }

    fclose(f);
    printf("Game saved to %s\n", filepath);
}




void loadGame(Player *player, Room ***rooms, int *numRooms, const char *filename, Room **currentRoom) {
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "C:\\Users\\furka\\Desktop\\cpp\\Dungeon7\\saveload\\%s", filename);

    FILE *f = fopen(fullPath, "r");
    if (!f) {
        printf("Failed to open file for loading: %s\n", fullPath);
        return;
    }

    // Clear old state if needed
    // If you want to support loading mid-game, you'd free old data here.

    freeItems(player->inventory);
    player->inventory = NULL;

    char line[256];
    char savedPlayerRoomName[50] = {0};

    while (fgets(line, sizeof(line), f)) {
        char key[50];
        sscanf(line, "%s", key);

        if (strcmp(key, "PlayerName") == 0) {
            sscanf(line, "PlayerName %49s", player->name);
        } else if (strcmp(key, "PlayerHealth") == 0) {
            sscanf(line, "PlayerHealth %d %d", &player->health, &player->maxHealth);
        } else if (strcmp(key, "PlayerStats") == 0) {
            sscanf(line, "PlayerStats %d %d", &player->strength, &player->baseAttack);
        } else if (strcmp(key, "PlayerRoom") == 0) {
            sscanf(line, "PlayerRoom %49s", savedPlayerRoomName);
        } else if (strcmp(key, "PlayerInventoryCount") == 0) {
            int icount;
            sscanf(line, "PlayerInventoryCount %d", &icount);
            for (int i = 0; i < icount; i++) {
                fgets(line, sizeof(line), f);
                char iname[50];
                int hb, ab, sb;
                sscanf(line, "PlayerInventoryItem %49s %d %d %d", iname, &hb, &ab, &sb);
                Item *newItem = createItem(iname, hb, ab, sb);
                player->inventory = addItemToList(player->inventory, newItem);
            }
        } else if (strcmp(key, "RoomCount") == 0) {
            sscanf(line, "RoomCount %d", numRooms);
            *rooms = initEmptyRooms(*numRooms);

            for (int i = 0; i < *numRooms; i++) {
                fgets(line, sizeof(line), f);
                char roomName[50];
                sscanf(line, "Room %49s", roomName);
                (*rooms)[i]->name = strdup(roomName);

                fgets(line, sizeof(line), f);
                char mname[50];
                    int health, maxHealth, strength, baseAttack;
                    sscanf(line, "Monster %49s %d %d %d %d", mname, &health, &maxHealth, &strength, &baseAttack);
                    (*rooms)[i]->monster = createMonster(mname, health, strength, baseAttack);
                    (*rooms)[i]->monster->maxHealth = maxHealth;


                fgets(line, sizeof(line), f);
                int itemCount;
                sscanf(line, "RoomItemCount %d", &itemCount);
                for (int j = 0; j < itemCount; j++) {
                    fgets(line, sizeof(line), f);
                    char iname[50];
                    int hb, ab, sb;
                    sscanf(line, "RoomItem %49s %d %d %d", iname, &hb, &ab, &sb);
                    Item *item = createItem(iname, hb, ab, sb);
                    (*rooms)[i]->items = addItemToList((*rooms)[i]->items, item);
                }
            }
        }
    }

    fclose(f);

    // Restore room connections
    restoreConnections(*rooms, *numRooms);

    *currentRoom = findRoomByName(*rooms, *numRooms, savedPlayerRoomName);//savedPlayerRoomName buraya güncel odayı atadık
if (*currentRoom == NULL) {
    printf("Error: Could not find saved room '%s'.\n", savedPlayerRoomName);
    fclose(f);
    return; // Stop loading here
}

    printf("Game loaded successfully from %s.\n", fullPath);
}






void restoreConnections(Room **rooms, int numRooms) {
    //it will be called after loading
    for (int i = 0; i < numRooms; i++) {
        if (strcmp(rooms[i]->name, "HUMAN SHELF") == 0) {
            rooms[i]->down = findRoomByName(rooms, numRooms, "SLOTH");
        } else if (strcmp(rooms[i]->name, "SLOTH") == 0) {
            rooms[i]->up = findRoomByName(rooms, numRooms, "HUMAN SHELF");
            rooms[i]->down = findRoomByName(rooms, numRooms, "GLUTTONY");
        } else if (strcmp(rooms[i]->name, "GLUTTONY") == 0) {
            rooms[i]->up = findRoomByName(rooms, numRooms, "SLOTH");
            rooms[i]->down = findRoomByName(rooms, numRooms, "PRIDE");
        } else if (strcmp(rooms[i]->name, "PRIDE") == 0) {
            rooms[i]->up = findRoomByName(rooms, numRooms, "GLUTTONY");
            rooms[i]->left = findRoomByName(rooms, numRooms, "ENVY");
            rooms[i]->right = findRoomByName(rooms, numRooms, "LUST");
        } else if (strcmp(rooms[i]->name, "ENVY") == 0) {
            rooms[i]->right = findRoomByName(rooms, numRooms, "PRIDE");
            rooms[i]->down = findRoomByName(rooms, numRooms, "WRATH");
        } else if (strcmp(rooms[i]->name, "WRATH") == 0) {
            rooms[i]->up = findRoomByName(rooms, numRooms, "ENVY");
        } else if (strcmp(rooms[i]->name, "LUST") == 0) {
            rooms[i]->left = findRoomByName(rooms, numRooms, "PRIDE");
            rooms[i]->down = findRoomByName(rooms, numRooms, "GREED");
        } else if (strcmp(rooms[i]->name, "GREED") == 0) {
            rooms[i]->up = findRoomByName(rooms, numRooms, "LUST");
        }
    }
}






void listSaves(const char *directory)
{

    char searchPath[MAX_PATH];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.sav", directory);

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(searchPath, &fd);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("No saves found in %s\n", directory);
        return;
    }

    printf("Saved games:\n");
    do
    {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            printf("- %s\n", fd.cFileName);
        }
    }
    while (FindNextFile(hFind, &fd));
    FindClose(hFind);
}




int monstersRemain(Room **rooms, int numRooms)
{
    for (int i = 0; i < numRooms; i++)
    {
        if (rooms[i]->monster && rooms[i]->monster->health > 0)
        {
            return 1; // Monster alive
        }
    }
    return 0; // no monsters
}
