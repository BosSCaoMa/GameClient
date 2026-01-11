#pragma once

#include "ItemTypes.h"
#include "BattleTypes.h"
#include "BattleAttr.h"
#include <string>

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
    int level; // 强化等级 todo
    
    // 词缀（随机）
    EquipmentAffix mainAffix;
    std::vector<EquipmentAffix> subAffixes;
    
    std::vector<int> skillIds;  // 装备携带的技能ID列表
    // 套装ID
    int setId;
    
    Equipment() = default;
    
    Equipment(int id_, const std::string& name_, EquipmentType type_, 
              EquipmentQuality quality_, int level_)
        : id(id_), name(name_), type(type_)
        , quality(quality_), level(level_), setId(0) {}

    bool hasSkill() const { return !skillIds.empty(); }

    // 传入角色的基础属性，返回装备加成后的属性
    BattleAttr calculateTotalAttr(const BattleAttr& charAttr) const;
private:
     // 应用单个词缀到属性上
    static void applyAffix(BattleAttr& total, const BattleAttr& charAttr, const EquipmentAffix& affix);
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
