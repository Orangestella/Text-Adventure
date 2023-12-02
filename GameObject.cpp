//
// Created by Edgar on 2023/11/6.
//

#include <stdexcept>
#include "GameObject.h"
#include "wordwrap.h"
using std::string;

std::list<GameObject*> GameObject::allObjects;

GameObject::GameObject(const string* _name, const string* _desc, const string* _keyword):
        name(_name), description(_desc), keyword(_keyword){
    GameObject::allObjects.push_back(this);
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
        throw std::invalid_argument("Energy should be limited in a range of 1-10");
    }
}

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

void GameObject::describe(GameObject* object) {
    wrapOut(object->description);
    wrapEndPara();
}

uint8_t FoodObject::getEnergy() const {
    return this->energy;
}