#pragma once

#include <string>
#include <map>

class Character;
class MyCharacter {
public:
    MyCharacter() = default;
    ~MyCharacter() = default;
    std::map<int, Character> characters;
     // 武将ID -> 武将对象
};