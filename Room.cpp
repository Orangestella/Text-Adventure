
#include "Room.h"
#include "wordwrap.h"
#include "strings.h"

uint16_t Room::currentId = 1;
/**
 * Stores a static list of all rooms.
 */
std::list<Room*> Room::rooms;

/**
 * Room default constructor.
 * @param _name Room's name.
 * @param _desc Room's description.
 */
Room::Room(const string* _name, const string *_desc) :
        name(_name), description(_desc), north(nullptr),east(nullptr),west(nullptr),south(nullptr),id(currentId) {
    Room::currentId += 1;
};

/**
 * Remove destroyed rooms from the static list.
 */
Room::~Room() {
    Room::rooms.remove(this);
}

/**
 * Prints the description of a room (the name and long description)
 */
void Room::describe() const {
    wrapOut(this->name);
    wrapEndPara();
    wrapOut(this->description);
    wrapEndPara();
    wrapOut(&roomObjectsMessage);
    wrapEndPara();
    for (GameObject* gameObject: this->roomObjects){
        wrapOut(gameObject->name);
        wrapEndPara();
        wrapOut(gameObject->description);
        wrapEndPara();
    }
}

/**
 * Statically creates a room and then adds it to the global list.
 * @param _name Name for the new room.
 * @param _desc Description for the new room.
 * @return A pointer to the newly created room.
 */
Room* Room::addRoom(const string* _name, const string *_desc) {
    auto *newRoom = new Room(_name, _desc);
    Room::rooms.push_back(newRoom);
    return newRoom;
}

/**
 * Adds an existing room to the static list.
 * @param room Pointer to the room to add.
 * @return
 */
void Room::addRoom(Room* room) {
    Room::rooms.push_back(room);
}

/**
 * Gets the north exit of a room.
 * @return The room that is north of this one, or NULL if there is no exit.
 */
Room* Room::getNorth() const {
    return this->north;
}
Room* Room::getEast() const {
    return this->east;
}
Room* Room::getSouth() const {
    return this->south;
}
Room* Room::getWest() const {
    return this->west;
}

/**
 * Sets the north exit of a room.
 * @param _north Pointer to the room to be north of this one.
 */
void Room::setNorth(Room* _north) {
    this->north = _north;
}
void Room::setEast(Room* _east) {
    this->east = _east;
}
void Room::setSouth(Room* _south) {
    this->south = _south;
}
void Room::setWest(Room* _west) {
    this->west = _west;
}

void Room::addObject(GameObject* gameObject) {
    this->roomObjects.push_back(gameObject);
}

GameObject *Room::addObject(const string* _name, const string *_desc, const string* _keyword) {
    auto* object = new GameObject(_name, _desc, _keyword);
    this->roomObjects.push_back(object);
    return object;
}

string Room::exportObjects() {
    string idString;
    for(Room* room:rooms){
        idString.append(std::to_string(room->id));
        idString.append("\40");
        for(GameObject* object:room->roomObjects){
            idString.append(std::to_string(object->id));
            idString.append("\40");
        }
        idString.append("\n");
    }
    idString.erase(idString.end() - 1);
    return idString;
}