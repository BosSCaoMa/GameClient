#pragma once
#include "BattleTypes.h"
#include <string>
#include <vector>
#include "SkillEffect.h"

// ==================== Buff实例 ====================
struct Buff {
    EffectType type; // Buff类型
    int64_t value; // Buff数值
    int duration; // 剩余回合
    int sourceId; // 来源技能ID（用于Buff刷新判断）
    bool isDebuff; // 是否为负面效果
    
    Buff(EffectType t, int64_t v, int dur, int src = 0)
        : type(t), value(v), duration(dur), sourceId(src) {
        isDebuff = IsDebuff(t);
    }
    
    bool tick() { return --duration <= 0; }
};

// ==================== 技能 ====================
class Skill {
public:
    int id = 0;
    std::string name;
    std::string description;

    SkillTrigger trigger; // 触发时机
    std::vector<SkillEffect> effects; // 效果列表
    
    Skill() = default;
    
    Skill(int id, const std::string& name, SkillTrigger trigger)
        : id(id), name(name), trigger(trigger) {}
    
    Skill& addEffect(const SkillEffect& effect) {
        effects.push_back(effect);
        return *this;
    }
};
