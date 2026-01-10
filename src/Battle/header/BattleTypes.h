#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_set>
// ==================== 目标类型 ====================
enum class TargetType {
    // 基础阵营类型（保留原有核心类型，统一命名风格）
    SELF = 0,               // 自己
    ALLY_SINGLE = 1,        // 己方单个目标（替代原FRIEND，语义更精准）
    ALLY_ALL = 2,           // 己方全体（替代原ALLY）
    ENEMY_SINGLE = 3,       // 敌方单个目标（替代原ENEMY）
    ENEMY_ALL = 4,          // 敌方全体（保留原ALL_ENEMY，统一命名）

    // 按站位区分（前/后排）
    ALLY_FRONT_ROW = 5,     // 己方前排
    ALLY_BACK_ROW = 6,      // 己方后排
    ENEMY_FRONT_ROW = 7,    // 敌方前排
    ENEMY_BACK_ROW = 8,     // 敌方后排

    // 按攻击属性排序（攻击最高1/2/3人）
    ALLY_ATK_TOP1 = 9,      // 己方攻击最高1人
    ALLY_ATK_TOP2 = 10,     // 己方攻击最高2人
    ALLY_ATK_TOP3 = 11,     // 己方攻击最高3人
    ENEMY_ATK_TOP1 = 12,    // 敌方攻击最高1人
    ENEMY_ATK_TOP2 = 13,    // 敌方攻击最高2人
    ENEMY_ATK_TOP3 = 14,    // 敌方攻击最高3人

    // 按血量属性排序（血量最低1/2/3人）
    ALLY_HP_LOW1 = 15,      // 己方血量最低1人
    ALLY_HP_LOW2 = 16,      // 己方血量最低2人
    ALLY_HP_LOW3 = 17,      // 己方血量最低3人
    ENEMY_HP_LOW1 = 18,     // 敌方血量最低1人
    ENEMY_HP_LOW2 = 19,     // 敌方血量最低2人
    ENEMY_HP_LOW3 = 20,     // 敌方血量最低3人

    // 随机
    ALLY_RANDOM_1 = 21,   // 己方随机1人
    ALLY_RANDOM_2 = 22,   // 己方随机2人
    ALLY_RANDOM_3 = 23,   // 己方随机3人
    ENEMY_RANDOM_1 = 24,  // 敌方随机1人
    ENEMY_RANDOM_2 = 25,  // 敌方随机2人
    ENEMY_RANDOM_3 = 26   // 敌方随机3人
};

// ==================== 数值计算类型 ====================
enum class ValueType {
    FIXED,                  // 固定值(<100为目标百分比)
    PERCENT_ATK,            // 攻击力百分比 
    PERCENT_DEF,            // 防御力百分比
    PERCENT_MAX_HP,         // 最大生命百分比
    PERCENT_CURRENT_HP,     // 当前生命百分比
    PERCENT_LOST_HP,        // 已损失生命百分比
};

// ==================== 效果类型 ====================
enum class EffectType {
    // 即时效果
    DAMAGE,             // 造成伤害
    HEAL,               // 治疗
    RAGE_ADD,           // 增加怒气
    RAGE_REDUCE,        // 减少怒气
    
    // 持续Buff效果
    BUFF_ATK,           // 攻击力变化
    BUFF_DEF,           // 防御力变化
    BUFF_SPEED,         // 速度变化
    BUFF_CRIT_RATE,     // 暴击率变化
    BUFF_CRIT_RESIST,   // 抗暴率变化
    BUFF_HIT_RATE,      // 命中率变化
    BUFF_DODGE_RATE,    // 闪避率变化

    // 控制效果
    STUN,               // 眩晕
    SILENCE,            // 沉默
    FREEZE,             // 冰冻
    
    // 持续伤害
    POISON,             // 中毒
    BURN,               // 灼烧
    BLEED,              // 流血
    
    // 特殊效果
    SHIELD,             // 护盾
    IMMUNITY,           // 免疫
    TAUNT,              // 嘲讽
    REVIVE,             // 复活
    INJURY,              // 重伤（限制回血）

    ALL
};

const std::unordered_set<EffectType> debuffTypes = {
    EffectType::STUN,
    EffectType::SILENCE,
    EffectType::FREEZE,
    EffectType::POISON,
    EffectType::BURN,
    EffectType::BLEED,
    EffectType::INJURY,
    EffectType::TAUNT
};

bool IsDebuff(EffectType type)
{
    return debuffTypes.find(type) != debuffTypes.end();
}

// ==================== 技能触发时机 ====================
enum class SkillTrigger {
    None = 999,   // 默认无触发
    NORMAL_ATTACK = 0,  // 普通攻击
    RAGE_SKILL = 1,     // 怒气技能
    BATTLE_START = 2,   // 开局技能
    ON_HIT = 3,         // 受击技能
    ON_LOW_HP = 4,      // 名刀/濒死技能
    ON_DEATH = 5,       // 阵亡技能
    ON_ALLY_DEATH = 6,  // 队友阵亡
    ROUND_START = 7,    // 回合开始
    ROUND_END = 8       // 回合结束
};

enum class PlayerAttrType {
    STAMINA = 1, // 体力
    ENERGY = 2, // 精力
    EXP = 3    // 经验
};