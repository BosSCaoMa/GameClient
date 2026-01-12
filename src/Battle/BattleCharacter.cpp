#include "BattleCharacter.h"
#include "LogM.h"

using namespace std;

BattleCharacter::BattleCharacter(Character* ch, int battleId)
: original(ch), battleId(battleId), name(ch->name), 
    isAlive(true), hasActed(false), shieldValue(0)
{
    if (ch == nullptr) {
        LOG_ERROR("BattleCharacter::fromCharacter: null Character pointer");
    }
    baseAttr = ch->getTotalAttr();
    currentAttr = baseAttr;
    
    // 复制技能
    for (const auto& pair : ch->skills) {
        skills[pair.first] = pair.second;
    }
    
}

// ==================== Buff管理 ====================
// 添加buff之后，首先会重新计算属性，过期之后会进行删除，而伤害类Buff会在tickBuffs中处理
void BattleCharacter::addBuff(EffectType type, int64_t value, int duration, int sourceId)
{
    // 同类型同来源刷新
    for (auto& b : buffs) {
        if (b.type == type && b.sourceId == sourceId) {
            b.duration = std::max(b.duration, duration);
            b.value = value;
            recalculateAttr();
            return;
        }
    }
    buffs.emplace_back(type, value, duration, sourceId);
    recalculateAttr();
}

void BattleCharacter::tickBuffs()
{
    // 处理持续, todo:处理各种buff效果
    for (const Buff& b : buffs) {
        if (b.type == EffectType::POISON || 
            b.type == EffectType::BURN ||
            b.type == EffectType::BLEED) {
            takeDamage(b.value, false);
        } else if (b.type == EffectType::BUFF_REGEN) {
            heal(b.value);
        }
    }
    
    // 移除过期Buff
    buffs.erase(
        std::remove_if(buffs.begin(), buffs.end(), [](Buff& b) { return b.tick(); }),
        buffs.end()
    );
    recalculateAttr();
}

void BattleCharacter::recalculateAttr()
{
    currentAttr = baseAttr;
    currentAttr.hp = std::min(currentAttr.hp, currentAttr.maxHp);  // 保持当前HP
    
    // 先保存当前HP
    int64_t currentHp = currentAttr.hp;
    int currentRage = currentAttr.rage;
    
    currentAttr = baseAttr;
    currentAttr.hp = currentHp;
    currentAttr.rage = currentRage;
    
    for (const Buff& buff : buffs) {
        switch (buff.type) {
            case EffectType::BUFF_MAX_HP:
                currentAttr.maxHp = baseAttr.maxHp * (100 + buff.value) / 100;
                break;
            case EffectType::BUFF_ATK:
                currentAttr.atk = baseAttr.atk * (100 + buff.value) / 100;
                break;
            case EffectType::BUFF_DEF:
                currentAttr.def = baseAttr.def * (100 + buff.value) / 100;
                break;
            case EffectType::BUFF_SPEED:
                currentAttr.speed = baseAttr.speed * (100 + static_cast<int>(buff.value)) / 100;
                break;
            case EffectType::BUFF_CRIT_RATE:
                currentAttr.critRate += static_cast<int>(buff.value);
                break;
            case EffectType::BUFF_CRIT_RESIST:
                currentAttr.critResist += static_cast<int>(buff.value);
                break;
            case EffectType::BUFF_HIT_RATE:
                currentAttr.hitRate += static_cast<int>(buff.value);
                break;
            case EffectType::BUFF_DODGE_RATE:
                currentAttr.dodgeRate += static_cast<int>(buff.value);
                break;
            default:
                break;
        }
    }
}

// ==================== 状态检查 ====================
bool BattleCharacter::isControlled() const
{
    for (const Buff& b : buffs) {
        if (b.type == EffectType::STUN || b.type == EffectType::FREEZE) {
            return true;
        }
    }
    return false;
}

bool BattleCharacter::isSilenced() const {
    for (const Buff& b : buffs) {
        if (b.type == EffectType::SILENCE) return true;
    }
    return false;
}

// ==================== 战斗操作 ====================
void BattleCharacter::takeDamage(int64_t damage, bool canBeShielded = true)
{
    if (canBeShielded && shieldValue > 0) {
        if (shieldValue >= damage) {
            shieldValue -= damage;
            return;
        } else {
            damage -= shieldValue;
            shieldValue = 0;
        }
    }
    
    currentAttr.hp -= damage;
    if (currentAttr.hp <= 0) {
        currentAttr.hp = 0;
        isAlive = false;
    }
}

void BattleCharacter::heal(int64_t amount) {
    if (!isAlive) return;
    currentAttr.hp = std::min(currentAttr.hp + amount, currentAttr.maxHp);
}

void BattleCharacter::addRage(int amount) {
    currentAttr.rage = std::clamp(currentAttr.rage + amount, 0, 6); // 【设定】假设最大怒气为6
}

void BattleCharacter::addShield(int64_t amount) {
    shieldValue += amount;
}

// ==================== 技能相关 ====================
Skill* BattleCharacter::getSkill(SkillTrigger trigger) {
    auto it = skills.find(trigger);
    if (it != skills.end()) {
        return &it->second;
    }
    return nullptr;
}

Skill* BattleCharacter::getNormalAttack() {
    return getSkill(SkillTrigger::NORMAL_ATTACK);
}

Skill* BattleCharacter::getRageSkill() {
    // todo: 合击技能
    Skill* skill = getSkill(SkillTrigger::RAGE_SKILL);
    if (skill && currentAttr.rage >= 4) {
        return skill;
    }
    return nullptr;
}