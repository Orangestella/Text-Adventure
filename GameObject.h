//
// Created by Edgar on 2023/11/6.
//

#ifndef TEXTADV_GAMEOBJECT_H
#define TEXTADV_GAMEOBJECT_H
#include <string>
#include <cstdint>
#include <list>

using std::string;
class GameObject {
public:
    const string* name;
    const string* description;
    const string* keyword;
    static std::list<GameObject*> allObjects;
    static GameObject* searchAll(const std::string &keyword);
    static void describe(GameObject* object);
    GameObject(const string* _name, const string* _desc, const string* _keyword);
    ~GameObject();
};

class FoodObject:public GameObject{
private:
    uint8_t energy;
public:
    FoodObject(const string* _name, const string* _desc, const string* _keyword, uint8_t _energy);
    uint8_t getEnergy() const;
};


#endif //TEXTADV_GAMEOBJECT_H
