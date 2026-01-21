#pragma once
#include "BattleTypes.h"

// ==================== 战斗属性（使用明确命名，避免魔法索引） ====================
// BattleAttr.h
struct BattleAttr {   // 相互关联 【 AffixType applyAffix 】 【 EffectType applyEffect 】【 BattleAttr 】
    // 基础属性
    int64_t hp = 10000;
    int64_t maxHp = 10000;
    int64_t atk = 1000;
    int64_t def = 100;
    int64_t speed = 300;
    
    // 战斗属性
    int16_t critRate = 0;       // 暴击率 (百分比)
    int16_t critDamage = 150;   // 暴击伤害 (默认150%)
    int16_t critResist = 0;    // 暴击抗性 (百分比)
    int16_t hitRate = 100;      // 命中率 (默认100%)
    int16_t dodgeRate = 0;      // 闪避率
    int16_t rage = 2;        // 基础怒气值

    // ========== 扩展属性（可选） ==========
    int16_t damageBonus = 0;        // 伤害加成 (百分比)
    int16_t damageReduction = 0;    // 伤害减免 (百分比)
    int16_t skillDamage = 0;        // 技能伤害加成 (百分比)
    
    int16_t lifesteal = 0;          // 吸血 (百分比)
    int16_t counterRate = 0;        // 反击率 (百分比)
    int16_t mutiHitRate = 0;       // 连击概率 (百分比)
    int16_t healBonus = 0;          // 治疗加成 (百分比)
    
    // 抗性
    int16_t burnResist = 0;        // 灼烧抗性 (百分比免疫)
    int16_t freezeResist = 0;      // 冰冻抗性 (百分比免疫)
    int16_t stunResist = 0;         // 眩晕抗性 (百分比免疫)
    int16_t silenceResist = 0;      // 沉默抗性 (百分比免疫)
    int16_t poisonResist = 0;       // 中毒抗性 (百分比免疫)
    int16_t tauntResist = 0;        // 嘲讽抗性 (百分比免疫)
    int16_t injuryResist = 0;      // 受伤抗性 (百分比免疫)
    int16_t bleedResist = 0;        // 流血抗性 (百分比免疫)
    int16_t curseResist = 0;        // 诅咒抗性 (百分比免疫)
    
    BattleAttr() = default;
    
    BattleAttr(int64_t hp_, int64_t atk_, int64_t def_, int64_t speed_)
        : hp(hp_), maxHp(hp_), atk(atk_), def(def_), speed(speed_) {}
    
    // 属性相加
    BattleAttr& operator+=(const BattleAttr& other);
    
    void upgradeByLevel(int level); // 根据等级提升属性
    uint64_t calculateCombatPower() const; // 计算战力值

    void InitAttrsByPosition(Position pos); // 根据定位初始化属性
private:
    void ExPandByQuality(QualityType quality); // 根据品质扩展属性
    static BattleAttr warriorAttrs(); // 战士属性模板
    static BattleAttr mageAttrs();   // 法师属性模板
    static BattleAttr tankAttrs();  // 坦克属性模板
    static BattleAttr healerAttrs(); // 辅助属性模板
    static BattleAttr assassinAttrs(); // 刺客属性模板
};

