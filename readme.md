# Text Adventure: SDwCaCpp coursework   
_Game engine version: v1.3.0  
Document version v1.4 (Fifth modification)_  
---
This project is a **simple game engine** for a text adventure.    
“**Text adventures**”, now called “interactive fiction”, were among the first type of computer game ever produced. These games have no graphics; the player reads the story of the game in text, and decides what their character will do by typing commands at a prompt.  
---
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
    There are **five possible situations** after using this command:
   * If the object exists in the current room, it will prompt `Picked successfully.` after normal pickup
   * If the object already exists in the inventory, it cannot be picked up and prompt `This object is already in the inventory.`
   * If this object does not exist in the current room, it cannot be picked up and prompted `Cannot find that object in this room.`
   * If the object does not exist, it cannot be picked up and prompts `This object doesn't exist.`   
   * If player haven't typed the keyword after the command, it will prompt `What do you want to get?` (All subsequent commands requiring `{keyword}` or `{filename}` will output an error message under this situation. For brevity, this potential output will not be repeated in the documentation for those later commands)  
3. `drop {keyword}`  
   Move object with matched `{keyword}` from the inventory into the current room.  
   There are **five possible situations** after using this command:  
    * If the object exists in the inventory, it will prompt `Dropped successfully.` after normal dropping
    * If the object already exists in the room, it cannot be dropped up and prompt `This object is already in the room.`
    * If this object does not exist in the current room, it cannot be dropped up and prompted `Cannot find that object in the inventory.`
    * If the object does not exist, it cannot be dropped up and prompts `This object doesn't exist.`  
    * See `get {keyword}`  
4. `inventory`  
   Print out the short names of all the objects in the inventory.
5. `examine {keyword}`  
   Print out the long description of the object with matched keyword.
6. `eat {keyword}`  
   Print out the player’s strength after adding the energy of the food object with matched keyword to the player’s strength, which should not exceed 100.  
   There are **five possible situations** after using this command:
    * If the food exists in the inventory, it will prompt `You have eaten this food.` after normal eaten
    * If this food does not exist in the inventory, it cannot be eaten up and prompted `Cannot find that thing in the inventory.`
    * If the object isn't a food, it cannot be eaten and prompts `You can't eat that.`  
    * See `get {keyword}`  
7. `save {filename}`   
   Save the current state of the game into a plain text file named `{filename}` without extension (but write `{filename}` with .txt or .csv is ok if you want). The save file contains the information as follows: 
   * The strength of player. 
   * The current room player in.  
   * The objects in player's inventory. 
   * The objects in each room.   
   There are **two possible situation** after using this command:  
   * If the file saved successfully, it prompts `Saved successfully.`
   * See `get {keyword}`  
8. `load {filename}`  
   Load the saved file named `{filename}` into the game. After successfully loading the file, the state of game will change according to the save file. And the game will announce the current location again.    
   There are **five possible situations** after using this command:  
   * If the file is loaded successfully, it prompts `Loaded successfully.`
   * If the file doesn't exist, it prompts `The save file doesn't exist.`  
   * If the file is not a validated file for the game (without validation string), it prompts `The save file is not a valid file.`  
   * See `get {keyword}`
9. `quit`  
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
    * ~~Keyword of object: the keyword of object should be **unique**. **It must comply with this format**:  
   `{type}:{name}`  
   which `{type}` must be a single character. `f` is reserved as a keyword for food and **cannot be modified**. You can decide on other types of keywords by yourself. In this project, `t` means tool as sample.~~  
    * No more keyword format limitation!
    * Energy of food: it should be limited in a range of 1-10, otherwise the energy of the object will be set to 1.
    * The remaining content can be modified as needed.  
    * All the room and object has its own automatically generated and increment id.  
3. The format of save file:  
   The save file can be without extension, but .txt or .csv is also validated. The example of format requirement of the file is as follows: 
```text
EDGAR VER1.3.0
100 4
0 3 
1 
2 1 
3 2 4 
4 5 
5 6 

```   
*    The first line `EDGAR VER1.3.0` is **validation line**, which is used to ensure the file if it's a validated file.  
*    The second line `100 4` stores the strength and current room id the player in.  
*    Other lines store the objects in each room. The first number of each line is the id of that room, and 0 is reserved for the inventory. Other numbers are the ids of objects.  
### Gameplay  
Player's strength will be reduced by 1 per minute. Eating food can increase the strength. When strength goes to 0, the game shall be terminated.  

## 2026-10-10 本地排查与修复

检查的原始版本：`39f4ea331272ac19b095068515782928d0a199bb`。

### 已复现：游戏自己生成的存档也会丢物品

在原版中输入以下命令（`slot` 是存档文件名）：

```text
get golden_apple
w
get melon_knife
drop golden_apple
save slot
load slot
inventory
```

读档前背包中有 Melon Knife，读档后背包变空。存档的背包记录从 `0 6` 变成了 `0`。
另一条复现路径是 `n → get honey_cake → s → w → drop honey_cake → save slot → load slot`：
此时房间 1 中的 Golden apple 消失。

原因在 `State::loadState` 的两层循环：

1. `while (ifs.good())` 判断的是**上一次**读取后的流状态。文件以换行结束时，还会再进入一次循环；随后 `getline` 读取失败，却继续处理空行。
2. `while (ss.good())` 同样在确认数字读取成功前就进入循环，`ss >> id` 失败后仍将 `id` 加入数组。对于空行，`id` 保留上一次的值。
3. 最后一条房间记录中的最后一个物品 ID，因此被误当成新的“位置 ID”。金苹果 ID 为 0，而位置 0 代表背包，于是代码执行 `inventory.clear()`；蜂蜜蛋糕 ID 为 1，则执行房间 1 的 `roomObjects.clear()`。
4. 默认最后一个房间放的是 ID 为 6 的刀，而房间只编号 1–5，所以默认布局常常掩盖了这个错误。移动物品后它才显现。

### 已复现：删除行尾空格后，每行最后一件物品丢失

原代码使用 `i < idArray.size() - 1`，无条件忽略最后一个数字。
这恰好抵消了“行尾有空格时，多追加一次旧 ID”的错误；当行尾没有空格时，最后一次成功读取直接设置 EOF，没有多余数字，真正的最后一件物品就被忽略。
所以两个错误曾经互相掩盖，存档格式对不显眼的行尾空格产生了依赖。

### 修复内容

- 按 `getline` 的成功结果逐行读取，只处理成功解析的数字；忽略空行，读取全部物品，不再跳过最后一个。
- 在临时结构中完整解析并验证存档，再替换背包、所有房间物品、位置和体力。错误版本、缺少记录、重复位置/物品、未知 ID、非数字、负数、溢出 ID 和不合法体力都会被拒绝，当前游戏状态保持不变。
- 保持 `EDGAR VER1.3.0` 格式和旧版有效存档兼容；容许无行尾空格、无末尾换行、空行、制表符、CRLF/LF，以及位置记录顺序变化。所有房间和背包仍需各有一条记录。已经吃掉的食物可以不出现在任何记录中。
- 保存失败给出提示，关闭文件后检查写入错误；只在读档成功时重新描述位置。
- 体力改用原子操作，避免后台计时与主线程吃东西、存档、读档之间的数据竞争。
- 去掉 `afxres.h` 的 MFC 依赖；Windows 控制台使用 `windows.h`，输出重定向时宽度默认 80。命令输入遇到 EOF 时退出，命令分隔位置使用 `size_t`，避免长命令截断。

### 本机验证与运行

使用 WinLibs GCC 16.2.0 / MinGW-w64，在 Windows 本机实际编译并运行。
原版仅通过额外的 `afxres.h` 转接头引入 `windows.h` 以便构建，其游戏和读档逻辑未修改。
修复版通过 `-Wall -Wextra -Wpedantic -Werror` 编译，存档测试共 10 个测试方法、36 个场景全部通过。
另有 100 轮并发体力更新测试。测试使用真实游戏进程和保存文件，不只模拟解析函数。

本地 PowerShell 构建与启动：

```powershell
.\build.ps1
.\build\text-adventure.exe
```

`build.ps1` 自动找到此次放在上级 `.tools/mingw64` 的便携式编译器，也可用 `-Compiler '编译器完整路径'` 指定 GCC。
回归测试使用 Python 标准库：

```powershell
python tests/save_load_test.py --exe build/text-adventure.exe
```

也提供常规 CMake/CTest 构建配置；此次本机验证使用 GCC 直接编译和上述 Python 测试。
本地证据保存在 `build/evidence/original` 和 `build/evidence/fixed`，包含输入命令、控制台日志和读档前后存档。
测试日志保存在 `build/original-tests.log`、`build/fixed-tests.log`。`build` 内容不纳入 Git。

修复能避免再次读丢物品。如果原存档中还有物品记录，修复版能够正确读取；如果已用原版读丢物品后覆盖保存，文件中已缺失的信息无法自动恢复。

