#include "Skill.h"

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