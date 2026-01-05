#pragma once

#include <string>
enum class SkillType { // 主被动
    ACTIVE = 0,
    PASSIVE = 1,
};

enum class TargetType {
    SELF = 0,
    FRIEND = 1,
    ALLY = 2,
    ENEMY = 3,
    ALL_ENEMY = 4,
};

enum class EffectType { // 效果类型
    DAMAGE = 0,
    HEAL = 1,
    ImpAttc = 2,      // 增加攻击力
    ImpDef = 3,       // 增加防御力
    ImpSpeed = 4,     // 增加速度
    DebuffAttc = 5,   // 减少攻击力
    DebuffDef = 6,    // 减少防御力
    DebuffSpeed = 7,  // 减少速度
    // 其他效果类型
};

struct SkillTTE {
    TargetType targetType;
    SkillType skillType;
    EffectType effectType;
    long long effectValue; // 效果数值
    int subSkillId; // 附带子技能
};

class Skill {
public:
    Skill() = default;
    Skill(int id, const std::string& name, const std::string& description,
        const SkillTTE& skillTTE)
    : id(id), name(name), description(description), skillTTE(skillTTE) {}
    ~Skill() = default;
private:
    int id;
    std::string name;
    std::string description;

    SkillTTE skillTTE;
};