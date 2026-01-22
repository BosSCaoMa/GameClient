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
        .addEffect(SE::Heal_T_HP(T::ALLY_ALL, 120)));
    
    reg(Skill(202, "妙手回春", Tr::RAGE_SKILL)
        .addEffect(SE::Heal_T_HP(T::ALLY_HP_LOW1, 300)));
    
    reg(Skill(203, "战地急救", Tr::RAGE_SKILL)
        .addEffect(SE::Heal_T_HP(T::ALLY_HP_LOW2, 200)));
    
    reg(Skill(204, "生命之泉", Tr::RAGE_SKILL)
        .addEffect(SE::Heal_T_HP(T::ALLY_ALL, 80))
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
        .addEffect(SE::Heal_T_HP(T::SELF, 5)));
    
    reg(Skill(606, "反击", Tr::ON_HIT)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 50)));
    
    reg(Skill(607, "同归于尽", Tr::ON_DEATH)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 100)));
    
    // ==================== 特殊技能 (700-799) ====================
    
    reg(Skill(702, "怒气燃烧", Tr::RAGE_SKILL)
        .addEffect(SE::RageChange(T::ALLY_ALL, 2)));
    
    reg(Skill(703, "夺气", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 100))
        .addEffect(SE::RageChange(T::ENEMY_SINGLE, -2)));
}

void SkillConfig::initQunSkills()
{
    // 吕布
    reg(Skill(4001, "无双一击", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 300))
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 30, 3))); // 自身攻击提升30%，持续3回合
    reg(Skill(4002, "魔神降世", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 500))
        .addEffect(SE::Buff(T::ENEMY_ALL, E::BUFF_DEF, -30, 3))); // 降低敌方防御30%，持续3回合
    reg(Skill(4003, "狂怒之吼", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 40, 99))
        .addEffect(SE::Buff(T::SELF, E::BUFF_CRIT_RATE, 20, 99))); // 战斗开始时提升自身攻击40%和暴击率20%

    // 左慈
    reg(Skill(4010, "幻术迷踪", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 170))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::SILENCE, 1, 50)));
    reg(Skill(4011, "天地无极", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 260))
        .addEffect(SE::Dot(T::ENEMY_ALL, E::POISON, 35, 3))
        .addEffect(SE::Heal_T_HP(T::ALLY_ALL, 60)));
    reg(Skill(4012, "道法自然", Tr::ROUND_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DODGE_RATE, 15, 2))
        .addEffect(SE::Cleanse(T::ALLY_ALL, 1)));

    // 袁绍
    reg(Skill(4013, "四世荣光", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 200))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_ATK, 10, 2)));
    reg(Skill(4014, "河北雄风", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 300))
        .addEffect(SE::Buff(T::ALLY_BACK_ROW, E::BUFF_CRIT_RATE, 20, 2)));
    reg(Skill(4015, "号令天下", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 20, 99))
        .addEffect(SE::RageChange(T::ALLY_ALL, 1)));

    // 贾诩
    reg(Skill(4016, "乱世毒谋", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 170))
        .addEffect(SE::Dot(T::ENEMY_SINGLE, E::POISON, 40, 3)));
    reg(Skill(4017, "离间连环", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 260))
        .addEffect(SE::Buff(T::ENEMY_ALL, E::BUFF_ATK, -20, 2)));
    reg(Skill(4018, "暗算无形", Tr::ROUND_START)
        .addEffect(SE::Cleanse(T::SELF, 1))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DODGE_RATE, 15, 2)));

    // 华雄
    reg(Skill(4019, "血饮狂斧", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 200))
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 15, 2)));
    reg(Skill(4020, "斩将夺旗", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 380))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::STUN, 1, 40)));
    reg(Skill(4021, "虎贲横冲", Tr::ON_HIT)
        .addEffect(SE::RageChange(T::SELF, 1))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DEF, 15, 2)));

    // 董卓
    reg(Skill(4022, "暴虐挥刀", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 200))
        .addEffect(SE::Heal_T_HP(T::SELF, 60)));
    reg(Skill(4023, "洛阳焚城", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 320))
        .addEffect(SE::Dot(T::ENEMY_ALL, E::BURN, 40, 3)));
    reg(Skill(4024, "乱世枭雄", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 15, 2))
        .addEffect(SE::RageChange(T::SELF, 1)));

    // 貂蝉
    reg(Skill(4025, "霓裳魅舞", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 150))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::SILENCE, 1, 40)));
    reg(Skill(4026, "闭月羞花", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 240))
        .addEffect(SE::RageChange(T::ENEMY_ALL, -1)));
    reg(Skill(4027, "倾城之姿", Tr::ROUND_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DODGE_RATE, 15, 2))
        .addEffect(SE::Cleanse(T::ALLY_ALL, 1)));

    // 公孙瓒
    reg(Skill(4028, "白马义从", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 180))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_SPEED, 10, 2)));
    reg(Skill(4029, "银枪裂阵", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 320))
        .addEffect(SE::Control(T::ENEMY_RANDOM_3, E::STUN, 1, 35)));
    reg(Skill(4030, "北疆铁骑", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 15, 2))
        .addEffect(SE::Shield(T::ALLY_FRONT_ROW, 20, 2)));

    // 颜良
    reg(Skill(4031, "双戟怒袭", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 200))
        .addEffect(SE::Buff(T::SELF, E::BUFF_CRIT_RATE, 15, 2)));
    reg(Skill(4032, "横扫冀州", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 290))
        .addEffect(SE::Buff(T::ENEMY_ALL, E::BUFF_DEF, -20, 2)));
    reg(Skill(4033, "河北强弓", Tr::ROUND_START)
        .addEffect(SE::RageChange(T::ALLY_ALL, 1))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_ATK, 10, 2)));

    // 文丑
    reg(Skill(4034, "烈马冲锋", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 180))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DODGE_RATE, 10, 2)));
    reg(Skill(4035, "泰山压顶", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 370))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::TAUNT, 1, 45)));
    reg(Skill(4036, "骁勇无畏", Tr::ON_HIT)
        .addEffect(SE::Shield(T::SELF, 25, 2))
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 15, 2)));
}

void SkillConfig::initWeiSkills()
{
    // 荀彧
    reg(Skill(3001, "神算无双", Tr::NORMAL_ATTACK)
        .addEffect(SE::HealByAtk(T::ALLY_HP_LOW1, 100))
        .addEffect(SE::Buff(T::ALLY_HP_LOW1, E::BUFF_HIT_RATE, 20, 2)));
    reg(Skill(3002, "帷幄之策", Tr::RAGE_SKILL)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 25, 3))
        .addEffect(SE::RageChange(T::ALLY_ALL, 1)));
    reg(Skill(3003, "王佐之才", Tr::ROUND_START)
        .addEffect(SE::Heal_T_HP(T::ALLY_ALL, 50)));

    // 司马懿
    reg(Skill(3004, "鬼才雷袭", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 160))
        .addEffect(SE::Control(T::ENEMY_RANDOM_3, E::SILENCE, 1, 40)));
    reg(Skill(3005, "风魔浩劫", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 340))
        .addEffect(SE::Dot(T::ENEMY_ALL, E::POISON, 40, 3)));
    reg(Skill(3006, "破局之谋", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_CRIT_RATE, 20, 99))
        .addEffect(SE::Buff(T::SELF, E::BUFF_SPEED, 15, 99)));

    // 曹操
    reg(Skill(3007, "魏武突袭", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 200))
        .addEffect(SE::RageChange(T::SELF, 1)));
    reg(Skill(3008, "虎狼铁骑", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 300))
        .addEffect(SE::Buff(T::ENEMY_ALL, E::BUFF_ATK, -20, 2)));
    reg(Skill(3009, "挟天子令诸侯", Tr::BATTLE_START)
        .addEffect(SE::RageChange(T::ALLY_ALL, 1))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_ATK, 15, 2)));

    // 张辽
    reg(Skill(3010, "突袭合肥", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 190))
        .addEffect(SE::Control(T::ENEMY_BACK_ROW, E::SILENCE, 1, 40)));
    reg(Skill(3011, "夜袭乌桓", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 300))
        .addEffect(SE::Buff(T::ENEMY_ALL, E::BUFF_DEF, -15, 2)));
    reg(Skill(3012, "威震逍遥", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 20, 2))
        .addEffect(SE::RageChange(T::SELF, 1)));

    // 许褚
    reg(Skill(3013, "虎痴重击", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 200))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DEF, 20, 2)));
    reg(Skill(3014, "披甲怒吼", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 300))
        .addEffect(SE::Control(T::ENEMY_FRONT_ROW, E::TAUNT, 1, 55)));
    reg(Skill(3015, "钢躯守护", Tr::ON_HIT)
        .addEffect(SE::Shield(T::SELF, 25, 2))
        .addEffect(SE::RageChange(T::SELF, 1)));

    // 夏侯惇
    reg(Skill(3016, "独眼斩", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 190))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::STUN, 1, 35)));
    reg(Skill(3017, "残光裂斧", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 320))
        .addEffect(SE::Buff(T::SELF, E::BUFF_CRIT_RATE, 15, 2)));
    reg(Skill(3018, "护主之魄", Tr::ROUND_START)
        .addEffect(SE::Shield(T::ALLY_HP_LOW1, 20, 2))
        .addEffect(SE::Buff(T::ALLY_HP_LOW1, E::BUFF_DEF, 15, 2)));

    // 夏侯渊
    reg(Skill(3019, "疾风箭雨", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 180))
        .addEffect(SE::Buff(T::SELF, E::BUFF_SPEED, 15, 2)));
    reg(Skill(3020, "迅雷破阵", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 400))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::STUN, 1, 40)));
    reg(Skill(3021, "疾掠突行", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_SPEED, 20, 2))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DODGE_RATE, 15, 2)));

    // 郭嘉
    reg(Skill(3022, "鬼谋策算", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 170))
        .addEffect(SE::Buff(T::ENEMY_SINGLE, E::BUFF_DEF, -15, 2)));
    reg(Skill(3023, "天妒英才", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 260))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_CRIT_RATE, 10, 2)));
    reg(Skill(3024, "料敌于先", Tr::ROUND_START)
        .addEffect(SE::Cleanse(T::ALLY_ALL, 1))
        .addEffect(SE::RageChange(T::ALLY_ALL, 1)));

    // 张郃
    reg(Skill(3025, "奇正游击", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 170))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DODGE_RATE, 15, 2)));
    reg(Skill(3026, "百战合围", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 320))
        .addEffect(SE::Control(T::ENEMY_FRONT_ROW, E::TAUNT, 1, 40)));
    reg(Skill(3027, "机动严整", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_SPEED, 10, 2))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 10, 2)));

    // 于禁
    reg(Skill(3028, "整肃护阵", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 160))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 10, 2)));
    reg(Skill(3029, "铁锁横江", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 250))
        .addEffect(SE::Control(T::ENEMY_ALL, E::SILENCE, 1, 35)));
    reg(Skill(3030, "慎战守律", Tr::ROUND_START)
        .addEffect(SE::Shield(T::ALLY_ALL, 15, 2))
        .addEffect(SE::RageChange(T::ALLY_ALL, 1)));

}

void SkillConfig::initShuSkills()
{
    // 关羽
    reg(Skill(2001, "青龙腾袭", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 180)));
    reg(Skill(2002, "武圣破阵", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 330))
        .addEffect(SE::Control(T::ENEMY_FRONT_ROW, E::STUN, 1, 40)));
    reg(Skill(2003, "忠义守护", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 25, 99))
        .addEffect(SE::Buff(T::SELF, E::BUFF_CRIT_RATE, 15, 99)));

    // 张飞
    reg(Skill(2004, "怒吼震军", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 150))
        .addEffect(SE::Control(T::ENEMY_FRONT_ROW, E::TAUNT, 1, 60)));
    reg(Skill(2005, "丈八横扫", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 280))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 20, 2)));
    reg(Skill(2006, "酒胆神威", Tr::ON_HIT)
        .addEffect(SE::RageChange(T::SELF, 1))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DEF, 15, 2)));

    // 赵云
    reg(Skill(2007, "龙胆枪舞", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 150)));
    reg(Skill(2008, "七进七出", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 420))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DODGE_RATE, 30, 2)));
    reg(Skill(2009, "护主之志", Tr::ROUND_START)
        .addEffect(SE::Shield(T::ALLY_HP_LOW1, 25, 2))
        .addEffect(SE::Buff(T::ALLY_HP_LOW1, E::BUFF_SPEED, 15, 2)));

    // 诸葛亮
    reg(Skill(2010, "天雷术", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 170))
        .addEffect(SE::Control(T::ENEMY_BACK_ROW, E::SILENCE, 1, 40)));
    reg(Skill(2011, "八阵图", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 260))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 20, 2))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_SPEED, 15, 2)));
    reg(Skill(2012, "神机妙算", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_HIT_RATE, 15, 99))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_CRIT_RATE, 10, 99)));

    // 刘备
    reg(Skill(2013, "仁德普济", Tr::NORMAL_ATTACK)
        .addEffect(SE::HealByAtk(T::ALLY_HP_LOW1, 90))
        .addEffect(SE::Buff(T::ALLY_HP_LOW1, E::BUFF_DEF, 10, 2)));
    reg(Skill(2014, "桃园结义", Tr::RAGE_SKILL)
        .addEffect(SE::Heal_T_HP(T::ALLY_ALL, 200))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_ATK, 15, 2)));
    reg(Skill(2015, "德昭天下", Tr::ROUND_START)
        .addEffect(SE::RageChange(T::ALLY_ALL, 1))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DODGE_RATE, 10, 2)));

    // 黄忠
    reg(Skill(2016, "百步穿杨", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 190))
        .addEffect(SE::Buff(T::SELF, E::BUFF_CRIT_RATE, 10, 2)));
    reg(Skill(2017, "连珠箭阵", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 280))
        .addEffect(SE::Dot(T::ENEMY_ALL, E::BURN, 25, 2)));
    reg(Skill(2018, "箭意不减", Tr::ROUND_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 15, 2))
        .addEffect(SE::Buff(T::SELF, E::BUFF_HIT_RATE, 10, 2)));

    // 马超
    reg(Skill(2019, "西凉突袭", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 200))
        .addEffect(SE::Buff(T::SELF, E::BUFF_SPEED, 15, 2)));
    reg(Skill(2020, "破阵骁骑", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 320))
        .addEffect(SE::Control(T::ENEMY_RANDOM_3, E::INJURY, 2, 45)));
    reg(Skill(2021, "马踏飞燕", Tr::ON_HIT)
        .addEffect(SE::RageChange(T::SELF, 1))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DODGE_RATE, 15, 2)));

    // 庞统
    reg(Skill(2022, "鸩羽暗袭", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 170))
        .addEffect(SE::Dot(T::ENEMY_SINGLE, E::POISON, 40, 2)));
    reg(Skill(2023, "连环奇谋", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 240))
        .addEffect(SE::Control(T::ENEMY_ALL, E::SILENCE, 1, 50)));
    reg(Skill(2024, "凤雏智谋", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_ATK, 15, 2))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_SPEED, 10, 2)));

    // 姜维
    reg(Skill(2025, "破敌枪阵", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 190))
        .addEffect(SE::Buff(T::SELF, E::BUFF_SPEED, 10, 2)));
    reg(Skill(2026, "北伐雷霆", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 300))
        .addEffect(SE::Control(T::ENEMY_ALL, E::STUN, 1, 35)));
    reg(Skill(2027, "继志图强", Tr::ROUND_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_HIT_RATE, 10, 2))
        .addEffect(SE::RageChange(T::ALLY_ALL, 1)));

    // 魏延
    reg(Skill(2028, "狂骨斩", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 200))
        .addEffect(SE::Control(T::ENEMY_FRONT_ROW, E::TAUNT, 1, 50)));
    reg(Skill(2029, "逆鳞狂袭", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 380))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DEF, 20, 2)));
    reg(Skill(2030, "悍勇不屈", Tr::ON_HIT)
        .addEffect(SE::Shield(T::SELF, 20, 2))
        .addEffect(SE::RageChange(T::SELF, 1)));

    // 张苞
    reg(Skill(2031, "虎痴猛击", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 180))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DEF, 15, 2)));
    reg(Skill(2032, "破阵冲锋", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 260))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_SPEED, 10, 2)));
    reg(Skill(2033, "父勇子烈", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_ATK, 10, 2))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 10, 2)));

    // 关银屏
    reg(Skill(2034, "双刃连舞", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 160))
        .addEffect(SE::Buff(T::SELF, E::BUFF_CRIT_RATE, 10, 2)));
    reg(Skill(2035, "神威裂空", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 320))
        .addEffect(SE::Buff(T::ENEMY_BACK_ROW, E::BUFF_DEF, -25, 2)));
    reg(Skill(2036, "家风传承", Tr::ROUND_START)
        .addEffect(SE::Heal_T_HP(T::ALLY_HP_LOW1, 120))
        .addEffect(SE::Buff(T::ALLY_HP_LOW1, E::BUFF_DEF, 15, 2)));

}

void SkillConfig::initWuSkills()
{
    // 周瑜
    reg(Skill(1101, "火舞连环", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 160))
        .addEffect(SE::Dot(T::ENEMY_BACK_ROW, E::BURN, 30, 2)));
    reg(Skill(1102, "赤壁焚天", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 320))
        .addEffect(SE::Dot(T::ENEMY_ALL, E::BURN, 50, 3)));
    reg(Skill(1103, "谋定乾坤", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_CRIT_RATE, 15, 99))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_HIT_RATE, 10, 99)));

    // 孙策
    reg(Skill(1104, "霸王突袭", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 180))
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 20, 2)));
    reg(Skill(1105, "江东虎啸", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 350))
        .addEffect(SE::Control(T::ENEMY_RANDOM_3, E::STUN, 1, 45)));
    reg(Skill(1106, "无畏之魄", Tr::ON_HIT)
        .addEffect(SE::RageChange(T::SELF, 1))
        .addEffect(SE::Shield(T::SELF, 20, 1)));

    // 孙权
    reg(Skill(1107, "制衡令", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 150))
        .addEffect(SE::RageChange(T::ALLY_ALL, 1)));
    reg(Skill(1108, "江东庇护", Tr::RAGE_SKILL)
        .addEffect(SE::Heal_T_HP(T::ALLY_ALL, 150))
        .addEffect(SE::Shield(T::ALLY_ALL, 25, 2)));
    reg(Skill(1109, "统御之道", Tr::ROUND_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 20, 2))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_ATK, 10, 2)));

    // 甘宁
    reg(Skill(1110, "劫营急袭", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 170))
        .addEffect(SE::RageChange(T::SELF, 1)));
    reg(Skill(1111, "破浪斩", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 380))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::STUN, 1, 40)));
    reg(Skill(1112, "虎鲨之魂", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::SELF, E::BUFF_CRIT_RATE, 20, 2))
        .addEffect(SE::Buff(T::SELF, E::BUFF_DODGE_RATE, 15, 2)));

    // 吕蒙
    reg(Skill(1113, "白衣渡江", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 160))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::SILENCE, 1, 45)));
    reg(Skill(1114, "缚敌擒王", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 280))
        .addEffect(SE::Buff(T::ENEMY_RANDOM_3, E::BUFF_SPEED, -20, 2)));
    reg(Skill(1115, "智取荆州", Tr::ROUND_START)
        .addEffect(SE::Cleanse(T::ALLY_ALL, 1))
        .addEffect(SE::RageChange(T::ALLY_ALL, 1)));

    // 陆逊
    reg(Skill(1116, "火攻箭雨", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_FRONT_ROW, 170))
        .addEffect(SE::Dot(T::ENEMY_FRONT_ROW, E::BURN, 30, 2)));
    reg(Skill(1117, "焚野烈焰", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 300))
        .addEffect(SE::Dot(T::ENEMY_ALL, E::BURN, 40, 3)));
    reg(Skill(1118, "沉着布阵", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DEF, 15, 2))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_SPEED, 10, 2)));

    // 凌统
    reg(Skill(1119, "奔狼快斩", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 190))
        .addEffect(SE::Buff(T::SELF, E::BUFF_SPEED, 15, 2)));
    reg(Skill(1120, "惊涛突击", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_SINGLE, 360))
        .addEffect(SE::Control(T::ENEMY_SINGLE, E::INJURY, 2, 45)));
    reg(Skill(1121, "雪恨之志", Tr::ON_HIT)
        .addEffect(SE::RageChange(T::SELF, 1))
        .addEffect(SE::Buff(T::SELF, E::BUFF_ATK, 15, 2)));

    // 丁奉
    reg(Skill(1122, "飞矢强袭", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_BACK_ROW, 160))
        .addEffect(SE::Control(T::ENEMY_BACK_ROW, E::SILENCE, 1, 35)));
    reg(Skill(1123, "石亭寒风", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 280))
        .addEffect(SE::Control(T::ENEMY_ALL, E::FREEZE, 1, 35)));
    reg(Skill(1124, "寒箭待发", Tr::ROUND_START)
        .addEffect(SE::Shield(T::ALLY_BACK_ROW, 20, 2))
        .addEffect(SE::Buff(T::ALLY_BACK_ROW, E::BUFF_DEF, 15, 2)));

    // 大乔
    reg(Skill(1125, "凤舞鼓音", Tr::NORMAL_ATTACK)
        .addEffect(SE::HealByAtk(T::ALLY_HP_LOW1, 110))
        .addEffect(SE::Buff(T::ALLY_HP_LOW1, E::BUFF_DEF, 10, 2)));
    reg(Skill(1126, "安国之舞", Tr::RAGE_SKILL)
        .addEffect(SE::Heal_T_HP(T::ALLY_ALL, 180))
        .addEffect(SE::Cleanse(T::ALLY_ALL, 1)));
    reg(Skill(1127, "倾城柔情", Tr::BATTLE_START)
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_DODGE_RATE, 15, 2))
        .addEffect(SE::RageChange(T::ALLY_ALL, 1)));

    // 孙尚香
    reg(Skill(1128, "流火箭舞", Tr::NORMAL_ATTACK)
        .addEffect(SE::Damage_PA(T::ENEMY_RANDOM_3, 160))
        .addEffect(SE::Buff(T::SELF, E::BUFF_CRIT_RATE, 10, 2)));
    reg(Skill(1129, "旋弓百矢", Tr::RAGE_SKILL)
        .addEffect(SE::Damage_PA(T::ENEMY_ALL, 300))
        .addEffect(SE::Dot(T::ENEMY_ALL, E::BURN, 35, 2)));
    reg(Skill(1130, "江东英姿", Tr::ROUND_START)
        .addEffect(SE::Buff(T::ALLY_FRONT_ROW, E::BUFF_ATK, 15, 2))
        .addEffect(SE::Buff(T::ALLY_ALL, E::BUFF_HIT_RATE, 10, 2)));
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