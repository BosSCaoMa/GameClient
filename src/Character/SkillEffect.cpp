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
            int dur, int ch)
    : target(t), effect(e), valueType(vt), value(v), 
      duration(dur), chance(ch) {}

// ==================== 1、伤害类实现 ====================
SkillEffect SkillEffect::Damage_PA(TargetType t, int64_t atkPercent, int ch) {
    return {t, EffectType::DAMAGE, ValueType::PERCENT_ATK, atkPercent, 0, ch};
}

SkillEffect SkillEffect::DamageFixed(TargetType t, int64_t value, int ch) {
    return {t, EffectType::DAMAGE, ValueType::FIXED, value, 0, ch};
}

SkillEffect SkillEffect::DamageByTargetHp(TargetType t, int64_t hpPercent, int ch) {
    return {t, EffectType::DAMAGE, ValueType::PERCENT_TARGET_MAXHP, hpPercent, 0, ch};
}

SkillEffect SkillEffect::DamageByLostHp(TargetType t, int64_t lostHpPercent, int ch) {
    return {t, EffectType::DAMAGE, ValueType::PERCENT_LOST_HP, lostHpPercent, 0, ch};
}

// ==================== 2、治疗类实现 ====================
SkillEffect SkillEffect::Heal_T_HP(TargetType t, int64_t hpPercent, int ch) {
    return {t, EffectType::HEAL, ValueType::PERCENT_TARGET_MAXHP, hpPercent, 0, ch};
}

SkillEffect SkillEffect::HealByAtk(TargetType t, int64_t atkPercent, int ch) {
    return {t, EffectType::HEAL, ValueType::PERCENT_ATK, atkPercent, 0, ch};
}

SkillEffect SkillEffect::HealFixed(TargetType t, int64_t value, int ch) {
    return {t, EffectType::HEAL, ValueType::FIXED, value, 0, ch};
}

// ============3、DOT类（持续伤害）POISON BURN BLEED CURSE ====================
// 
SkillEffect SkillEffect::Dot(TargetType t, EffectType dotType, int64_t atkPercent, 
    int dur, int ch) {
    return {t, dotType, ValueType::PERCENT_ATK, atkPercent, dur, ch};
}

// ==================== 4、Buff/Debuff类实现【目前buff类只能通过百分比提升】 ====================
// 攻击力、防御、速度等等
SkillEffect SkillEffect::Buff(TargetType t, EffectType e, int64_t val, int dur, int ch) {
    return {t, e, ValueType::FIXED, val, dur, ch};
}

// ==================== 5、护盾类实现 【目标类型 -数值】====================

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

// ==================== 6、怒气类实现 ====================
SkillEffect SkillEffect::RageChange(TargetType t, int64_t amount, int ch) {
    return {t, EffectType::RAGE_ADD, ValueType::FIXED, amount, 0, ch};
}

// ==================7、控制与反控制类实现 ======================
// =====================【目标 - 等级 - 触发概率】=======================

// 控制效果（眩晕、沉默、冰冻、嘲讽、受伤）
SkillEffect SkillEffect::Control(TargetType t, EffectType e, int dur, int ch) {
    return {t, e, ValueType::FIXED, 0, dur, ch};
}

// 驱散Buff
SkillEffect SkillEffect::Dispel(TargetType t, int64_t count, int ch) {
    return {t, EffectType::DISPEL, ValueType::FIXED, count, 0, ch};
}

// 净化Debuff 
SkillEffect SkillEffect::Cleanse(TargetType t, int64_t count, int ch) {
    return {t, EffectType::CLEANSE, ValueType::FIXED, count, 0, ch};
}

// 转移Debuff
SkillEffect SkillEffect::TransferDebuff(TargetType t, int ch) {
    return {t, EffectType::TRANSFER_DEBUFF, ValueType::FIXED, 0, 0, ch};
}

// ==================== 特殊类实现 ====================
SkillEffect SkillEffect::Revive(TargetType t, int64_t hpPercent, int ch) {
    return {t, EffectType::REVIVE, ValueType::PERCENT_MAX_HP, hpPercent, 0, ch};
}