# Dungeon7: A Console-Based Adventure Game in C

Dungeon7 is a text-based dungeon crawler game written in the C programming language. Set in a dungeon inspired by the seven deadly sins, players navigate through interconnected rooms, battle monsters symbolizing each sin, and strive for self-improvement through strategic gameplay.

---

## 📜 Story

The game’s story focuses on overcoming personal weaknesses by defeating the **seven deadly sins**:
- **Gluttony**: Test of persistence.
- **Wrath**: Test of strength and agility.
- **Pride, Envy, Lust, Greed, and Sloth**: Each sin presents unique challenges symbolized by powerful monsters.

Each victory not only symbolizes physical triumph but also represents moral growth and self-improvement.

---

## 🛠️ Technical Features

1. **File-Based Storage**:
   - Game data (rooms, items, and monsters) is initialized from external `.txt` files.
   - Save files are stored in `.sav` format.

2. **Dynamic Memory Management**:
   - Memory is dynamically allocated for items, rooms, and monsters.

3. **Robust Struct-Based Design**:
   - `Player`, `Room`, `Item`, and `Monster` structs encapsulate game logic.

4. **Save and Load Mechanic**:
   - Progress can be saved and loaded seamlessly.

---

## 🎮 How to Run Dungeon7

   look          - Examine your surroundings.
                  This will show you the items, monsters, and exits in the current room.

  move          - Move to an adjacent room.
                  After typing 'move', you'll be given a list of available directions.

  inventory     - Check the items you currently carry.
                  Use this to see what you have picked up and potentially drop items.
                  Your inventory carries up to 3 items, pick your items wisely.

  pickup <item> - Pick up an item from the room.
                  Example: 'pickup sword' will add the sword to your inventory if available.

  attack        - Engage a monster in combat if one is present.
                  Tip: Use this after 'look' to decide if you want to fight.

  list     - Lists the saved games.

  save     - Saves the current state of the games.
             You can later load and continue your adventure.

  load     - Loads a saved game file.
             Example: 'load save1.sav'

  quit          - Exit the game.



### Requirements:
- A C compiler (e.g., GCC)
- Make utility

### Steps to Build and Run the Game:
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/hosafxd/Dungeon7.git
   cd Dungeon7
   
2. **Build the Game: Use the provided Makefile to compile the game**:
   ```bash
   make

3. ** Run the game**:
   ```bash
   make run
   
4.**Clean Up: Remove the executable and object files**:
   ```bash
       make clean

