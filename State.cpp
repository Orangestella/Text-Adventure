

#include "State.h"
#include "wordwrap.h"
#include "strings.h"


/**
 * Current state of the game.
 */

/**
 * Display the description of the room the player is in. */

void State::announceLoc() const {
    this->currentRoom->describe();
}

/**
 * Constructor.
 * @param startRoom Pointer to the room to start in.
 */
State::State(Room *startRoom) : currentRoom(startRoom), strength(100) {};

/**
 * Move to a specified room and print its description.
 * @param target Pointer to the room to move to.
 */
void State::goTo(Room *target) {
    this->currentRoom = target;
    this->announceLoc();
}

/**
 * Return a pointer to the current room.
 * @return Pointer to the current room.
 */
Room* State::getCurrentRoom() const {
    return this->currentRoom;
}

/**
 * Change player’s physical strength.
 * @param variation Value used to change the strength.
 */
void State::changeStrength(int16_t variation) {
    this->strength += variation;
    if(this->strength > 100) strength = 100;
}

/**
 * Get the strength of current state
 * @return
 */
int16_t State::getStrength() const {
    return this->strength;
}

/**
 * Function to search for a specific object in the current room based on a keyword.
 * @param keyword The keyword to search for.
 * @param modify A flag indicating whether to modify the room's objects list. Default by false.
 * @return A pointer to the matching GameObject if found, otherwise nullptr.
 */
GameObject* State::searchRoom(const std::string &keyword, bool modify) {
    GameObject* objectFound = nullptr;
    std::vector<GameObject*>::iterator it;
    for (it=currentRoom->roomObjects.begin(); it != currentRoom->roomObjects.end(); it++){
        if (keyword == *((*it)->keyword)){
            objectFound = *it;
            if (modify) currentRoom->roomObjects.erase(it);
            break;
        }
    }
    return objectFound;
}

/**
 * Search for a specific object in the player's inventory based on a keyword.
 * @param keyword The keyword to search for.
 * @param modify A flag indicating whether to modify the inventory's objects list. Default by false.
 * @return A pointer to the matching GameObject if found, otherwise nullptr.
 */
GameObject* State::searchInventory(const std::string &keyword, bool modify) {
    GameObject* objectFound = nullptr;
    std::vector<GameObject*>::iterator it;
    for (it=this->inventory.begin(); it != this->inventory.end(); it++){
        if (keyword == *((*it)->keyword)){
            objectFound = *it;
            if (modify) this->inventory.erase(it);
            break;
        }
    }
    return objectFound;
}

/**
 * Put selected object into inventory and delete it from room
 * @param keyword The unique keyword of selected object
 * @return The state of operation, 0 for successfully operation, 1 for object already in inventory, 2 for the object not
 * in this room, 3 for this object doesn't exist.
 */
uint8_t State::pickObject(const std::string &keyword) {
    uint8_t operationState;
    GameObject* pickedObject = searchRoom(keyword, true);
    if (pickedObject != nullptr){
        this->inventory.push_back(pickedObject);
        operationState = 0; // successfully get
    }
    else if (searchInventory(keyword, false) != nullptr) operationState = 1; // already in inventory
    else if (GameObject::searchAll(keyword) != nullptr) operationState = 2; // not in this room
    else operationState = 3; // not exist
    return operationState;
}

/**
 * Drop object to thw room
 * @param keyword The unique keyword of selected object
 * @return The state of operation, 0 for successfully operation, 1 for object already in room, 2 for the object not
 * in this inventory, 3 for this object doesn't exist.
 */
uint8_t State::dropObject(const std::string &keyword) {
    uint8_t operationState;
    GameObject* pickedObject = searchInventory(keyword, true);
    if (pickedObject != nullptr){
        currentRoom->roomObjects.push_back(pickedObject);
        operationState = 0; // successfully drop
    }
    else if(searchRoom(keyword, false) != nullptr) operationState = 1; // already in room
    else if(GameObject::searchAll(keyword) == nullptr) operationState = 3; // not exist
    else operationState = 2; // not in inventory
    return operationState;
}

/**
 * Describe the objects in the inventory.
 */
void State::inventoryDescribe() const {
    wrapOut(&inventoryObjectsMessage);
    wrapEndPara();
    for (GameObject* object:inventory){
        wrapOut(object->keyword);
        wrapEndPara();
    }
}

/**
 * Eat the food and add strength
 * @param keyword the keyword of food
 * @return the state of operation, 0 for successfully eating, 1 for can't found that food in the inventory, 2 for the
 * selected object is not a food.
 */
uint8_t State::eat(const string &keyword) {
    uint8_t state = 0;
    if(keyword[0] == 'f'){
        auto* food = (FoodObject*)(this->searchInventory(keyword, true));
        if(food != nullptr) this->changeStrength(food->getEnergy());
        else state = 1;
    } else state = 2;
    return state;
}

