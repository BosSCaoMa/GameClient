#include "Character.h"
#include "ItemConfig.h"
#include <algorithm>
#include "characterConfig.h"
#include <cmath>

// ==================== 构造函数 ====================
Character::Character() 
    : id(0), level(1), star(1), quality(1)
    , exp(0), expMax(100), breakthrough(0) {}

Character::Character(int id_, const std::string& name_, int level_, int star_)
    : id(id_), name(name_), level(level_), star(star_), quality(1)
    , exp(0), expMax(100), breakthrough(0)
{
    initExpMax();
}

// ==================== 技能管理 ====================
void Character::setSkill(const Skill& skill) {
    if (skill.id == 0) {
        return;
    }
    skills[skill.trigger] = skill;
}

const Skill* Character::getSkill(SkillTrigger trigger) const {
    auto it = skills.find(trigger);
    return it != skills.end() ? &it->second : nullptr;
}

bool Character::hasSkill(SkillTrigger trigger) const {
    return skills.find(trigger) != skills.end();
}

// ==================== 装备管理 ====================
void Character::equipItem(const Equipment& equip)
{
    // 卸下旧装备
    unequipItem(equip.type);
    
    // 装备新装备
    equipments[equip.type] = equip;
    setSkill(GET_SKILL(equip.skillId));
    // 重新计算属性
    recalculateAttr();
}

void Character::unequipItem(EquipmentType type) {
    equipments.erase(type);
    for (auto it = skills.begin(); it != skills.end(); ++it) {
        if (it->second.id == equipments[type].skillId) {
            it = skills.erase(it);
        }
    }
    recalculateAttr();
}

const Equipment* Character::getEquipment(EquipmentType type) const {
    auto it = equipments.find(type);
    return it != equipments.end() ? &it->second : nullptr;
}

bool Character::hasEquipment(EquipmentType type) const {
    return equipments.find(type) != equipments.end();
}

// ==================== 属性计算 ====================
void Character::recalculateAttr() {
    // 1. 从基础属性开始
    baseAttr = originAttr;
    
    // 2. 应用所有装备
    for (const auto& [type, equip] : equipments) {
        BattleAttr equipAttr = equip.calculateTotalAttr(originAttr);
        baseAttr += equipAttr;
    }
    
    // 3. 应用套装加成
    applySetBonuses();
    
    // 4. 确保 HP 不超过最大值
    if (baseAttr.hp > baseAttr.maxHp) {
        baseAttr.hp = baseAttr.maxHp;
    }
}

void Character::applySetBonuses()
{
    // 统计套装件数
    std::unordered_map<int, int> setCounts;
    for (const auto& [type, equip] : equipments) {
        if (equip.setId > 0) {
            setCounts[equip.setId]++;
        }
    }
    
    // 应用套装效果
    for (const auto& [setId, count] : setCounts) {
        const auto* bonus = ItemConfig::instance().getSetBonus(setId);
        if (!bonus) continue;
        
        for (const auto& b : bonus->bonuses) {
            if (count >= b.pieceCount) {
                for (const auto& affix : b.affixes) {
                    // 直接应用词缀（使用 Equipment 的静态方法）
                    // 注意：这里需要 Equipment::applyAffix 改为 public 或提供友元
                    switch (affix.type) {
                        case AffixType::ATK_FLAT:
                            baseAttr.atk += affix.value;
                            break;
                        case AffixType::ATK_PERCENT:
                            baseAttr.atk += originAttr.atk * affix.value / 100;
                            break;
                        case AffixType::HP_FLAT:
                            baseAttr.hp += affix.value;
                            baseAttr.maxHp += affix.value;
                            break;
                        case AffixType::HP_PERCENT:
                            baseAttr.hp += originAttr.hp * affix.value / 100;
                            baseAttr.maxHp += originAttr.maxHp * affix.value / 100;
                            break;
                        case AffixType::DEF_PERCENT:
                            baseAttr.def += originAttr.def * affix.value / 100;
                            break;
                        case AffixType::CRIT_RATE:
                            baseAttr.critRate += affix.value;
                            break;
                        case AffixType::CRIT_DAMAGE:
                            baseAttr.critDamage += affix.value;
                            break;
                        case AffixType::DAMAGE_REDUCTION:
                            baseAttr.damageReduction += affix.value;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

uint64_t Character::calculateCombatPower() const {
    // 战力计算公式（可根据需求调整）
    uint64_t power = 0;
    
    power += baseAttr.hp / 10;               // 生命值权重 0.1
    power += baseAttr.atk * 5;               // 攻击力权重 5
    power += baseAttr.def * 3;               // 防御力权重 3
    power += baseAttr.speed * 2;             // 速度权重 2
    power += baseAttr.critRate * 10;         // 暴击率权重 10
    power += baseAttr.critDamage * 5;        // 暴击伤害权重 5
    power += baseAttr.damageBonus * 8;       // 伤害加成权重 8
    power += baseAttr.damageReduction * 8;   // 伤害减免权重 8
    
    // 星级加成
    power = power * (100 + star * 10) / 100;
    
    return power;
}

// ==================== 升级/进阶 ====================
bool Character::addExp(int64_t amount) {
    exp += amount;
    
    bool leveledUp = false;
    while (exp >= expMax && canLevelUp()) {
        levelUp();
        leveledUp = true;
    }
    
    return leveledUp;
}

bool Character::canLevelUp() const {
    // 等级上限：基础80级 + 突破等级*10
    int maxLevel = 80 + breakthrough * 10;
    return level < maxLevel;
}

void Character::levelUp() {
    if (!canLevelUp()) return;
    
    level++;
    exp -= expMax;
    
    // 更新经验上限
    initExpMax();
    
    // 重新计算属性（CharacterConfig 会根据等级计算基础属性）
    // 基础属性->等级加固定值-->突破/星级加成
    originAttr = CharacterConfig::instance().calculateAttrForLevel(id, level, star, breakthrough);
    recalculateAttr();
}

void Character::initExpMax() {
    // 经验需求公式：100 * level^1.5
    expMax = static_cast<int64_t>(100 * std::pow(level, 1.5));
}

bool Character::canBreakthrough() const {
    // 突破条件：
    // 1. 等级达到上限
    // 2. 突破次数未达上限
    int maxLevel = 80 + breakthrough * 10;
    return level >= maxLevel && breakthrough < 10;
}

void Character::DoBreakthrough() {
    if (!canBreakthrough()) return;
    
    breakthrough++;
    
    // 突破后属性额外加成（可选）
    originAttr.hp += originAttr.maxHp * 5 / 100;
    originAttr.maxHp += originAttr.maxHp * 5 / 100;
    originAttr.atk += originAttr.atk * 5 / 100;
    originAttr.def += originAttr.def * 5 / 100;
    
    recalculateAttr();
}

bool Character::canUpgradeStar() const {
    // 升星条件（需要消耗材料，这里只判断星级上限）
    return star < 5;
}

void Character::upgradeStar()
{
    if (!canUpgradeStar()) return;
    
    star++;
    
    // 升星属性加成
    originAttr.hp += originAttr.maxHp * 10 / 100;
    originAttr.maxHp += originAttr.maxHp * 10 / 100;
    originAttr.atk += originAttr.atk * 10 / 100;
    originAttr.def += originAttr.def * 10 / 100;
    
    recalculateAttr();
}
