# README for Text Adventure Game

## Requirements
- C++ Compiler (GCC or Clang)
- Make (Optional, for easier compilation)
- Filesystem library 
- `nlohmann/json` for JSON parsing

## Setup

1. **Compiling the Game**: 
   - If you have a `Makefile`, run `make` in the terminal within the project directory.
   - Without a `Makefile`, compile manually using `g++ -std=c++17 -o main main.cpp Player.cpp Utilities.cpp -lstdc++fs`.

2. **JSON Map Files**: The game reads JSON files to create the game world. Ensure you have at least one JSON file in the same directory as the executable. The JSON file should follow the structure shared earlier.

## How to Play
1. **Starting the Game**: Run the compiled executable, usually `./main` in the terminal.

2. **Choosing a Map**: At the start, the game will list available map files. Enter the number corresponding to the map you wish to play.

3. **Game Commands**:
   - `move <direction>`: Moves the player to a different room. Example: `move east`.
   - `grab <item>` or `take <item>`: Picks up an item in the room. Example: `grab key`.
   - `kill <enemy>`: Attempts to kill an enemy in the room. Example: `kill zombie`.
   - `look` or `look <item>`: Describes the current room or a specific item. Example: `look` or `look gun`.
   - `exit`: Quits the game.

4. **Playing the Game**: The game revolves around exploring rooms, collecting items, and defeating enemies. Pay attention to the descriptions and use your commands to interact with the game world.

5. **Winning the Game**: To win, you need to complete the objective specified in the JSON file. This could be defeating a certain enemy, collecting specific items, or reaching a particular room.

## Troubleshooting
- **Compilation Errors**: Ensure you're using C++17 or later due to the use of the filesystem library.
- **JSON Parsing Errors**: Check the format of your JSON files if the game fails to load the map.
