# Dungeon7: A Console-Based Adventure Game in C

![Dungeon7 Banner](https://placeholder-image-url.com) <!-- Replace with your own image URL or remove this line -->

Dungeon7 is a text-based dungeon crawler game written in the C programming language. Set in a dungeon inspired by the seven deadly sins, players navigate through interconnected rooms, battle monsters symbolizing each sin, and strive for self-improvement through strategic gameplay.

## 🎮 Gameplay Overview

Embark on a journey to conquer the seven deadly sins! Explore a dynamically generated dungeon filled with monsters, items, and surprises. Every decision matters as you navigate hidden passages, collect powerful items, and defeat challenging foes.

Key Features:
- **Dynamic Exploration**: Move through interconnected rooms in a dungeon map.
- **Turn-Based Combat**: Engage in strategic battles against monsters.
- **Item Management**: Equip, use, or drop items to strengthen your character.
- **Save & Load**: Save your progress and resume later with `.sav` files.
- **Replayability**: Each run is a unique experience with evolving challenges.

## 📜 Story

The dungeon represents a journey of self-improvement, where players confront and defeat the seven deadly sins: **Gluttony**, **Wrath**, **Pride**, **Envy**, **Lust**, **Greed**, and **Sloth**. Each sin is embodied by a powerful monster that presents unique challenges. Will you emerge victorious and become your best self?

## 🛠️ Technical Features

- **File-Based Data Storage**: 
  - Rooms, items, and monsters are initialized from `.txt` files for easy customization.
  - Game states are saved to `.sav` files for persistence.
  
- **Dynamic Memory Management**: 
  - Memory is dynamically allocated and freed for rooms, items, and player inventory.
  
- **Struct-Based Design**: 
  - Modular design with structs for **Player**, **Room**, **Monster**, and **Item**.
  
- **Robust Error Handling**: 
  - Handles missing files, invalid commands, and boundary conditions gracefully.

## 📜 Commands

| Command        | Description                                                                 |
|----------------|-----------------------------------------------------------------------------|
| `look`         | Inspect the current room for items, monsters, and doors.                   |
| `move <dir>`   | Move to another room. Direction options: `up`, `down`, `left`, `right`.    |
| `pickup <item>`| Pick up an item in the current room and add it to your inventory.           |
| `inventory`    | View your inventory and manage items (use or drop them).                   |
| `attack`       | Engage in combat with the monster in the current room.                     |
| `save`         | Save your game progress to a new `.sav` file.                              |
| `load <file>`  | Load a saved game from a `.sav` file.                                       |
| `list`         | List all available saved games.                                            |
| `quit`         | Exit the game.                                                             |

## 🖥️ Setup Instructions

### Prerequisites
- A C compiler (e.g., GCC)
- A Windows or Linux terminal
- (Optional) Code::Blocks or another IDE for editing and running the code

### Steps to Run the Game
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/Dungeon7.git
   cd Dungeon7
