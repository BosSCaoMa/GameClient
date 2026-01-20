#pragma once
#include "BattleTypes.h"

// 只声明结构体和成员函数，不提供实现
struct SkillEffect {
    // 成员变量声明
    TargetType target;
    EffectType effect;
    ValueType valueType;
    int64_t value;
    int duration;       // 持续时间: 0 = 即时
    int chance;         // 触发概率: 100 = 100%
    // 构造函数声明
    SkillEffect();
    SkillEffect(TargetType t, EffectType e, ValueType vt, int64_t v, 
                int dur = 0, int ch = 100);
    
    static SkillEffect Damage_PA(TargetType t, int64_t atkPercent, int ch = 100);
    static SkillEffect DamageFixed(TargetType t, int64_t value, int ch = 100);
    static SkillEffect DamageByTargetHp(TargetType t, int64_t hpPercent, int ch = 100);
    static SkillEffect DamageByLostHp(TargetType t, int64_t lostHpPercent, int ch = 100);
    
    static SkillEffect Heal_T_HP(TargetType t, int64_t hpPercent, int ch = 100);
    static SkillEffect HealByAtk(TargetType t, int64_t atkPercent, int ch = 100);
    static SkillEffect HealFixed(TargetType t, int64_t value, int ch = 100);
    
    static SkillEffect Dot(TargetType t, EffectType dotType, int64_t atkPercent, 
                           int dur, int ch = 100);
    
    static SkillEffect Buff(TargetType t, EffectType e, int64_t val, int dur, int ch = 100);
    static SkillEffect BuffPercent(TargetType t, EffectType e, int64_t percent, 
                                    int dur, int ch = 100);
    
    static SkillEffect Control(TargetType t, EffectType e, int dur, int ch = 100);
    
    static SkillEffect Shield(TargetType t, int64_t hpPercent, int dur = 99, int ch = 100);
    static SkillEffect ShieldByAtk(TargetType t, int64_t atkPercent, int dur = 99, int ch = 100);
    static SkillEffect ShieldFixed(TargetType t, int64_t value, int dur = 99, int ch = 100);
    
    static SkillEffect RageChange(TargetType t, int64_t amount, int ch = 100);
    
    static SkillEffect Revive(TargetType t, int64_t hpPercent, int ch = 100);
    static SkillEffect Dispel(TargetType t, int64_t count = 99, int ch = 100);
    static SkillEffect Cleanse(TargetType t, int64_t count = 99, int ch = 100);
    static SkillEffect TransferDebuff(TargetType t, int ch = 100);
};