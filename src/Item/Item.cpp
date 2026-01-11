#include "Item.h"
#include <sstream>
#include <iomanip>

// ==================== 应用词缀到属性 ====================
void Equipment::applyAffix(BattleAttr& attr, const EquipmentAffix& affix, 
                           const BattleAttr& baseAttr) {
    switch (affix.type) {
        case AffixType::NONE:
            break;
        
        // ========== 攻击力 ==========
        case AffixType::ATK_FLAT:
            attr.atk += affix.value;
            break;
        
        case AffixType::ATK_PERCENT:
            // 基于基础攻击力的百分比加成
            attr.atk += baseAttr.atk * affix.value / 100;
            break;
        
        // ========== 防御力 ==========
        case AffixType::DEF_FLAT:
            attr.def += affix.value;
            break;
        
        case AffixType::DEF_PERCENT:
            attr.def += baseAttr.def * affix.value / 100;
            break;
        
        // ========== 生命值 ==========
        case AffixType::HP_FLAT:
            attr.hp += affix.value;
            attr.maxHp += affix.value;
            break;
        
        case AffixType::HP_PERCENT:
            attr.hp += baseAttr.hp * affix.value / 100;
            attr.maxHp += baseAttr.maxHp * affix.value / 100;
            break;
        
        // ========== 速度 ==========
        case AffixType::SPEED_FLAT:
            attr.speed += affix.value;
            break;
        
        // ========== 暴击 ==========
        case AffixType::CRIT_RATE:
            attr.critRate += affix.value;
            break;
        
        case AffixType::CRIT_DAMAGE:
            attr.critDamage += affix.value;
            break;
        
        // ========== 命中/闪避 ==========
        case AffixType::HIT_RATE:
            attr.hitRate += affix.value;
            break;
        
        case AffixType::DODGE_RATE:
            attr.dodgeRate += affix.value;
            break;
        
        // ========== 伤害加成 ==========
        case AffixType::DAMAGE_BONUS:
            // 存储在扩展属性中（如果需要）
            // attr.damageBonus += affix.value;
            break;
        
        case AffixType::DAMAGE_REDUCTION:
            // attr.damageReduction += affix.value;
            break;
        
        case AffixType::SKILL_DAMAGE:
            // attr.skillDamage += affix.value;
            break;
        
        // ========== 特殊效果 ==========
        case AffixType::LIFESTEAL:
            // attr.lifesteal += affix.value;
            break;
        
        case AffixType::COUNTER_RATE:
            // attr.counterRate += affix.value;
            break;
        
        case AffixType::RAGE_GAIN:
            // attr.rageGain += affix.value;
            break;
        
        case AffixType::HEAL_BONUS:
            // attr.healBonus += affix.value;
            break;
        
        // ========== 抗性 ==========
        case AffixType::STUN_RESIST:
            // attr.stunResist += affix.value;
            break;
        
        case AffixType::SILENCE_RESIST:
            // attr.silenceResist += affix.value;
            break;
        
        case AffixType::POISON_RESIST:
            // attr.poisonResist += affix.value;
            break;
        
        default:
            break;
    }
}

// ==================== 计算装备总属性 ====================
BattleAttr Equipment::calculateTotalAttr(const BattleAttr& charBaseAttr) const {
    // 1. 从装备基础属性开始
    BattleAttr total = baseAttr;
    
    // 2. 应用主词缀
    applyAffix(total, mainAffix, charBaseAttr);
    
    // 3. 应用所有副词缀
    for (const auto& affix : subAffixes) {
        applyAffix(total, affix, charBaseAttr);
    }
    
    return total;
}

// ==================== 词缀描述 ====================
std::string EquipmentAffix::getDescription() const {
    std::ostringstream oss;
    
    switch (type) {
        case AffixType::ATK_FLAT:
            oss << "攻击力 +" << value;
            break;
        case AffixType::ATK_PERCENT:
            oss << "攻击力 +" << value << "%";
            break;
        case AffixType::DEF_FLAT:
            oss << "防御力 +" << value;
            break;
        case AffixType::DEF_PERCENT:
            oss << "防御力 +" << value << "%";
            break;
        case AffixType::HP_FLAT:
            oss << "生命值 +" << value;
            break;
        case AffixType::HP_PERCENT:
            oss << "生命值 +" << value << "%";
            break;
        case AffixType::SPEED_FLAT:
            oss << "速度 +" << value;
            break;
        case AffixType::CRIT_RATE:
            oss << "暴击率 +" << value << "%";
            break;
        case AffixType::CRIT_DAMAGE:
            oss << "暴击伤害 +" << value << "%";
            break;
        case AffixType::HIT_RATE:
            oss << "命中率 +" << value << "%";
            break;
        case AffixType::DODGE_RATE:
            oss << "闪避率 +" << value << "%";
            break;
        case AffixType::DAMAGE_BONUS:
            oss << "伤害加成 +" << value << "%";
            break;
        case AffixType::DAMAGE_REDUCTION:
            oss << "伤害减免 +" << value << "%";
            break;
        case AffixType::SKILL_DAMAGE:
            oss << "技能伤害 +" << value << "%";
            break;
        case AffixType::LIFESTEAL:
            oss << "吸血 +" << value << "%";
            break;
        case AffixType::COUNTER_RATE:
            oss << "反击率 +" << value << "%";
            break;
        case AffixType::RAGE_GAIN:
            oss << "怒气获取 +" << value << "%";
            break;
        case AffixType::HEAL_BONUS:
            oss << "治疗加成 +" << value << "%";
            break;
        case AffixType::STUN_RESIST:
            oss << "眩晕抗性 +" << value << "%";
            break;
        case AffixType::SILENCE_RESIST:
            oss << "沉默抗性 +" << value << "%";
            break;
        case AffixType::POISON_RESIST:
            oss << "中毒抗性 +" << value << "%";
            break;
        default:
            oss << "未知属性";
            break;
    }
    
    return oss.str();
}
