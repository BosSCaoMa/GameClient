#pragma once

#include "Item.h"
#include <unordered_map>
#include <vector>

// ==================== 物品模板 ====================
struct ItemTemplate {
    int id;
    std::string name;
    ItemType type;
    ConsumableType subType;
    int quality;
    std::string description;
    int maxStack;
    
    // 消耗品效果
    struct Effect {
        EffectType type;
        int64_t value;
    };
    std::vector<Effect> effects;
    
    ItemTemplate() 
        : id(0), type(ItemType::NONE), subType(ConsumableType::HP_POTION)
        , quality(1), maxStack(1) {}
};

// ==================== 装备模板 ====================
struct EquipmentTemplate {
    int id;
    std::string name;
    EquipmentType type;
    EquipmentQuality quality;
    int level;
    
    BattleAttr baseAttr;
    AffixType mainAffixType;
    int mainAffixMin;
    int mainAffixMax;
    
    int setId;
    
    EquipmentTemplate()
        : id(0), type(EquipmentType::WEAPON)
        , quality(EquipmentQuality::WHITE)
        , level(1), mainAffixType(AffixType::NONE)
        , mainAffixMin(0), mainAffixMax(0), setId(0) {}
};

// ==================== 物品配置管理器 ====================
class ItemConfig {
public:
    static ItemConfig& instance() {
        static ItemConfig inst;
        return inst;
    }
    
    // 获取物品模板
    const ItemTemplate* getItem(int itemId) const {
        auto it = items_.find(itemId);
        return it != items_.end() ? &it->second : nullptr;
    }
    
    // 获取装备模板
    const EquipmentTemplate* getEquipment(int equipId) const {
        auto it = equipments_.find(equipId);
        return it != equipments_.end() ? &it->second : nullptr;
    }
    
    // 创建装备实例（随机词缀）
    Equipment createEquipment(int equipId, int level = 1) const;
    
    // 注册
    void regItem(const ItemTemplate& tmpl) { items_[tmpl.id] = tmpl; }
    void regEquipment(const EquipmentTemplate& tmpl) { equipments_[tmpl.id] = tmpl; }
    void regSetBonus(const SetBonus& bonus) { setBonuses_[bonus.setId] = bonus; }
    
    // 获取套装加成
    const SetBonus* getSetBonus(int setId) const {
        auto it = setBonuses_.find(setId);
        return it != setBonuses_.end() ? &it->second : nullptr;
    }

private:
    ItemConfig() { init(); }
    ItemConfig(const ItemConfig&) = delete;
    ItemConfig& operator=(const ItemConfig&) = delete;
    
    void init();
    void initConsumables();
    void initMaterials();
    void initEquipments();
    void initSetBonuses();
    
    std::unordered_map<int, ItemTemplate> items_;
    std::unordered_map<int, EquipmentTemplate> equipments_;
    std::unordered_map<int, SetBonus> setBonuses_;
};

#define GET_ITEM(id) ItemConfig::instance().getItem(id)
#define CREATE_EQUIP(id, lv) ItemConfig::instance().createEquipment(id, lv)
