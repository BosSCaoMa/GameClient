#include "SkillEffect.h"

// 构造函数实现
SkillEffect::SkillEffect() 
    : target(TargetType::SELF)
    , effect(EffectType::NONE)
    , valueType(ValueType::FIXED)
    , value(0)
    , duration(0)
    , chance(100) {}

SkillEffect::SkillEffect(TargetType t, EffectType e, ValueType vt, int64_t v, 
            int dur = 0, int ch = 100)
    : target(t), effect(e), valueType(vt), value(v), 
      duration(dur), chance(ch) {}

// ==================== 伤害类实现 ====================
SkillEffect SkillEffect::Damage(TargetType t, int64_t atkPercent, int ch) {
    return {t, EffectType::DAMAGE, ValueType::PERCENT_ATK, atkPercent, 0, ch};
}

SkillEffect SkillEffect::DamageFixed(TargetType t, int64_t value, int ch) {
    return {t, EffectType::DAMAGE, ValueType::FIXED, value, 0, ch};
}

SkillEffect SkillEffect::DamageByTargetHp(TargetType t, int64_t hpPercent, int ch) {
    return {t, EffectType::DAMAGE, ValueType::PERCENT_TARGET_HP, hpPercent, 0, ch};
}

SkillEffect SkillEffect::DamageByLostHp(TargetType t, int64_t lostHpPercent, int ch) {
    return {t, EffectType::DAMAGE, ValueType::PERCENT_LOST_HP, lostHpPercent, 0, ch};
}

// ==================== 治疗类实现 ====================
SkillEffect SkillEffect::Heal(TargetType t, int64_t hpPercent, int ch) {
    return {t, EffectType::HEAL, ValueType::PERCENT_MAX_HP, hpPercent, 0, ch};
}

SkillEffect SkillEffect::HealByAtk(TargetType t, int64_t atkPercent, int ch) {
    return {t, EffectType::HEAL, ValueType::PERCENT_ATK, atkPercent, 0, ch};
}

SkillEffect SkillEffect::HealFixed(TargetType t, int64_t value, int ch) {
    return {t, EffectType::HEAL, ValueType::FIXED, value, 0, ch};
}

// ====================DOT类（持续伤害） ====================
SkillEffect SkillEffect::Poison(TargetType t, int64_t atkPercent, int dur, int ch) {
    return {t, EffectType::POISON, ValueType::PERCENT_ATK, atkPercent, dur, ch};
}

SkillEffect SkillEffect::Burn(TargetType t, int64_t atkPercent, int dur, int ch) {
    return {t, EffectType::BURN, ValueType::PERCENT_ATK, atkPercent, dur, ch};
}

SkillEffect SkillEffect::Bleed(TargetType t, int64_t atkPercent, int dur, int ch) {
    return {t, EffectType::BLEED, ValueType::PERCENT_ATK, atkPercent, dur, ch};
}

// 通用DOT（可指定类型）
SkillEffect SkillEffect::Dot(TargetType t, EffectType dotType, int64_t atkPercent, 
                       int dur, int ch) {
    return {t, dotType, ValueType::PERCENT_ATK, atkPercent, dur, ch};
}

// ==================== Buff/Debuff类实现 ====================
SkillEffect SkillEffect::Buff(TargetType t, EffectType e, int64_t val, int dur, int ch) {
    return {t, e, ValueType::FIXED, val, dur, ch};
}

SkillEffect SkillEffect::BuffPercent(TargetType t, EffectType e, int64_t percent, 
                        int dur, int ch) {
    return {t, e, ValueType::PERCENT, percent, dur, ch};
}

// ==================== 控制类实现 ====================
// 控制效果（眩晕、沉默、冰冻）
SkillEffect SkillEffect::Control(TargetType t, EffectType e, int dur, int ch) {
    return {t, e, ValueType::FIXED, 0, dur, ch};
}
// 眩晕
SkillEffect SkillEffect::Stun(TargetType t, int dur, int ch) {
    return {t, EffectType::STUN, ValueType::FIXED, 0, dur, ch};
}
// 眩晕
SkillEffect SkillEffect::Silence(TargetType t, int dur, int ch) {
    return {t, EffectType::SILENCE, ValueType::FIXED, 0, dur, ch};
}
// 冰冻
SkillEffect SkillEffect::Freeze(TargetType t, int dur, int ch) {
    return {t, EffectType::FREEZE, ValueType::FIXED, 0, dur, ch};
}
// 嘲讽
SkillEffect SkillEffect::Taunt(TargetType t, int dur, int ch) {
    return {t, EffectType::TAUNT, ValueType::FIXED, 0, dur, ch};
}

// ==================== 护盾类实现 ====================

// 最大生命值百分比护盾
SkillEffect SkillEffect::Shield(TargetType t, int64_t hpPercent, int dur, int ch) {
    return {t, EffectType::SHIELD, ValueType::PERCENT_MAX_HP, hpPercent, dur, ch};
}

// 攻击力百分比护盾
SkillEffect SkillEffect::ShieldByAtk(TargetType t, int64_t atkPercent, int dur, int ch) {
    return {t, EffectType::SHIELD, ValueType::PERCENT_ATK, atkPercent, dur, ch};
}

// 固定值护盾
SkillEffect SkillEffect::ShieldFixed(TargetType t, int64_t value, int dur, int ch) {
    return {t, EffectType::SHIELD, ValueType::FIXED, value, dur, ch};
}

// ==================== 怒气类实现 ====================
SkillEffect SkillEffect::RageAdd(TargetType t, int64_t amount, int ch) {
    return {t, EffectType::RAGE_ADD, ValueType::FIXED, amount, 0, ch};
}

SkillEffect SkillEffect::RageReduce(TargetType t, int64_t amount, int ch) {
    return {t, EffectType::RAGE_REDUCE, ValueType::FIXED, amount, 0, ch};
}

// ==================== 特殊类实现 ====================
// 复活
SkillEffect SkillEffect::Revive(TargetType t, int64_t hpPercent, int ch) {
    return {t, EffectType::REVIVE, ValueType::PERCENT_MAX_HP, hpPercent, 0, ch};
}

// 驱散Buff
SkillEffect SkillEffect::Dispel(TargetType t, int64_t count, int ch) {
    return {t, EffectType::DISPEL, ValueType::FIXED, count, 0, ch};
}

// 净化Debuff
SkillEffect SkillEffect::Cleanse(TargetType t, int64_t count, int ch) {
    return {t, EffectType::CLEANSE, ValueType::FIXED, count, 0, ch};
}