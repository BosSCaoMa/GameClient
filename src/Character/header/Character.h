#pragma once

#include "BattleTypes.h"
#include "BattleAttr.h"
#include "Skill.h"
#include "Item.h"
#include <string>
#include <unordered_map>
#include <vector>

class Character {
public:
    // ==================== 基础信息 ====================
    int id;
    int relid; // 关联角色id，用于触发合击技能
    std::string name;
    int level;
    int star;           // 星级 1-5
    int quality;        // 品质 1-5
    
    // ==================== 属性 ====================
    BattleAttr originAttr;        // 基础属性（等级/星级/突破成长）
    BattleAttr baseAttr;     // 当前属性（基础 + 装备 + Buff）
    
    // ==================== 技能 ====================
    std::unordered_map<SkillTrigger, Skill> skills;
    
    // ==================== 装备 ====================
    std::unordered_map<EquipmentType, Equipment> equipments;
    
    // ==================== 经验/进阶 ====================
    int64_t exp;            // 当前经验
    int64_t expMax;         // 升级所需经验
    int breakthrough;       // 突破等级（如 0, 1, 2...）
    
    // ==================== 构造函数 ====================
    Character();
    Character(int id_, const std::string& name_, int level_ = 1, int star_ = 1);
    
    // ==================== 技能管理 ====================
    void setSkill(SkillTrigger trigger, const Skill& skill);
    const Skill* getSkill(SkillTrigger trigger) const;
    bool hasSkill(SkillTrigger trigger) const;
    
    // ==================== 装备管理 ====================
    void equipItem(const Equipment& equip);
    void unequipItem(EquipmentType type);
    const Equipment* getEquipment(EquipmentType type) const;
    bool hasEquipment(EquipmentType type) const;
    
    // ==================== 属性计算 ====================
    void recalculateAttr();
    uint64_t calculateCombatPower() const;
    BattleAttr getTotalAttr() {
        return baseAttr;
    }
    // ==================== 升级/进阶 ====================
    bool addExp(int64_t amount);        // 返回是否升级
    bool canLevelUp() const;
    void levelUp();
    bool canBreakthrough() const;
    void DoBreakthrough();
    bool canUpgradeStar() const;
    void upgradeStar();
    
    // ==================== 序列化（可选） ====================
    // 用于存档/网络传输
    // std::string serialize() const;
    // static Character deserialize(const std::string& data);

private:
    void applySetBonuses();
    void initExpMax();
};
