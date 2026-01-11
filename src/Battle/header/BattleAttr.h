#pragma once
#include "BattleTypes.h"

// ==================== 战斗属性（使用明确命名，避免魔法索引） ====================
// BattleAttr.h
struct BattleAttr {
    // 基础属性
    int64_t hp = 0;
    int64_t maxHp = 0;
    int64_t atk = 0;
    int64_t def = 0;
    int64_t speed = 0;
    
    // 战斗属性
    int64_t critRate = 0;       // 暴击率 (百分比)
    int64_t critDamage = 150;   // 暴击伤害 (默认150%)
    int64_t critResist = 0;    // 暴击抗性 (百分比)
    int64_t hitRate = 100;      // 命中率 (默认100%)
    int64_t dodgeRate = 0;      // 闪避率
    int64_t hitRate;        // 命中率（反闪避）
    int64_t rage;        // 基础怒气值

    // ========== 扩展属性（可选） ==========
    int64_t damageBonus = 0;        // 伤害加成 (百分比)
    int64_t damageReduction = 0;    // 伤害减免 (百分比)
    int64_t skillDamage = 0;        // 技能伤害加成 (百分比)
    
    int64_t lifesteal = 0;          // 吸血 (百分比)
    int64_t counterRate = 0;        // 反击率 (百分比)
    int64_t rageGain = 100;         // 怒气获取 (百分比，默认100%)
    int64_t healBonus = 0;          // 治疗加成 (百分比)
    
    // 抗性
    int64_t stunResist = 0;         // 眩晕抗性 (百分比)
    int64_t silenceResist = 0;      // 沉默抗性
    int64_t poisonResist = 0;       // 中毒抗性
    
    BattleAttr() = default;
    
    BattleAttr(int64_t hp_, int64_t atk_, int64_t def_, int64_t speed_)
        : hp(hp_), maxHp(hp_), atk(atk_), def(def_), speed(speed_) {}
    
    // 属性相加
    BattleAttr& operator+=(const BattleAttr& other) {
        hp += other.hp;
        maxHp += other.maxHp;
        atk += other.atk;
        def += other.def;
        speed += other.speed;
        critRate += other.critRate;
        critResist += other.critResist;
        critDamage += other.critDamage;
        hitRate += other.hitRate;
        dodgeRate += other.dodgeRate;
        rage += other.rage;
        
        damageBonus += other.damageBonus;
        damageReduction += other.damageReduction;
        skillDamage += other.skillDamage;
        lifesteal += other.lifesteal;
        counterRate += other.counterRate;
        rageGain += other.rageGain;
        healBonus += other.healBonus;
        
        stunResist += other.stunResist;
        silenceResist += other.silenceResist;
        poisonResist += other.poisonResist;
        
        return *this;
    }
};

