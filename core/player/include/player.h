#pragma once

#include <string>
#include <map>

class Character;
class Player {
public:
    void modifyAttribute(int attrType, int value) {
        // 修改玩家属性的实现
    }

private:
    int id;
    std::string name;
    int level;
    int vipLevel;
    int hp;

    std::map<int, int> attributes;
    std::map<int, int> resources;
    Character* character;
    long long combatPower;
    int combatPowerLevel;
};