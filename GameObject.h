//
// Created by Edgar on 2023/11/6.
//

#ifndef TEXTADV_GAMEOBJECT_H
#define TEXTADV_GAMEOBJECT_H
#include <string>

using std::string;
class GameObject {
    const string* name;
    const string* description;
    const string* keyword;
public:
    GameObject(string* _name, string* _desc, string* _keyword);
    ~GameObject();
};


#endif //TEXTADV_GAMEOBJECT_H
