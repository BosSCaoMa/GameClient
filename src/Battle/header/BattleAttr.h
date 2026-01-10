#pragma once
#include "BattleTypes.h"

// ==================== 战斗属性（使用明确命名，避免魔法索引） ====================
struct BattleAttr {
    // 基础属性（long long防止大数溢出）
    int64_t hp;         // 当前生命
    int64_t maxHp;      // 最大生命
    int64_t atk;        // 攻击力
    int64_t defence;        // 防御力
    
    // 战斗属性（int足够，用百分制或千分制）
    int speed;          // 速度
    int critRate;       // 暴击率 (5 = 5%)
    int critDamage;     // 暴击伤害 (150 = 150%)
    int critResist;     // 抗暴率
    int dodgeRate;      // 闪避率
    int hitRate;        // 命中率（反闪避）
    int rage;           // 怒气值
    int maxRage;        // 最大怒气
    
    // 伤害增减（可选，按需添加）
    int damageBonus;    // 伤害加成 %
    int damageReduce;   // 伤害减免 %
    
    // 默认构造
    BattleAttr() 
        : hp(10000), maxHp(10000), atk(1000), defence(50),
          speed(100), critRate(5), critDamage(150), critResist(5),
          dodgeRate(5), hitRate(0), rage(0), maxRage(100),
          damageBonus(0), damageReduce(0) {}
    
    // 便捷构造
    BattleAttr(int64_t maxHp, int64_t atk, int64_t defence, int speed)
        : hp(maxHp), maxHp(maxHp), atk(atk), defence(defence),
          speed(speed), critRate(5), critDamage(150), critResist(5),
          dodgeRate(5), hitRate(0), rage(0), maxRage(100),
          damageBonus(0), damageReduce(0) {}
          
    // 属性加成（用于装备/buff叠加）
    BattleAttr& operator+=(const BattleAttr& other) {
        maxHp += other.maxHp;
        atk += other.atk;
        defence += other.defence;
        speed += other.speed;
        critRate += other.critRate;
        critDamage += other.critDamage;
        critResist += other.critResist;
        dodgeRate += other.dodgeRate;
        hitRate += other.hitRate;
        rage += other.rage;
        maxRage += other.maxRage;
        damageBonus += other.damageBonus;
        damageReduce += other.damageReduce;

        return *this;
    }
};
