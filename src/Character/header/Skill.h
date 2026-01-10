#pragma once
#include "BattleTypes.h"
#include <string>
#include <vector>

// ==================== 单个技能效果 ====================
struct SkillEffect {
    TargetType target;
    EffectType effect;
    ValueType valueType;
    int64_t value;
    int duration;       // 持续时间: 0 = 即时
    int chance;         // 触发概率: 100 = 100%
    
    SkillEffect() = default;
    
    SkillEffect(TargetType t, EffectType e, ValueType vt, int64_t v, 
                int dur = 0, int ch = 100)
        : target(t), effect(e), valueType(vt), value(v), 
          duration(dur), chance(ch) {}
    
    // 便捷构造:todo补充
    static SkillEffect Damage(TargetType t, int64_t atkPercent) {
        return {t, EffectType::DAMAGE, ValueType::PERCENT_ATK, atkPercent};
    }
    
    static SkillEffect Heal(TargetType t, int64_t hpPercent) {
        return {t, EffectType::HEAL, ValueType::PERCENT_MAX_HP, hpPercent};
    }
    
    static SkillEffect Buff(TargetType t, EffectType e, int64_t val, int dur) {
        return {t, e, ValueType::FIXED, val, dur};
    }
    
    static SkillEffect Control(TargetType t, EffectType e, int dur, int ch = 100) {
        return {t, e, ValueType::FIXED, 0, dur, ch};
    }
};

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

// ==================== 预定义技能（后续拓展）todo是否放到源文件 ====================
namespace SkillFactory {
    inline Skill NormalAttack() {
        return Skill(0, "普通攻击", SkillTrigger::NORMAL_ATTACK)
            .addEffect(SkillEffect::Damage(TargetType::ENEMY_SINGLE, 100));
    }
    
    inline Skill ArmorBreak() {
        return Skill(101, "破甲斩", SkillTrigger::RAGE_SKILL)
            .addEffect(SkillEffect::Damage(TargetType::ENEMY_SINGLE, 180))
            .addEffect(SkillEffect::Buff(TargetType::ENEMY_SINGLE, 
                       EffectType::BUFF_DEF, -30, 2));
    }
    
    inline Skill Whirlwind() {
        return Skill(102, "旋风斩", SkillTrigger::RAGE_SKILL)
            .addEffect(SkillEffect::Damage(TargetType::ENEMY_ALL, 120));
    }
    
    inline Skill GroupHeal() {
        return Skill(201, "甘霖普降", SkillTrigger::RAGE_SKILL)
            .addEffect(SkillEffect::Heal(TargetType::ALLY_ALL, 25));
    }
    
    inline Skill ThunderStrike() {
        return Skill(103, "雷霆一击", SkillTrigger::RAGE_SKILL)
            .addEffect(SkillEffect::Damage(TargetType::ENEMY_SINGLE, 200))
            .addEffect(SkillEffect::Control(TargetType::ENEMY_SINGLE, 
                       EffectType::STUN, 1, 50));
    }
}
