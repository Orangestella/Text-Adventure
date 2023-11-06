//
// Created by Edgar on 2023/11/6.
//

#include "GameObject.h"
using std::string;

GameObject::GameObject(string* _name, string* _desc, string* _keyword):
        name(_name), description(_desc), keyword(_keyword){};
GameObject::~GameObject() = default;