#pragma once

#include <string>

enum class ResourceType {
    SILVER = 0,
    GOLD = 1,
    CRYSTAL = 2,
    // 货币类型添加到上面
    WOOD = 10,
    STONE = 11
};

enum class BattleAttrType {
    hp = 0,
    attack = 1,
    phyDefense = 2,
    magDefense = 3,
    speed = 4,
    critRate = 5,
    critDamage = 6,
    decritRate = 7,
    dodgeRate = 8, // 闪避率
    rage = 9
};