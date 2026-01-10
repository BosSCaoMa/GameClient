#pragma once
#include "BattleAttr.h"
#include "Skill.h"
#include <string>
#include <vector>
#include <array>

class Character {
public:
    int id = 0;
    std::string name;
    
    int level = 1;
    int stars = 1;
    int exp = 0;
    
    BattleAttr baseAttr;    // 基础属性
    
    std::vector<int> equipmentIds;  // 装备ID列表
    std::array<Skill, 9> skills;  // 按SkillTrigger索引,比如skillIds[0]处的技能代表是普通攻击
    
public:
    Character(int id, const std::string& name, int level = 1, int stars = 1)
        : id(id), name(name), level(level), stars(stars) {
        skills[static_cast<int>(SkillTrigger::NORMAL_ATTACK)] = SkillFactory::NormalAttack();
    }
    // 设置技能
    void setSkill(SkillTrigger trigger, const Skill& skill) {
        skills[static_cast<int>(trigger)] = skill;
    }
    
    Skill& getSkill(SkillTrigger trigger) {
        return skills[static_cast<int>(trigger)];
    }
    
    const Skill& getSkill(SkillTrigger trigger) const {
        return skills[static_cast<int>(trigger)];
    }
    
    BattleAttr getTotalAttr() const {
        BattleAttr total = baseAttr;
        // TODO: 装备加成
        return total;
    }
};
