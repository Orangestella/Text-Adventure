//
// Created by Edgar on 2023/11/6.
//

#include <stdexcept>
#include "GameObject.h"
#include "wordwrap.h"
using std::string;

/*Global object list*/
std::list<GameObject*> GameObject::allObjects;
uint16_t GameObject::currentId = 0;

/**
 * The constructor of class GameObject, construct the object and push it into global objects list.
 * @param _name Name of the object.
 * @param _desc Description of the object.
 * @param _keyword
 */
GameObject::GameObject(const string* _name, const string* _desc, const string* _keyword):
        name(_name), description(_desc), keyword(_keyword), id(GameObject::currentId){
    GameObject::allObjects.push_back(this);
    GameObject::currentId += 1;
};
GameObject::~GameObject() = default;

/**
 * The constructor of class FoodObject, limits the value of energy in a range of 1-10.
 * @param _name Name of the food object.
 * @param _desc Description of the food object.
 * @param _keyword Keyword of the food object.
 * @param _energy Energy of the food object, should be limited in a range of 1-10. The default value is 1.
 */
FoodObject::FoodObject(const std::string *_name, const std::string *_desc, const std::string *_keyword, uint8_t _energy):
        GameObject(_name,_desc,_keyword),energy(_energy){
    if(!(_energy >= 1 && _energy <= 10)){
        energy = 1;
    }
}

/**
 * Search the object with matched keyword in the whole game.
 * @param keyword The keyword of wanted object.
 * @return The pointer points to the searched object, or nullptr if not found.
 */
GameObject* GameObject::searchAll(const std::string &keyword) {
    GameObject* objectFound = nullptr;
    for (GameObject* object:allObjects){
        if(*(object->keyword) == keyword){
            objectFound = object;
            break;
        }
    }
    return objectFound;
}

/**
 * Print out the description of the object.
 * @param object The object to be described.
 */
void GameObject::describe(GameObject* object) {
    wrapOut(object->description);
    wrapEndPara();
}

string GameObject::getType() {
    return "default";
}

/**
 * Get the energy of the food.
 * @return The energy of the food
 */
uint8_t FoodObject::getEnergy() const {
    return this->energy;
}

string FoodObject::getType() {
    return "food";
}