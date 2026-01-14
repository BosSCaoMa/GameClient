#include "SkillConfig.h"

void SkillConfig::initSkills()
{
    initBaseSkills();
    initEquipSkills();
    initQunSkills();
    initWeiSkills();
    initShuSkills();
    initWuSkills();
    initShenSkills();
}

void SkillConfig::initBaseSkills()
{
    reg(Skill(0, "无技能", Tr::None)); // 占位无效技能

    // ========== 基础技能(ID: 1-999 保留给基础技能) ============== 
        // =====================普通攻击======================

    // 攻击类
    reg(Skill(1, "普通攻击", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 100)));
    reg(Skill(2, "群体攻击", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 40)));
    reg(Skill(3, "前排攻击", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 60)));
    reg(Skill(4, "后排攻击", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 60)));
    reg(Skill(5, "随机三人攻击", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 60)));

    // 治疗类
    reg(Skill(10, "单体治疗", Tr::RAGE_SKILL)
        .addEffect(SE::HealByAtk(T::ALLY_HP_LOW1, 100)));
    reg(Skill(11, "多人治疗", Tr::RAGE_SKILL)
        .addEffect(SE::HealByAtk(T::ALLY_HP_LOW3, 60)));
    reg(Skill(12, "群体治疗", Tr::RAGE_SKILL)
        .addEffect(SE::HealByAtk(T::ALLY_ALL, 40)));


    // ==================== 物理伤害技能 (100-199) ====================
    
    reg(Skill(101, "破甲斩", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 180))
        .addEffect(SE::Buff(T::ENEMY_SINGLE, E::BUFF_DEF, -30, 2)));
    
    reg(Skill(102, "旋风斩", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 120)));
    
    reg(Skill(103, "雷霆一击", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 200))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::STUN, 1, 50)));
    
    reg(Skill(104, "穿云箭", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 150)));
    
    reg(Skill(105, "横扫千军", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 140)));
    
    reg(Skill(106, "斩首", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_HP_LOW1, 250)));
    
    reg(Skill(108, "暴风突刺", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 160))
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 20, 2)));
    
    // ==================== 治疗技能 (200-299) ====================
    
    reg(Skill(201, "甘霖普降", Tr::RAGE_SKILL)
        .addEffect(SE::Heal(T::ALLY_ALL, 120)));
    
    reg(Skill(202, "妙手回春", Tr::RAGE_SKILL)
        .addEffect(SE::Heal(T::ALLY_HP_LOW1, 300)));
    
    reg(Skill(203, "战地急救", Tr::RAGE_SKILL)
        .addEffect(SE::Heal(T::ALLY_HP_LOW2, 200)));
    
    reg(Skill(204, "生命之泉", Tr::RAGE_SKILL)
        .addEffect(SE::Heal(T::ALLY_ALL, 80))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 15, 2)));
    
    // ==================== 辅助技能 (300-399) ====================
    
    reg(Skill(301, "战鼓", Tr::RAGE_SKILL)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_ATK, 30, 2)));
    
    reg(Skill(302, "铁壁", Tr::RAGE_SKILL)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 30, 2)));
    
    reg(Skill(303, "疾风", Tr::RAGE_SKILL)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_SPEED, 25, 2)));
    
    reg(Skill(304, "嘲讽", Tr::RAGE_SKILL)
        .addEffect(SE::Control(T::ENEMY_ALL, E::TAUNT, 2, 80)));
    
    reg(Skill(305, "护盾", Tr::RAGE_SKILL)
        .addEffect(SE::Shield(T::ALLY_ALL, 30)));
    
    // ==================== 控制技能 (400-499) ====================
    
    reg(Skill(401, "冰冻术", Tr::RAGE_SKILL)
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::FREEZE, 1, 70))
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 80)));
    
    reg(Skill(402, "沉默", Tr::RAGE_SKILL)
        .addEffect(SE::Control(T::ENEMY_ATK_TOP1, E::SILENCE, 2, 80)));
    
    reg(Skill(403, "群体眩晕", Tr::RAGE_SKILL)
        .addEffect(SE::Control(T::ENEMY_ALL, E::STUN, 1, 40)));
    
    // ==================== DOT技能 (500-599) ====================
    
    reg(Skill(501, "毒刃", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 120))
        .addEffect(SE::Dot(T::ENEMY_SINGLE, E::POISON, 30, 3)));
    
    reg(Skill(502, "烈焰", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 80))
        .addEffect(SE::Dot(T::ENEMY_ALL, E::BURN, 20, 2)));
    
    reg(Skill(503, "裂伤", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 110))
        .addEffect(SE::Dot(T::ENEMY_SINGLE, E::BLEED, 40, 3)));
    
    // ==================== 被动技能 (600-699) ====================
    
    reg(Skill(601, "战意", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 20, 99)));
    
    reg(Skill(602, "铁甲", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_DEF, 25, 99)));
    
    reg(Skill(603, "迅捷", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_SPEED, 30, 99)));
    
    reg(Skill(604, "回合恢复", Tr::ROUND_START)
        .addEffect(SE::Heal(T::SELF, 5)));
    
    reg(Skill(606, "反击", Tr::ON_HIT)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 50)));
    
    reg(Skill(607, "同归于尽", Tr::ON_DEATH)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 100)));
    
    // ==================== 特殊技能 (700-799) ====================
    
    reg(Skill(702, "怒气燃烧", Tr::RAGE_SKILL)
        .addEffect(SE::RageAdd(T::ALLY_ALL, 2)));
    
    reg(Skill(703, "夺气", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 100))
        .addEffect(SE::RageReduce(T::ENEMY_SINGLE, 2)));
}

void SkillConfig::initQunSkills()
{

}

void SkillConfig::initWeiSkills()
{

}

void SkillConfig::initShuSkills()
{

}

void SkillConfig::initWuSkills()
{
    // 徐盛
    reg(Skill(1001, "破军", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 500))
        .addEffect(SE::Control(T::ENEMY_FRONT_ROW, E::INJURY, 3, 40))); // 受伤3回合(40%)
    reg(Skill(1002, "力破万军", Tr::ON_SAME_CAMP)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 500))
        .addEffect(SE::Control(T::ENEMY_FRONT_ROW, E::INJURY, 5, 60))); // 受伤5回合(60%)

    // 周泰
    reg(Skill(1003, "坚不可摧", Tr::RAGE_SKILL)
        .addEffect(SE::Shield(T::SELF, 50, 5)) // 护盾50%，持续5回合
        .addEffect(SE::Buff(T::SELF, E::BUFF_DEF, 30, 5))); // 防御提升30%，持续5回合

    // 太史慈
    reg(Skill(1004, "烈焰箭雨", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 300))
        .addEffect(SE::Dot(T::ENEMY_ALL, E::BURN, 50, 4))); // 灼烧，持续4回合，基于攻击力的50%
}

void SkillConfig::initShenSkills()
{
}

void SkillConfig::initEquipSkills()
{
    // 基础装备技能
    
    initFourEquipSkills(); // 四象装备技能:武器、防具、头盔、鞋子
    initMountAndGeneralSkills(); // 坐骑、名将等技能
    initTreasureSkills(); // 宝物、兵符技能
}

void SkillConfig::initFourEquipSkills()
{
    // 武器技能

    
    // 防具技能

    
    // 头盔技能

    
    // 鞋子技能

}

void SkillConfig::initMountAndGeneralSkills()
{
    // 坐骑技能
    
    // 名将技能
}

void SkillConfig::initTreasureSkills()
{
    // 宝物技能

    
    // 兵符技能

}