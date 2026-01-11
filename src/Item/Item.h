#pragma once

#include "ItemTypes.h"
#include "BattleTypes.h"
#include "BattleAttr.h"
#include <string>

// ==================== 基础物品 ====================
struct Item {
    int id;
    std::string name;
    ItemType type;
    int quality;            // 品质 1-6
    std::string description;
    int maxStack;           // 最大堆叠数
    
    Item() : id(0), type(ItemType::NONE), quality(1), maxStack(1) {}
    
    Item(int id_, const std::string& name_, ItemType type_, 
         int quality_ = 1, int maxStack_ = 1)
        : id(id_), name(name_), type(type_)
        , quality(quality_), maxStack(maxStack_) {}
};

// ==================== 装备词缀 ====================
struct EquipmentAffix {
    AffixType type;
    int64_t value;
    
    EquipmentAffix() : type(AffixType::NONE), value(0) {}
    EquipmentAffix(AffixType t, int64_t v) : type(t), value(v) {}
    
    // 获取词缀描述
    std::string getDescription() const;
};

// ==================== 装备 ====================
struct Equipment {
    int id;
    std::string name;
    EquipmentType type;
    EquipmentQuality quality;
    int level;
    
    // 基础属性（固定） 百分比词缀统一基于角色基础属性计算，避免复杂的叠加逻辑。
    BattleAttr baseAttr;
    
    // 词缀（随机）
    EquipmentAffix mainAffix;
    std::vector<EquipmentAffix> subAffixes;
    
    // 套装ID
    int setId;
    
    Equipment() 
        : id(0), type(EquipmentType::WEAPON)
        , quality(EquipmentQuality::WHITE)
        , level(1), setId(0) {}
    
    Equipment(int id_, const std::string& name_, EquipmentType type_, 
              EquipmentQuality quality_, int level_)
        : id(id_), name(name_), type(type_)
        , quality(quality_), level(level_), setId(0) {}
    
    // ==================== 计算总属性 ====================
    // baseAttr: 用于百分比计算的基础值（通常是武将的基础属性）
    BattleAttr calculateTotalAttr(const BattleAttr& baseAttr = BattleAttr()) const;

     // 应用单个词缀到属性上
    static void applyAffix(BattleAttr& attr, const EquipmentAffix& affix, 
        const BattleAttr& baseAttr);
};

// ==================== 套装效果 ====================
struct SetBonus {
    int setId;
    std::string name;
    
    struct Bonus {
        int pieceCount;     // 需要件数
        std::string desc;
        std::vector<EquipmentAffix> affixes;
    };
    
    std::vector<Bonus> bonuses;  // 2件套、4件套、6件套等
};
