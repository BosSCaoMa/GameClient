#pragma once
#include <string>
#include <map>
#include <vector>
#include "BattleTypes.h"

// 前向声明（避免包含不必要的头文件，减少编译依赖）
class Character;
class Package;

// BattleAttr 结构体前向声明（因为 calculateCharacterPower 用到了 ch->baseAttr）
struct BattleAttr;

class Player {
public:
    // 成员变量（保持原有定义）
    int id = 0;
    std::string name;
    int level = 1;
    int vipLevel = 0;
    
    std::map<PlayerAttrType, int> attributes;  // 体力、精力等
    std::map<int, int> resources;              // 货币资源
    
    Character* character = nullptr;            // 主角
    uint64_t combatPower = 0;
    
    Package* package = nullptr;
    std::map<int, Character> characters;       // 拥有的武将
    std::vector<int> characterOrder;           // 上阵的武将 (最多5个)
    
public:
    // 构造函数声明
    Player();
    
    // 成员函数声明
    void modifyAttribute(PlayerAttrType type, int value);
    int getAttribute(PlayerAttrType type) const;
    std::vector<Character*> getBattleTeam();
    void updateCombatPower();
    bool saveDataToServer();
    
private:
    // 私有函数声明
    uint64_t calculateCharacterPower(Character* ch);
};