

#ifndef TEXTADV_STATE_H
#define TEXTADV_STATE_H


#include <cstdint>
#include "Room.h"

class State {
    Room *currentRoom;
    std::list<GameObject*> inventory;
    int16_t strength;
    GameObject* searchRoom(const string &keyword);
    GameObject* searchInventory(const string &keyword);
public:
    explicit State(Room *startRoom);
    void goTo(Room *target);
    void announceLoc() const;
    void changeStrength(int16_t variation);
    int16_t getStrength() const;
    Room* getCurrentRoom() const;
    uint8_t pickObject(const string &keyword);
    uint8_t dropObject(const string &keyword);
    void inventoryDescribe() const;
    uint8_t eat(const string &keyword);
    string exportInventory();
    void loadState(std::ifstream&);
};


#endif //TEXTADV_STATE_H
