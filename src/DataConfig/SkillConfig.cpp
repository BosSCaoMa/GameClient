#include "SkillConfig.h"

void SkillConfig::initSkills() {
    using T = TargetType;
    using E = EffectType;
    using Tr = SkillTrigger;
    
    // ==================== 基础技能 ====================
    reg(Skill(0, "无技能", Tr::None)); // 占位无效技能
    
    // 普通攻击 (ID: 1-99 保留给基础技能)
    reg(Skill(1, "普通攻击", Tr::NORMAL_ATTACK)
        .addEffect(SkillEffect::Damage(T::ENEMY_SINGLE, 100)));
    
    // ==================== 物理伤害技能 (100-199) ====================
    
    reg(Skill(101, "破甲斩", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_SINGLE, 180))
        .addEffect(SkillEffect::Buff(T::ENEMY_SINGLE, E::BUFF_DEF, -30, 2)));
    
    reg(Skill(102, "旋风斩", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_ALL, 120)));
    
    reg(Skill(103, "雷霆一击", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_SINGLE, 200))
        .addEffect(SkillEffect::Control(T::ENEMY_SINGLE, E::STUN, 1, 50)));
    
    reg(Skill(104, "穿云箭", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_BACK_ROW, 150)));
    
    reg(Skill(105, "横扫千军", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_FRONT_ROW, 140)));
    
    reg(Skill(106, "斩首", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_HP_LOW1, 250)));
    
    reg(Skill(107, "连环击", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_RANDOM_3, 100)));
    
    reg(Skill(108, "暴风突刺", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_SINGLE, 160))
        .addEffect(SkillEffect::Buff(T::SELF, E::BUFF_ATK, 20, 2)));
    
    // ==================== 治疗技能 (200-299) ====================
    
    reg(Skill(201, "甘霖普降", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Heal(T::ALLY_ALL, 25)));
    
    reg(Skill(202, "妙手回春", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Heal(T::ALLY_HP_LOW1, 50)));
    
    reg(Skill(203, "战地急救", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Heal(T::ALLY_HP_LOW2, 35)));
    
    reg(Skill(204, "生命之泉", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Heal(T::ALLY_ALL, 20))
        .addEffect(SkillEffect::Buff(T::ALLY_ALL, E::BUFF_DEF, 15, 2)));
    
    // ==================== 辅助技能 (300-399) ====================
    
    reg(Skill(301, "战鼓", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Buff(T::ALLY_ALL, E::BUFF_ATK, 30, 2)));
    
    reg(Skill(302, "铁壁", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Buff(T::ALLY_ALL, E::BUFF_DEF, 30, 2)));
    
    reg(Skill(303, "疾风", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Buff(T::ALLY_ALL, E::BUFF_SPEED, 25, 2)));
    
    reg(Skill(304, "嘲讽", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Control(T::ENEMY_ALL, E::TAUNT, 2, 80)));
    
    reg(Skill(305, "护盾", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Shield(T::ALLY_ALL, 30)));
    
    // ==================== 控制技能 (400-499) ====================
    
    reg(Skill(401, "冰冻术", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Control(T::ENEMY_SINGLE, E::FREEZE, 1, 70))
        .addEffect(SkillEffect::Damage(T::ENEMY_SINGLE, 80)));
    
    reg(Skill(402, "沉默", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Control(T::ENEMY_ATK_TOP1, E::SILENCE, 2, 80)));
    
    reg(Skill(403, "群体眩晕", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Control(T::ENEMY_ALL, E::STUN, 1, 40)));
    
    // ==================== DOT技能 (500-599) ====================
    
    reg(Skill(501, "毒刃", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_SINGLE, 120))
        .addEffect(SkillEffect::Dot(T::ENEMY_SINGLE, E::POISON, 30, 3)));
    
    reg(Skill(502, "烈焰", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_ALL, 80))
        .addEffect(SkillEffect::Dot(T::ENEMY_ALL, E::BURN, 20, 2)));
    
    reg(Skill(503, "裂伤", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_SINGLE, 110))
        .addEffect(SkillEffect::Dot(T::ENEMY_SINGLE, E::BLEED, 40, 3)));
    
    // ==================== 被动技能 (600-699) ====================
    
    reg(Skill(601, "战意", Tr::BATTLE_START)
        .addEffect(SkillEffect::Buff(T::SELF, E::BUFF_ATK, 20, 99)));
    
    reg(Skill(602, "铁甲", Tr::BATTLE_START)
        .addEffect(SkillEffect::Buff(T::SELF, E::BUFF_DEF, 25, 99)));
    
    reg(Skill(603, "迅捷", Tr::BATTLE_START)
        .addEffect(SkillEffect::Buff(T::SELF, E::BUFF_SPEED, 30, 99)));
    
    reg(Skill(604, "回合恢复", Tr::ROUND_START)
        .addEffect(SkillEffect::Heal(T::SELF, 5)));
    
    reg(Skill(605, "死战不退", Tr::ON_LOW_HP)
        .addEffect(SkillEffect::Buff(T::SELF, E::BUFF_ATK, 50, 3))
        .addEffect(SkillEffect::Buff(T::SELF, E::BUFF_DEF, 50, 3)));
    
    reg(Skill(606, "反击", Tr::ON_HIT)
        .addEffect(SkillEffect::Damage(T::ENEMY_SINGLE, 50)));
    
    reg(Skill(607, "同归于尽", Tr::ON_DEATH)
        .addEffect(SkillEffect::Damage(T::ENEMY_ALL, 100)));
    
    // ==================== 特殊技能 (700-799) ====================
    
    reg(Skill(701, "复活术", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Revive(T::ALLY_HP_LOW1, 30)));
    
    reg(Skill(702, "怒气燃烧", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::RageAdd(T::ALLY_ALL, 30)));
    
    reg(Skill(703, "夺气", Tr::RAGE_SKILL)
        .addEffect(SkillEffect::Damage(T::ENEMY_SINGLE, 100))
        .addEffect(SkillEffect::RageReduce(T::ENEMY_SINGLE, 50)));
}
