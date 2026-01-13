#include "ItemConfig.h"
#include <random>
#include "LogM.h"

// ==================== 创建装备实例 ====================
Equipment ItemConfig::createEquipment(int equipId) const
{
    const auto* tmpl = getEquipment(equipId);
    if (!tmpl) {
        LOG_ERROR("Equipment template not found for id: {}", equipId);
        return Equipment();
    }
    
    // 生成主词缀
    Equipment equip(tmpl->id, tmpl->name, tmpl->type, tmpl->quality, tmpl->skillId, tmpl->setId);
    for (const auto& affix : tmpl->mainAffixs) {
        equip.mainAffixs.push_back(affix);
    }
    // 随机器
    std::random_device rd;  // 随机数种子（硬件随机数）
    std::mt19937 gen(rd());  // 梅森旋转算法随机数生成器
    
    // 根据品质生成副词缀
    int subAffixCount = static_cast<int>(tmpl->quality) - 2;  // 蓝装1个, 紫装2个, 橙装3个, 红装4个
    subAffixCount = std::min(subAffixCount, 4);
    
    static const AffixType possibleAffixes[] = {
        AffixType::CRIT_RATE, AffixType::CRIT_DAMAGE, AffixType::HIT_RATE,
        AffixType::COUNTER_RATE, AffixType::HEAL_BONUS, AffixType::MULTI_HIT_RATE,
        AffixType::STUN_RESIST, AffixType::SILENCE_RESIST, AffixType::POISON_RESIST
    }; // 随机词缀池,全部是百分比加成

    // 生成模板指定范围内的随机数
    int minvalue = 3, maxvalue = 5;
    minvalue += static_cast<int>(tmpl->quality);
    maxvalue += static_cast<int>(tmpl->quality) * 2;
    int numPossible = sizeof(possibleAffixes) / sizeof(possibleAffixes[0]);
    std::uniform_int_distribution<> affixDis(0, numPossible - 1); // possibleAffixes数组索引
    std::uniform_int_distribution<> valueDis(minvalue, maxvalue); // 词缀数值范围
    
    for (int i = 0; i < subAffixCount; ++i) {
        AffixType type = possibleAffixes[affixDis(gen)];
        int64_t value = valueDis(gen);
        equip.subAffixes.push_back({type, value});
    }
    
    return equip;
}


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
        ItemTemplate tmpl(10001, "小型生命药水", IT::CONSUMABLE, CT::HP_POTION, 1, 999);
        tmpl.description = "恢复500点生命值";
        tmpl.effects.push_back({ET::HEAL, 500});
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl(10002, "中型生命药水", IT::CONSUMABLE, CT::HP_POTION, 2, 999);
        tmpl.description = "恢复2000点生命值";
        tmpl.effects.push_back({ET::HEAL, 2000});
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl(10003, "大型生命药水", IT::CONSUMABLE, CT::HP_POTION, 3, 999);
        tmpl.description = "恢复5000点生命值";
        tmpl.effects.push_back({ET::HEAL, 5000});
        regItem(tmpl);
    }
    
    // ===== 怒气药水 (10101-10199) =====
    {
        ItemTemplate tmpl(10101, "怒气精华", IT::CONSUMABLE, CT::RAGE_POTION, 2, 999);
        tmpl.description = "立即获得2点怒气";
        tmpl.effects.push_back({ET::RAGE_ADD, 2});
        regItem(tmpl);
    }
    
    // ===== 经验药水 (10201-10299) =====
    {
        ItemTemplate tmpl(10201, "经验丹（小）", IT::CONSUMABLE, CT::EXP_POTION, 1, 999);
        tmpl.description = "获得1000点经验";
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl(10202, "经验丹（中）", IT::CONSUMABLE, CT::EXP_POTION, 2, 999);
        tmpl.description = "获得5000点经验";
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl(10203, "经验丹（大）", IT::CONSUMABLE, CT::EXP_POTION, 3, 999);
        tmpl.description = "获得20000点经验";
        regItem(tmpl);
    }
}

// ==================== 材料配置 ====================
void ItemConfig::initMaterials() {
    using IT = ItemType;
    
    // ===== 强化材料 (11001-11099) =====
    {
        ItemTemplate tmpl(11001, "铁矿石", IT::MATERIAL, ConsumableType::NONE, 1, 999);
        tmpl.description = "用于强化装备";
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl(11002, "秘银", IT::MATERIAL, ConsumableType::NONE, 3, 999);
        tmpl.description = "稀有强化材料";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
    
    {
        ItemTemplate tmpl(11003, "精金", IT::MATERIAL, ConsumableType::NONE, 4, 999);
        tmpl.description = "高级强化材料";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
    
    // ===== 进阶材料 (11101-11199) =====
    {
        ItemTemplate tmpl(11101, "武将碎片", IT::MATERIAL, ConsumableType::NONE, 2, 999);
        tmpl.description = "用于合成或进阶武将";
        tmpl.maxStack = 999;
        regItem(tmpl);
    }
}

// ==================== 装备配置 ====================
void ItemConfig::initEquipments() {

    // 分离各类初始化函数，便于维护
    initWeapons();
    initArmors();
    initHelmets();
    initBoots();
    initSteeds();
    initTallys();
    initTreasures();
    initFamouss();
      
}

void ItemConfig::initWeapons()
{
    // ===== 武器 (20001-20999) =====
    {
        EquipmentTemplate tmpl(20001, "青铜剑", ET::WEAPON, EQ::WHITE);
        tmpl.mainAffixs.push_back({AT::ATK_FLAT, 20});
        regEquipment(tmpl);
    }
    
    {
        EquipmentTemplate tmpl(20002, "铁剑", ET::WEAPON, EQ::GREEN);
        tmpl.mainAffixs.push_back({AT::ATK_FLAT, 50});
        regEquipment(tmpl);
    }
    
    {
        EquipmentTemplate tmpl(20003, "青龙偃月刀", ET::WEAPON, EQ::ORANGE);
        tmpl.mainAffixs.push_back({AT::ATK_PERCENT, 15});
        tmpl.setId = 1001;  // 蜀国套装
        regEquipment(tmpl);
    }
}

void ItemConfig::initArmors() {
    // ===== 盔甲 (21001-21999) =====
    {
        EquipmentTemplate tmpl(21001, "布衣", ET::ARMOR, EQ::WHITE); 
        tmpl.mainAffixs.push_back({AT::HP_FLAT, 100});
        regEquipment(tmpl);
    }
    
    {
        EquipmentTemplate tmpl(21002, "铁甲", ET::ARMOR, EQ::BLUE);
        tmpl.mainAffixs.push_back({AT::DEF_PERCENT, 10});
        regEquipment(tmpl);
    }
}

void ItemConfig::initHelmets()
{
    // ===== 头盔 (22001-22999) =====
    EquipmentTemplate tmpl(22001, "铁盔", ET::HELMET, EQ::GREEN);
    tmpl.mainAffixs.push_back({AT::HP_PERCENT, 8});
    regEquipment(tmpl);
}

void ItemConfig::initBoots()
{
    // ===== 鞋子 (23001-23999) =====
    EquipmentTemplate tmpl(23001, "疾风靴", ET::BOOTS, EQ::BLUE);
    tmpl.mainAffixs.push_back({AT::SPEED_FLAT, 5});
    regEquipment(tmpl);
}

void ItemConfig::initSteeds()
{
    // Implementation for steeds initialization
}

void ItemConfig::initTallys()
{
    // Implementation for tallys initialization
}

void ItemConfig::initTreasures()
{
    // Implementation for treasures initialization
}

void ItemConfig::initFamouss()
{
    // Implementation for famouss initialization
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