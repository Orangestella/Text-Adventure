
#include <iostream>
#include <memory>
#include <forward_list>
#include <map>
#include <chrono>
#include <thread>
#include "Room.h"
#include "wordwrap.h"
#include "State.h"
#include "strings.h"


using std::string;
using std::unique_ptr;
using std::map;

string commandBuffer;
State *currentState;
string secCommand;

/**
 * Print out the command prompt then read a command into the provided string buffer.
 * @param buffer Pointer to the string buffer to use.
 */
void inputCommand(string *buffer) {
    buffer->clear();
    std::cout << "> ";
    std::getline(std::cin, *buffer);
}

/**
 * Sets up the map.
 */
void initRooms() {
    /*auto * r2 = new Room(&r2name, &r2desc);
    auto * r1 = new Room(&r1name, &r1desc);
    Room room3(&r1name,&r1desc);
    Room* r3=&room3;
    Room::addRoom(r1);
    Room::addRoom(r2);
    Room::addRoom(r3);
    r1->setNorth(r2);
    r1->setSouth(r2);
    r2->setEast(r3);*/
    auto * r1 = Room::addRoom(&r1name,&r1desc);
    auto * r2 = Room::addRoom(&r2name,&r2desc);
    auto * r3 = Room::addRoom(&r3name,&r3desc);
    auto * r4 = Room::addRoom(&r4name,&r4desc);
    auto * r5 = Room::addRoom(&r5name,&r5desc);

    map<Room*, Room*> mapRoomWest = {{r1,r5},{r3,r1}};
    map<Room*, Room*> mapRoomEast = {{r1,r3},{r5,r1}};
    map<Room*, Room*> mapRoomNorth = {{r1,r2},{r4,r1}};
    map<Room*, Room*> mapRoomSouth = {{r1,r4},{r2,r1}};

    for (auto room:mapRoomNorth){
        room.first ->setNorth(room.second);
    }
    for (auto room:mapRoomEast){
        room.first ->setEast(room.second);
    }
    for (auto room:mapRoomWest){
        room.first ->setWest(room.second);
    }
    for (auto room:mapRoomSouth){
        room.first ->setSouth(room.second);
    }

    /*Object*/
    auto o1 = new FoodObject(&o1name,&o1desc,&o1key,10); // golden apple
    auto o2 = new FoodObject(&o2name,&o2desc,&o2key,5); // honey cake
    auto o3 = new FoodObject(&o3name,&o3desc,&o3key,8); // brownie
    auto o4 = new FoodObject(&o4name,&o4desc,&o4key,6); // sugar rush
    auto o5 = new GameObject(&o5name,&o5desc,&o5key); // ak47
    auto o6 = new GameObject(&o6name,&o6desc,&o6key); // single scissor
    auto o7 = new GameObject(&o7name,&o7desc,&o7key); // melon knife

    r1->addObject(o1);
    r2->addObject(o2);
    r3->addObject(o3);
    r3->addObject(o5);
    r4->addObject(o4);
    r4->addObject(o6);
    r5->addObject(o7);
}

/**
 * Sets up the game state.
 */
void initState() {
    currentState = new State(Room::rooms.front());
}

/**
 * Reduce strength by 1 per minute.
 */
[[noreturn]] void timerThread() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::minutes(1));
        currentState->changeStrength(-1);
        if (currentState->getStrength() <= 0) {
            wrapOut(&terminateMessage0);
            wrapEndPara();
            exit(0);
        }
    }
}

/**
 * The main game loop.
 */
void gameLoop() {
    bool gameOver=false;
    std::thread timer(timerThread);
    timer.detach();
    while (!gameOver) {
        /* Ask for a command. */
        bool commandOk = false;
        inputCommand(&commandBuffer);

        /* The first word of a command would normally be the verb. The first word is the text before the first
         * space, or if there is no space, the whole string. */
        auto endOfVerb = static_cast<uint8_t>(commandBuffer.find(' '));
        /*Check if the command string contains space delimiters*/
        if(endOfVerb != static_cast<uint8_t>(std::string::npos)) secCommand = commandBuffer.substr(endOfVerb+1);
        else secCommand = "NULL"; //

        /* We could copy the verb to another string but there's no reason to, we'll just compare it in place. */
        /* Command to go north. */
        if ((commandBuffer.compare(0,endOfVerb,"north") == 0) || (commandBuffer.compare(0,endOfVerb,"n") == 0)) {
            commandOk = true; /* Confirm command has been handled */
            /* See if there's a north exit */
            Room *northRoom = currentState->getCurrentRoom()->getNorth();
            if (northRoom == nullptr) { /* there isn't */
                wrapOut(&badExit);      /* Output the "can't go there" message */
                wrapEndPara();
            } else {                    /* There is */
                currentState->goTo(northRoom); /* Update state to that room - this will also inventoryDescribe it */
            }
        }
        else if ((commandBuffer.compare(0,endOfVerb,"east") == 0) || (commandBuffer.compare(0,endOfVerb,"e") == 0)) {
            commandOk = true; /* Confirm command has been handled */
            /* See if there's an east exit */
            Room *eastRoom = currentState->getCurrentRoom()->getEast();
            if (eastRoom == nullptr) { /* there isn't */
                wrapOut(&badExit);      /* Output the "can't go there" message */
                wrapEndPara();
            } else {                    /* There is */
                currentState->goTo(eastRoom); /* Update state to that room - this will also inventoryDescribe it */
            }
        }
        else if ((commandBuffer.compare(0,endOfVerb,"west") == 0) || (commandBuffer.compare(0,endOfVerb,"w") == 0)) {
            commandOk = true; /* Confirm command has been handled */
            /* See if there's a west exit */
            Room *westRoom = currentState->getCurrentRoom()->getWest();
            if (westRoom == nullptr) { /* there isn't */
                wrapOut(&badExit);      /* Output the "can't go there" message */
                wrapEndPara();
            } else {                    /* There is */
                currentState->goTo(westRoom); /* Update state to that room - this will also inventoryDescribe it */
            }
        }
        else if ((commandBuffer.compare(0,endOfVerb,"south") == 0) || (commandBuffer.compare(0,endOfVerb,"s") == 0)) {
            commandOk = true; /* Confirm command has been handled */
            /* See if there's a south exit */
            Room *southRoom = currentState->getCurrentRoom()->getSouth();
            if (southRoom == nullptr) { /* there isn't */
                wrapOut(&badExit);      /* Output the "can't go there" message */
                wrapEndPara();
            } else {                    /* There is */
                currentState->goTo(southRoom); /* Update state to that room - this will also inventoryDescribe it */
            }
        }
        else if(commandBuffer.compare(0,endOfVerb,"inventory") == 0){
            commandOk = true;
            currentState->inventoryDescribe();
        }
        else if(commandBuffer.compare(0,endOfVerb,"get") == 0){
            commandOk = true;
            if (secCommand!="NULL") {
                uint8_t state = currentState->pickObject(secCommand); // Gain the state of the operation.
                switch (state) {
                    case 0: // picked successfully.
                        wrapOut(&pickObjectMessage0);
                        wrapEndPara();
                        break;
                    case 1: // already in the inventory.
                        wrapOut(&pickObjectMessage1);
                        wrapEndPara();
                        break;
                    case 2: // cannot find.
                        wrapOut(&pickObjectMessage2);
                        wrapEndPara();
                        break;
                    case 3: // no exist.
                        wrapOut(&objectNotFound);
                        wrapEndPara();
                        break;
                    default: // bad command.
                        wrapOut(&badCommand);
                        wrapEndPara();
                }
            }else{
                wrapOut(&badCommand);
                wrapEndPara();
            }
        }
        else if(commandBuffer.compare(0,endOfVerb,"drop") == 0) {
            commandOk = true;
            if (secCommand != "NULL") {
                uint8_t state = currentState->dropObject(secCommand);
                switch (state) {
                    case 0:  // dropped successfully.
                        wrapOut(&dropObjectMessage0);
                        wrapEndPara();
                        break;
                    case 1: // already in the room.
                        wrapOut(&dropObjectMessage1);
                        wrapEndPara();
                        break;
                    case 2: // cannot find.
                        wrapOut(&dropObjectMessage2);
                        wrapEndPara();
                        break;
                    case 3: // no exist.
                        wrapOut(&objectNotFound);
                        wrapEndPara();
                        break;
                    default: // bad command.
                        wrapOut(&badCommand);
                        wrapEndPara();
                }
            }else{
                wrapOut(&badCommand);
                wrapEndPara();
            }
        }
        else if(commandBuffer.compare(0,endOfVerb,"examine") == 0) {
            commandOk = true;
            if (secCommand != "NULL") {
                GameObject *object = GameObject::searchAll(secCommand);
                if (object != nullptr) GameObject::describe(object);
                else {
                    wrapOut(&objectNotFound);
                    wrapEndPara();
                }
            }else{
                wrapOut(&badCommand);
                wrapEndPara();
            }
        }
        else if(commandBuffer.compare(0,endOfVerb,"eat") == 0){
            commandOk = true;
            if(secCommand!="NULL") {
                uint8_t state = currentState->eat(secCommand);
                string info = "Your strength is\40" + std::to_string(currentState->getStrength());
                switch (state) {
                    case 0:
                        wrapOut(&eatFoodMessage0);
                        wrapEndPara();
                        wrapOut(&info);
                        wrapEndPara();
                        break;
                    case 1:
                        wrapOut(&eatFoodMessage1);
                        wrapEndPara();
                        break;
                    case 2:
                        wrapOut(&eatFoodMessage2);
                        wrapEndPara();
                        break;
                    default:
                        wrapOut(&badCommand);
                        wrapEndPara();
                }
            }else{
                wrapOut(&badCommand);
                wrapEndPara();
            }
        }

        /* Quit command */
        if ((commandBuffer.compare(0,endOfVerb,"quit") == 0)) {
            commandOk = true;
            gameOver = true;
        }

        /* If commandOk hasn't been set, command wasn't understood, display error message */
        if(!commandOk) {
            wrapOut(&badCommand);
            wrapEndPara();
        }
    }
}


int main() {
    initWordWrap();
    initRooms();
    initState();
    currentState->announceLoc();
    gameLoop();
    return 0;
}