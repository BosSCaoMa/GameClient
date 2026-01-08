#pragma once

#include <string>
#include <vector>
#include "Type.h"

enum class TargetType {
    SELF = 0,
    FRIEND = 1,
    ALLY = 2,
    ENEMY = 3,
    ALL_ENEMY = 4,
};


struct SkillTTE {
    TargetType targetType;
    std::map<BattleAttrType, long long> effects; // 属性效果集合
};

class Character {
public:
    Character() = default;
    Character(int id, const std::string& name, const std::string& description,
              int level, int stars, long long hp, long long attack,
              long long phyDefense, long long magDefense, int speed,
              const std::vector<int>& skills)
        : id(id), name(name), description(description),
          level(level), stars(stars), hp(hp), attack(attack),
          phyDefense(phyDefense), magDefense(magDefense),
          speed(speed), skills(skills) {}
    ~Character() = default;
public:
    int id;
    std::string name;
    std::string description;

    int level;
    int stars;
    long long hp; // 生命值
    long long attack; // 攻击力
    long long magicAttack; // 法强
    long long phyDefense; // 物理防御
    long long magDefense; // 魔法防御
    int speed;  // 速度（影响出手顺序）
    int critRate; // 暴击率(%)
    int critDamage; // 暴击伤害倍率
    int decritRate; // 抗暴率(%)
    int dodgeRate; // 闪避率(%)
    std::vector<SkillTTE> skills; // 0-普通攻击，1-怒气技能，2-开局技能，3-被动技能。4-阵亡技能
    std::vector<int> equipment;
};