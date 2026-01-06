#pragma once

#include <string>
#include <map>

class MyCharacter;
class Character;
class Package;
class Player {
public:
    void modifyAttribute(int attrType, int value) {
        // 修改玩家属性的实现
    }

private:
    int id;
    std::string name;
    int level;
    int vipLevel; // VIP等级

    std::map<int, int> attributes; // 玩家属性，如体力、精力、经验值等
    std::map<int, int> resources; // 货币等资源
    Character* character; // 玩家本身也是个武将
    long long combatPower; // 战力
    int combatPowerLevel; // 战力单位
    Package* package; // 玩家背包
    MyCharacter* characterPackage; // 玩家武将背包
};