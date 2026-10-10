
#include <sstream>
#include <unordered_map>
#include <unordered_set>
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
    // The timer thread also updates strength, including during save/load.
    int16_t previous = strength.load();
    int16_t next;
    do {
        int value = static_cast<int>(previous) + variation;
        if (value > 100) value = 100;
        if (value < 0) value = 0;
        next = static_cast<int16_t>(value);
    } while (!strength.compare_exchange_weak(previous, next));
}

/**
 * Get the strength of current state
 * @return
 */
int16_t State::getStrength() const {
    return this->strength.load();
}

/**
 * Function to search for a specific object in the current room based on a keyword.
 * @param keyword The keyword to search for.
 * @return A pointer to the matching GameObject if found, otherwise nullptr.
 */
GameObject* State::searchRoom(const std::string &keyword) {
    GameObject* objectFound = nullptr;
    for(GameObject* object:currentRoom->roomObjects){
        if (keyword == *(object->keyword)){
            objectFound = object;
            break;
        }
    }
    return objectFound;
}

/**
 * Search for a specific object in the player's inventory based on a keyword.
 * @param keyword The keyword to search for.
 * @return A pointer to the matching GameObject if found, otherwise nullptr.
 */
GameObject* State::searchInventory(const std::string &keyword) {
    GameObject* objectFound = nullptr;
    for (GameObject* object:inventory){
        if (keyword == *(object->keyword)){
            objectFound = object;
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
    GameObject* pickedObject = searchRoom(keyword);
    if (pickedObject != nullptr){
        this->inventory.push_back(pickedObject);
        currentRoom->roomObjects.remove(pickedObject);
        operationState = 0; // successfully get
    }
    else if (searchInventory(keyword) != nullptr) operationState = 1; // already in inventory
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
    GameObject* droppedObject = searchInventory(keyword);
    if (droppedObject != nullptr){
        currentRoom->roomObjects.push_back(droppedObject);
        inventory.remove(droppedObject);
        operationState = 0; // successfully drop
    }
    else if(searchRoom(keyword) != nullptr) operationState = 1; // already in room
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
        wrapOut(object->name);
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
    GameObject* food = searchInventory(keyword);
    if (food == nullptr) state = 1;
    else if(food->getType() == "food") {
        this->changeStrength(dynamic_cast<FoodObject*>(food)->getEnergy());
        inventory.remove(food);
    }
    else state = 2;
    return state;
}

/**
 * Export the inventory in plain text.
 * @return The string with the information of objects in the inventory.
 */
string State::exportInventory() {
    string idString = "0\40";
    for(GameObject* object:inventory){
        idString.append(std::to_string(object->id));
        idString.append("\40");
    }
    return idString;
}

/**
 * Load the state of game from a file stream.
 * @param ifs The input file stream to load state.
 */
bool State::loadState(std::istream &ifs) {
    auto fail = [](const string& message) {
        wrapOut(&message);
        wrapEndPara();
        return false;
    };
    if (!ifs.good()) return fail(loadFailMessage);

    string line;
    if (!std::getline(ifs, line)) return fail(validateFailMessage);
    if (!line.empty() && line.back() == '\r') line.pop_back();
    if (line != "EDGAR VER1.3.0") return fail(validateFailMessage);

    // Extract only successful, bounded decimal tokens. EOF is not a token.
    auto parseNumbers = [](const string& text, std::vector<uint16_t>& values) {
        std::istringstream input(text);
        string token;
        while (input >> token) {
            unsigned int value = 0;
            for (char digit : token) {
                if (digit < '0' || digit > '9') return false;
                value = value * 10 + static_cast<unsigned int>(digit - '0');
                if (value > UINT16_MAX) return false;
            }
            values.push_back(static_cast<uint16_t>(value));
        }
        return !input.bad();
    };

    std::vector<uint16_t> header;
    if (!std::getline(ifs, line) || !parseNumbers(line, header) ||
        header.size() != 2 || header[0] < 1 || header[0] > 100) {
        return fail(validateFailMessage);
    }

    std::unordered_map<uint16_t, Room*> roomsById;
    for (Room* room : Room::rooms) roomsById.emplace(room->id, room);
    const auto targetRoom = roomsById.find(header[1]);
    if (targetRoom == roomsById.end()) return fail(validateFailMessage);
    std::unordered_map<uint16_t, GameObject*> objectsById;
    for (GameObject* object : GameObject::allObjects) objectsById.emplace(object->id, object);

    // Build a complete candidate snapshot before changing any live state.
    std::unordered_map<uint16_t, std::list<GameObject*>> locations;
    std::unordered_set<uint16_t> seenObjects;
    while (std::getline(ifs, line)) {
        std::vector<uint16_t> ids;
        if (!parseNumbers(line, ids)) return fail(validateFailMessage);
        if (ids.empty()) continue; // Blank lines do not describe a location.
        const uint16_t locationId = ids.front();
        if ((locationId != 0 && roomsById.count(locationId) == 0) ||
            locations.count(locationId) != 0) return fail(validateFailMessage);

        std::list<GameObject*> objects;
        for (size_t i = 1; i < ids.size(); ++i) {
            const auto object = objectsById.find(ids[i]);
            if (object == objectsById.end() || !seenObjects.insert(ids[i]).second) {
                return fail(validateFailMessage);
            }
            objects.push_back(object->second);
        }
        locations.emplace(locationId, std::move(objects));
    }
    if (ifs.bad() || !ifs.eof() || locations.count(0) == 0 ||
        locations.size() != roomsById.size() + 1) return fail(validateFailMessage);

    // Objects absent from all locations (e.g. eaten food) remain absent.
    inventory.swap(locations.at(0));
    for (const auto& entry : roomsById) {
        entry.second->roomObjects.swap(locations.at(entry.first));
    }
    strength = static_cast<int16_t>(header[0]);
    currentRoom = targetRoom->second;
    wrapOut(&loadOkMessage);
    wrapEndPara();
    return true;
}
