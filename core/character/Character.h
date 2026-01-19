#pragma once

#include <string>
#include <vector>
#include "Type.h"
#include <map>
enum class TargetType {
    // 基础阵营类型（保留原有核心类型，统一命名风格）
    SELF = 0,               // 自己
    ALLY_SINGLE = 1,        // 己方单个目标（替代原FRIEND，语义更精准）
    ALLY_ALL = 2,           // 己方全体（替代原ALLY）
    ENEMY_SINGLE = 3,       // 敌方单个目标（替代原ENEMY）
    ENEMY_ALL = 4,          // 敌方全体（保留原ALL_ENEMY，统一命名）

    // 按站位区分（前/后排）
    ALLY_FRONT_ROW = 5,     // 己方前排
    ALLY_BACK_ROW = 6,      // 己方后排
    ENEMY_FRONT_ROW = 7,    // 敌方前排
    ENEMY_BACK_ROW = 8,     // 敌方后排

    // 按攻击属性排序（攻击最高1/2/3人）
    ALLY_ATK_TOP1 = 9,      // 己方攻击最高1人
    ALLY_ATK_TOP2 = 10,     // 己方攻击最高2人
    ALLY_ATK_TOP3 = 11,     // 己方攻击最高3人
    ENEMY_ATK_TOP1 = 12,    // 敌方攻击最高1人
    ENEMY_ATK_TOP2 = 13,    // 敌方攻击最高2人
    ENEMY_ATK_TOP3 = 14,    // 敌方攻击最高3人

    // 按血量属性排序（血量最低1/2/3人）
    ALLY_HP_LOW1 = 15,      // 己方血量最低1人
    ALLY_HP_LOW2 = 16,      // 己方血量最低2人
    ALLY_HP_LOW3 = 17,      // 己方血量最低3人
    ENEMY_HP_LOW1 = 18,     // 敌方血量最低1人
    ENEMY_HP_LOW2 = 19,     // 敌方血量最低2人
    ENEMY_HP_LOW3 = 20,     // 敌方血量最低3人

    // 随机
    ALLY_RANDOM_1 = 21,   // 己方随机1人
    ALLY_RANDOM_2 = 22,   // 己方随机2人
    ALLY_RANDOM_3 = 23,   // 己方随机3人
    ENEMY_RANDOM_1 = 24,  // 敌方随机1人
    ENEMY_RANDOM_2 = 25,  // 敌方随机2人
    ENEMY_RANDOM_3 = 26   // 敌方随机3人
};
struct SkillTTE {
    TargetType targetType;
    std::map<BattleAttrType, long long> effects; // 属性效果集合
};

struct BattleAttr {
    long long hp = 0; // 生命值
    long long attack = 0; // 攻击力
    long long magicAttack = 0; // 法强
    long long phyDefense = 0; // 物理防御
    long long magDefense = 0; // 魔法防御
    int speed = 0; // 速度（影响出手顺序）
    int critRate = 0; // 暴击率(%)
    int critDamage = 0; // 暴击伤害倍率(%)
    int decritRate = 0; // 抗暴率(%)
    int dodgeRate = 0; // 闪避率(%)
    int rage = 0; // 怒气值
    std::vector<SkillTTE> skills; // 0-普通攻击，1-怒气技能，2-开局技能，3-被动技能。4-名刀技能，5-阵亡技能
};
class Character {
public:
    Character() = default;
    Character(int id, const std::string& name, int level, int stars, BattleAttr battleAttr = BattleAttr())
        : id(id), name(name), level(level), stars(stars), battleAttr(battleAttr) {}
    ~Character() = default;
public:
    int id;
    std::string name;
    // std::string description; // 描述信息，太长，后续放到专门文件

    int level;
    int stars;

    BattleAttr battleAttr; // 战斗属性
    std::vector<int> equipment;
};