#pragma once
#include "Character.h"
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>
class BattleCharacter {
public:
    Character* original;        // 原始角色
    int battleId;               // 战斗中ID（正数己方，负数敌方），0-2前排，3-5后排
    bool isInFrontRow() const {
        return std::abs(battleId) <= 2;
    }

    std::string name;

    BattleAttr baseAttr;        // 战斗开始快照
    BattleAttr currentAttr;     // 当前属性
    
    std::unordered_map<SkillTrigger, Skill> skills;
    std::vector<Buff> buffs;

    
    bool isAlive = true;     // 是否存活
    bool hasActed = false;     // 本回合是否已行动
    int64_t shieldValue = 0;    // 护盾值
    
public:
    BattleCharacter(Character* ch, int battleId);

    // ==================== Buff管理 ====================
    void addBuff(EffectType type, int64_t value, int duration, int sourceId = 0);
    
    void tickBuffs();
    
    void recalculateAttr(); // EffectType changedBuff = EffectType::ALL
    
    // ==================== 状态检查 ====================
    bool isControlled() const;
    
    bool isSilenced() const {
        for (const Buff& b : buffs) {
            if (b.type == EffectType::SILENCE) return true;
        }
        return false;
    }
    
    // ==================== 战斗操作 ====================
    void takeDamage(int64_t damage, bool canBeShielded = true);
    
    void heal(int64_t amount);
    
    void addRage(int amount);
    
    void addShield(int64_t amount);

    int dispelBuffs(int count);

    int cleanseDebuffs(int count);

    int transferDebuffsTo(BattleCharacter* target, int count);

    bool BuffIsOffset(EffectType type) const;
    
    // ==================== 技能相关 ====================
    Skill* getSkill(SkillTrigger trigger);
    
    Skill* getNormalAttack();
    
    Skill* getRageSkill();

private:
    bool hasBuffOfType(EffectType type) const;

    bool hasInjury() const;

    bool hasLockBleed() const;

    bool hasControlImmunity() const;

    bool isInvincible() const;

    int removeBuffsInternal(bool removeDebuff, int count);

    int removeBuffsByType(EffectType type, int count = 1);
};
