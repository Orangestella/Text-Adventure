# Text Adventure: SDwCaCpp coursework 
This project is a **simple game engine** for a text adventure.    
“**Text adventures**”, now called “interactive fiction”, were among the first type of computer game ever produced. These games have no graphics; the player reads the story of the game in text, and decides what their character will do by typing commands at a prompt.  
## Features 
There lists the main features of this engine.

### Command
The only way for players to interact with the game. Only the legal command below can work, otherwise the engine will prompt `I don't understand that.`  
There lists all the legal commands:  
1. `n`/`north` `s`/`south` `w`/`west` `e`/`east`  
    Go to the room in the north/south/west/east direction of the current room.  
    If there doesn't exist a passage to which room, engine will prompt `You can't go that way.` 
2. `get {keyword}`  
    Move object with matched `{keyword}` from the current room list into the inventory.  
    There are **four possible situations** after using this command:
   * If the object exists in the current room, it will prompt `Picked successfully.` after normal pickup
   * If the object already exists in the inventory, it cannot be picked up and prompt `This object is already in the inventory.`
   * If this object does not exist in the current room, it cannot be picked up and prompted `Cannot find that object in this room.`
   * If the object does not exist, it cannot be picked up and prompts `This object doesn't exist.`  
3. `drop {keyword}`  
   Move object with matched `{keyword}` from the inventory into the current room.  
   There are **four possible situations** after using this command:  
    * If the object exists in the inventory, it will prompt `Dropped successfully.` after normal dropping
    * If the object already exists in the room, it cannot be dropped up and prompt `This object is already in the room.`
    * If this object does not exist in the current room, it cannot be dropped up and prompted `Cannot find that object in the inventory.`
    * If the object does not exist, it cannot be dropped up and prompts `This object doesn't exist.`  
4. `inventory`  
   Print out the short names of all the objects in the inventory.
5. `examine {keyword}`  
   Print out the long description of the object with matched keyword. 
6. `eat {keyword}`  
   Print out the player’s strength after adding the energy of the food object with matched keyword to the player’s strength, which should not exceed 100.  
   There are **three possible situations** after using this command:
    * If the food exists in the inventory, it will prompt `You have eaten this food.` after normal eaten
    * If this food does not exist in the inventory, it cannot be eaten up and prompted `Cannot find that thing in the inventory.`
    * If the object isn't a food, it cannot be eaten and prompts `You can't eat that.`
7. `quit`  
    Quit the game.

### Game Content  
This engine stores game content in `strings.h` file, you can modify it to implement your own game.  
Before that, you should know about this **specification**:  
1. Naming rules for referencing string variables related to rooms and objects:  
    * `r{n}name` for room name.
    * `r{n}desc` for room description.
    * `o{n}name` for game object name.
    * `o{n}desc` for game object description.
    * `o{n}key` for game object keyword.  
_Other referencing string variables are not recommended for modification._  
**Please note**: If you have modified any variable names, please make the corresponding modifications in the `initRooms` function, or use the refactoring feature of the IDE
2. Content rules for rooms and objects:  
    * Keyword of object: the keyword of object should be unique. It must comply with this format:  
   `{type}:{name}`  
   which `{type}` must be a single character. `f` is reserved as a keyword for food and **cannot be modified**. You can decide on other types of keywords by yourself. In this project, `t` means tool as sample.  
    * Energy of food: it should be limited in a range of 1-10, otherwise the engine will throw out `std::invalid_argument`  
    * The remaining content can be modified as needed.  

### Gameplay  
Player's strength will be reduced by 1 per minute. Eating food can increase the strength. When strength goes to 0, the game shall be terminated.  

## Environment Requirement  
There are currently no experiments for this issue. It is recommended to use the Windows system to run the program.  

## Todo  
Since most players will not want to play an entire game at one sitting, most games include save and load (or restore) commands.  

## Author 
Edgar Hou (202118020425) CDUT-OBU L5S3
GitHub: https://github.com/Orangestella  
Email: hou.guanyu@student.zy.cdut.edu.cn