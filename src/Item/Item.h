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
    
    std::string getDescription() const; // 获取词缀描述
};

// ==================== 装备 ====================
struct Equipment {
    int id;
    std::string name;
    EquipmentType type;
    EquipmentQuality quality;
    
    // 词缀(主词缀+副词缀-随机生成)
    std::vector<EquipmentAffix> mainAffixs;
    std::vector<EquipmentAffix> subAffixes;
    
    int skillId;  // 装备携带的技能ID列表
    // 套装ID
    int setId;
    
    Equipment() = default;
    
    Equipment(int id_, const std::string& name_, EquipmentType type_, 
              EquipmentQuality quality_, int skill = 0, int setId_ = 0)
        : id(id_), name(name_), type(type_), quality(quality_),
            skillId(skill), setId(setId_) {}

    bool hasSkill() const { return skillId != 0; }

    // 传入角色的基础属性，返回装备加成的属性
    BattleAttr calculateTotalAttr(const BattleAttr& charAttr) const;
    std::string GetDescription() const {return name;} // TODO 完善描述;
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
