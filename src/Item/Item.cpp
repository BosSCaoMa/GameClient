#include "Item.h"
#include <sstream>
#include <iomanip>

// ==================== 计算装备总属性 ====================
BattleAttr Equipment::calculateTotalAttr(const BattleAttr& charAttr) const {
    // 1. 从装备基础属性开始
    BattleAttr total = charAttr;
    
    // 2. 应用主词缀
    applyAffix(total, charAttr, mainAffix);
    
    // 3. 应用所有副词缀
    for (const auto& affix : subAffixes) {
        applyAffix(total, charAttr, affix);
    }
    
    return total;
}

// ==================== 应用词缀到属性 ====================
void Equipment::applyAffix(BattleAttr& total, const BattleAttr& charAttr, const EquipmentAffix& affix)
{
    switch (affix.type) {
        case AffixType::NONE:
            break;
        
        // ========== 攻击力 ==========
        case AffixType::ATK_FLAT:
            total.atk += affix.value;
            break;
        
        case AffixType::ATK_PERCENT:
            // 基于基础攻击力的百分比加成
            total.atk += charAttr.atk * affix.value / 100;
            break;
        
        // ========== 防御力 ==========
        case AffixType::DEF_FLAT:
            total.def += affix.value;
            break;
        
        case AffixType::DEF_PERCENT:
            total.def += charAttr.def * affix.value / 100;
            break;
        
        // ========== 生命值 ==========
        case AffixType::HP_FLAT:
            total.hp += affix.value;
            total.maxHp += affix.value;
            break;
        
        case AffixType::HP_PERCENT:
            total.hp += charAttr.hp * affix.value / 100;
            total.maxHp += charAttr.maxHp * affix.value / 100;
            break;
        
        // ========== 速度 ==========
        case AffixType::SPEED_FLAT:
            total.speed += affix.value;
            break;
        
        // ========== 暴击 ==========
        case AffixType::CRIT_RATE:
            total.critRate += affix.value;
            break;
        
        case AffixType::CRIT_DAMAGE:
            total.critDamage += affix.value;
            break;
        
        // ========== 命中/闪避 ==========
        case AffixType::HIT_RATE:
            total.hitRate += affix.value;
            break;
        
        case AffixType::DODGE_RATE:
            total.dodgeRate += affix.value;
            break;
        
        // ========== 伤害加成 ==========
        case AffixType::DAMAGE_BONUS:
            // 存储在扩展属性中（如果需要）
            total.damageBonus += affix.value;
            break;
        
        case AffixType::DAMAGE_REDUCTION:
            total.damageReduction += affix.value;
            break;
        
        case AffixType::SKILL_DAMAGE:
            total.skillDamage += affix.value;
            break;
        
        // ========== 特殊效果 ==========
        case AffixType::LIFESTEAL:
            total.lifesteal += affix.value;
            break;
        
        case AffixType::COUNTER_RATE:
            total.counterRate += affix.value;
            break;
        
        case AffixType::RAGE_GAIN:
            total.rageGain += affix.value;
            break;
        
        case AffixType::MULTI_HIT_RATE:
            total.mutiHitRate += affix.value;
            break;

        case AffixType::HEAL_BONUS:
            total.healBonus += affix.value;
            break;
        
        // ========== 抗性 ==========
        case AffixType::STUN_RESIST:
            total.stunResist += affix.value;
            break;
        
        case AffixType::SILENCE_RESIST:
            total.silenceResist += affix.value;
            break;
        
        case AffixType::POISON_RESIST:
            total.poisonResist += affix.value;
            break;
        
        default:
            break;
    }
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
