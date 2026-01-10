#pragma once
#include "BattleTypes.h"

// ==================== 单个技能效果 ====================
struct SkillEffect {
    TargetType target;      // 作用目标
    EffectType effect;      // 效果类型
    ValueType valueType;    // 数值计算方式
    int64_t value;          // 基础数值（百分比用整数，如150表示150%）
    int duration;           // 持续回合（0=即时）
    int chance;             // 触发概率（100=100%）
    
    SkillEffect() = default;
    
    SkillEffect(TargetType t, EffectType e, ValueType vt, int64_t v, 
                int dur = 0, int ch = 100)
        : target(t), effect(e), valueType(vt), value(v), 
          duration(dur), chance(ch) {}
          
    // 便捷构造：伤害效果
    static SkillEffect Damage(TargetType t, int64_t percent) {
        return {t, EffectType::DAMAGE, ValueType::PERCENT_ATK, percent};
    }
    
    // 便捷构造：治疗效果
    static SkillEffect Heal(TargetType t, int64_t percent) {
        return {t, EffectType::HEAL, ValueType::PERCENT_MAX_HP, percent};
    }
    
    // 便捷构造：Buff效果
    static SkillEffect Buff(TargetType t, EffectType e, int64_t value, int duration) {
        return {t, e, ValueType::FIXED, value, duration};
    }
    
    // 便捷构造：控制效果
    static SkillEffect Control(TargetType t, EffectType e, int duration, int chance = 100) {
        return {t, e, ValueType::FIXED, 0, duration, chance};
    }
};
