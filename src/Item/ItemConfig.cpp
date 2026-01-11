#include "ItemConfig.h"
#include <random>

void ItemConfig::init() {
    initConsumables();
    initMaterials();
    initEquipments();
    initSetBonuses();
}

// ==================== 消耗品配置 ====================
void ItemConfig::initConsumables() {
    using IT = ItemType;
    using CT = ConsumableType;
    using ET = EffectType;
    
    // ===== 生命药水 (10001-10099) =====
    {
        ItemTemplate tmpl;
        tmpl.id = 10001;
        tmpl.name = "小型生命药水";
        tmpl.type = IT::CONSUMABLE;
        tmpl.subType = CT::HP_POTION;
        tmpl.quality = 1;
        tmpl.description = "恢复500点生命值";
        tmpl.maxStack = 99;
        tmpl.effects.push_back({ET::HEAL, 500});
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl;
        tmpl.id = 10002;
        tmpl.name = "中型生命药水";
        tmpl.type = IT::CONSUMABLE;
        tmpl.subType = CT::HP_POTION;
        tmpl.quality = 2;
        tmpl.description = "恢复2000点生命值";
        tmpl.maxStack = 99;
        tmpl.effects.push_back({ET::HEAL, 2000});
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl;
        tmpl.id = 10003;
        tmpl.name = "大型生命药水";
        tmpl.type = IT::CONSUMABLE;
        tmpl.subType = CT::HP_POTION;
        tmpl.quality = 3;
        tmpl.description = "恢复5000点生命值";
        tmpl.maxStack = 99;
        tmpl.effects.push_back({ET::HEAL, 5000});
        regItem(tmpl);
    }
    
    // ===== 怒气药水 (10101-10199) =====
    {
        ItemTemplate tmpl;
        tmpl.id = 10101;
        tmpl.name = "怒气精华";
        tmpl.type = IT::CONSUMABLE;
        tmpl.subType = CT::RAGE_POTION;
        tmpl.quality = 2;
        tmpl.description = "立即获得50点怒气";
        tmpl.maxStack = 99;
        tmpl.effects.push_back({ET::RAGE_ADD, 50});
        regItem(tmpl);
    }
    
    // ===== 经验药水 (10201-10299) =====
    {
        ItemTemplate tmpl;
        tmpl.id = 10201;
        tmpl.name = "经验丹（小）";
        tmpl.type = IT::CONSUMABLE;
        tmpl.subType = CT::EXP_POTION;
        tmpl.quality = 1;
        tmpl.description = "获得1000点经验";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl;
        tmpl.id = 10202;
        tmpl.name = "经验丹（中）";
        tmpl.type = IT::CONSUMABLE;
        tmpl.subType = CT::EXP_POTION;
        tmpl.quality = 2;
        tmpl.description = "获得5000点经验";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl;
        tmpl.id = 10203;
        tmpl.name = "经验丹（大）";
        tmpl.type = IT::CONSUMABLE;
        tmpl.subType = CT::EXP_POTION;
        tmpl.quality = 3;
        tmpl.description = "获得20000点经验";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
}

// ==================== 材料配置 ====================
void ItemConfig::initMaterials() {
    using IT = ItemType;
    
    // ===== 强化材料 (11001-11099) =====
    {
        ItemTemplate tmpl;
        tmpl.id = 11001;
        tmpl.name = "铁矿石";
        tmpl.type = IT::MATERIAL;
        tmpl.quality = 1;
        tmpl.description = "用于强化装备";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl;
        tmpl.id = 11002;
        tmpl.name = "秘银";
        tmpl.type = IT::MATERIAL;
        tmpl.quality = 3;
        tmpl.description = "稀有强化材料";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl;
        tmpl.id = 11003;
        tmpl.name = "精金";
        tmpl.type = IT::MATERIAL;
        tmpl.quality = 4;
        tmpl.description = "高级强化材料";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
    
    // ===== 进阶材料 (11101-11199) =====
    {
        ItemTemplate tmpl;
        tmpl.id = 11101;
        tmpl.name = "武将碎片";
        tmpl.type = IT::MATERIAL;
        tmpl.quality = 2;
        tmpl.description = "用于合成或进阶武将";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
}

// ==================== 装备配置 ====================
void ItemConfig::initEquipments() {
    using ET = EquipmentType;
    using EQ = EquipmentQuality;
    using AT = AffixType;
    
    // ===== 武器 (20001-20999) =====
    {
        EquipmentTemplate tmpl;
        tmpl.id = 20001;
        tmpl.name = "青铜剑";
        tmpl.type = ET::WEAPON;
        tmpl.quality = EQ::WHITE;
        tmpl.level = 1;
        tmpl.baseAttr.atk = 100;
        tmpl.mainAffixType = AT::ATK_FLAT;
        tmpl.mainAffixMin = 20;
        tmpl.mainAffixMax = 40;
        regEquipment(tmpl);
    }
    
    {
        EquipmentTemplate tmpl;
        tmpl.id = 20002;
        tmpl.name = "铁剑";
        tmpl.type = ET::WEAPON;
        tmpl.quality = EQ::GREEN;
        tmpl.level = 10;
        tmpl.baseAttr.atk = 250;
        tmpl.mainAffixType = AT::ATK_FLAT;
        tmpl.mainAffixMin = 50;
        tmpl.mainAffixMax = 80;
        regEquipment(tmpl);
    }
    
    {
        EquipmentTemplate tmpl;
        tmpl.id = 20003;
        tmpl.name = "青龙偃月刀";
        tmpl.type = ET::WEAPON;
        tmpl.quality = EQ::ORANGE;
        tmpl.level = 50;
        tmpl.baseAttr.atk = 800;
        tmpl.baseAttr.critRate = 5;
        tmpl.mainAffixType = AT::ATK_PERCENT;
        tmpl.mainAffixMin = 15;
        tmpl.mainAffixMax = 25;
        tmpl.setId = 1001;  // 蜀国套装
        regEquipment(tmpl);
    }
    
    // ===== 盔甲 (21001-21999) =====
    {
        EquipmentTemplate tmpl;
        tmpl.id = 21001;
        tmpl.name = "布衣";
        tmpl.type = ET::ARMOR;
        tmpl.quality = EQ::WHITE;
        tmpl.level = 1;
        tmpl.baseAttr.hp = 500;
        tmpl.baseAttr.def = 50;
        tmpl.mainAffixType = AT::HP_FLAT;
        tmpl.mainAffixMin = 100;
        tmpl.mainAffixMax = 200;
        regEquipment(tmpl);
    }
    
    {
        EquipmentTemplate tmpl;
        tmpl.id = 21002;
        tmpl.name = "铁甲";
        tmpl.type = ET::ARMOR;
        tmpl.quality = EQ::BLUE;
        tmpl.level = 20;
        tmpl.baseAttr.hp = 2000;
        tmpl.baseAttr.def = 200;
        tmpl.mainAffixType = AT::DEF_PERCENT;
        tmpl.mainAffixMin = 10;
        tmpl.mainAffixMax = 20;
        regEquipment(tmpl);
    }
    
    // ===== 头盔 (22001-22999) =====
    {
        EquipmentTemplate tmpl;
        tmpl.id = 22001;
        tmpl.name = "铁盔";
        tmpl.type = ET::HELMET;
        tmpl.quality = EQ::GREEN;
        tmpl.level = 15;
        tmpl.baseAttr.hp = 800;
        tmpl.baseAttr.def = 100;
        tmpl.mainAffixType = AT::HP_PERCENT;
        tmpl.mainAffixMin = 8;
        tmpl.mainAffixMax = 15;
        regEquipment(tmpl);
    }
    
    // ===== 鞋子 (23001-23999) =====
    {
        EquipmentTemplate tmpl;
        tmpl.id = 23001;
        tmpl.name = "疾风靴";
        tmpl.type = ET::BOOTS;
        tmpl.quality = EQ::BLUE;
        tmpl.level = 25;
        tmpl.baseAttr.speed = 20;
        tmpl.mainAffixType = AT::SPEED_FLAT;
        tmpl.mainAffixMin = 5;
        tmpl.mainAffixMax = 10;
        regEquipment(tmpl);
    }
    
    // ===== 饰品 (24001-24999) =====
    {
        EquipmentTemplate tmpl;
        tmpl.id = 24001;
        tmpl.name = "力量戒指";
        tmpl.type = ET::ACCESSORY_1;
        tmpl.quality = EQ::PURPLE;
        tmpl.level = 30;
        tmpl.baseAttr.atk = 300;
        tmpl.mainAffixType = AT::CRIT_RATE;
        tmpl.mainAffixMin = 5;
        tmpl.mainAffixMax = 10;
        regEquipment(tmpl);
    }
    
    {
        EquipmentTemplate tmpl;
        tmpl.id = 24002;
        tmpl.name = "吸血项链";
        tmpl.type = ET::ACCESSORY_2;
        tmpl.quality = EQ::ORANGE;
        tmpl.level = 40;
        tmpl.baseAttr.hp = 1500;
        tmpl.mainAffixType = AT::LIFESTEAL;
        tmpl.mainAffixMin = 8;
        tmpl.mainAffixMax = 15;
        regEquipment(tmpl);
    }
}

// ==================== 套装配置 ====================
void ItemConfig::initSetBonuses() {
    // ===== 蜀国套装 (1001) =====
    {
        SetBonus bonus;
        bonus.setId = 1001;
        bonus.name = "蜀国五虎";
        
        // 2件套
        {
            SetBonus::Bonus b;
            b.pieceCount = 2;
            b.desc = "攻击力 +10%";
            b.affixes.push_back({AffixType::ATK_PERCENT, 10});
            bonus.bonuses.push_back(b);
        }
        
        // 4件套
        {
            SetBonus::Bonus b;
            b.pieceCount = 4;
            b.desc = "暴击率 +10%, 暴击伤害 +20%";
            b.affixes.push_back({AffixType::CRIT_RATE, 10});
            b.affixes.push_back({AffixType::CRIT_DAMAGE, 20});
            bonus.bonuses.push_back(b);
        }
        
        regSetBonus(bonus);
    }
    
    // ===== 魏国套装 (1002) =====
    {
        SetBonus bonus;
        bonus.setId = 1002;
        bonus.name = "魏国铁骑";
        
        // 2件套
        {
            SetBonus::Bonus b;
            b.pieceCount = 2;
            b.desc = "生命值 +15%";
            b.affixes.push_back({AffixType::HP_PERCENT, 15});
            bonus.bonuses.push_back(b);
        }
        
        // 4件套
        {
            SetBonus::Bonus b;
            b.pieceCount = 4;
            b.desc = "防御力 +20%, 伤害减免 +10%";
            b.affixes.push_back({AffixType::DEF_PERCENT, 20});
            b.affixes.push_back({AffixType::DAMAGE_REDUCTION, 10});
            bonus.bonuses.push_back(b);
        }
        
        regSetBonus(bonus);
    }
}

// ==================== 创建装备实例 ====================
Equipment ItemConfig::createEquipment(int equipId, int level) const {
    const auto* tmpl = getEquipment(equipId);
    if (!tmpl) {
        return Equipment();
    }
    
    Equipment equip(tmpl->id, tmpl->name, tmpl->type, tmpl->quality, level);
    equip.baseAttr = tmpl->baseAttr;
    equip.setId = tmpl->setId;
    
    // 随机生成主词缀
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(tmpl->mainAffixMin, tmpl->mainAffixMax);
    equip.mainAffix = {tmpl->mainAffixType, dis(gen)};
    
    // 根据品质生成副词缀
    int subAffixCount = static_cast<int>(tmpl->quality) - 1;  // 绿装1个，蓝装2个...
    subAffixCount = std::min(subAffixCount, 4);
    
    // 简化：随机生成副词缀（实际可以做更复杂的词缀池）
    static const AffixType possibleAffixes[] = {
        AffixType::CRIT_RATE, AffixType::CRIT_DAMAGE,
        AffixType::HIT_RATE, AffixType::DODGE_RATE,
        AffixType::DAMAGE_BONUS, AffixType::LIFESTEAL
    };
    
    std::uniform_int_distribution<> affixDis(0, 5);
    std::uniform_int_distribution<> valueDis(3, 8);
    
    for (int i = 0; i < subAffixCount; ++i) {
        AffixType type = possibleAffixes[affixDis(gen)];
        int64_t value = valueDis(gen);
        equip.subAffixes.push_back({type, value});
    }
    
    return equip;
}
