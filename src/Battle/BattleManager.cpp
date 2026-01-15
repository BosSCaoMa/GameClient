#include "BattleManager.h"
#include "Player.h"
#include <algorithm>
#include <cmath>
#include "LogM.h"
#include <iostream>

using namespace std;
// ==================== 构造函数 ====================
BattleManager::BattleManager(Player* user, Player* enemy, LogCallback logCallback)
    : userPlayer_(user)
    , enemyPlayer_(enemy)
    , round_(0)
    , maxRounds_(50)
    , result_(Result::ONGOING)
    , logCallback_(logCallback)
{
    LOG_DEBUG("BattleManager initialized, user: %d, enemy: %d", userPlayer_->id, enemyPlayer_->id);
    if (!logCallback_) {
        setLogCallback([](const string& msg) {
            cout << msg << endl;
        });
    }
    rng_.seed(random_device{}());
    initBattle();
}

// ==================== 初始化 ====================
void BattleManager::initBattle()
{
    userTeam_.clear();
    enemyTeam_.clear();
    actionOrder_.clear();
    round_ = 0;
    result_ = Result::ONGOING;
    
    createBattleCharacters();
    buildUnitMap();
    buildTriggerBuckets();
    log("战斗开始");
}

void BattleManager::createBattleCharacters() {
    
    // 己方主角
    int battleId = 1;
    if (userPlayer_->mainCharacter) {
        userTeam_.push_back(BattleCharacter(
            userPlayer_->mainCharacter, battleId++));
    }
    
    // 己方武将
    for (size_t i = 0; i < userPlayer_->battleTeam.size() && i < 5; ++i) {
        int charId = userPlayer_->battleTeam[i];
        auto it = userPlayer_->characters.find(charId);
        if (it != userPlayer_->characters.end()) {
            userTeam_.push_back(BattleCharacter(
                &it->second, battleId++));
        }
    }
    
    // 敌方主角（负ID）
    battleId = -1;
    if (enemyPlayer_->mainCharacter) {
        enemyTeam_.push_back(BattleCharacter(
            enemyPlayer_->mainCharacter, battleId--));
    }
    
    // 敌方武将
    for (size_t i = 0; i < enemyPlayer_->battleTeam.size() && i < 5; ++i) {
        int charId = enemyPlayer_->battleTeam[i];
        auto it = enemyPlayer_->characters.find(charId);
        if (it != enemyPlayer_->characters.end()) {
            enemyTeam_.push_back(BattleCharacter(
                &it->second, battleId--));
        }
    }
}

void BattleManager::buildUnitMap()
{
    unitMap.clear();
    for (auto& ch : userTeam_) {
        unitMap[ch.battleId] = &ch;
    }
    for (auto& ch : enemyTeam_) {
        unitMap[ch.battleId] = &ch;
    }
}

void BattleManager::buildTriggerBuckets()
{
    for (auto& bucket : triggerBuckets_) {
        bucket.clear();
    }

    for (auto& ch : userTeam_) {
        registerCharacterTriggers(ch);
    }
    for (auto& ch : enemyTeam_) {
        registerCharacterTriggers(ch);
    }
}

void BattleManager::registerCharacterTriggers(BattleCharacter& ch)
{
    for (const auto& entry : ch.skills) {
        const auto index = static_cast<size_t>(entry.first);
        if (index >= triggerBuckets_.size()) {
            continue;
        }
        if (entry.second.empty()) {
            continue;
        }
        triggerBuckets_[index].push_back(&ch);
    }
}

// ==================== 公共接口.runBattle开始战斗 ====================
BattleManager::Result BattleManager::runBattle()
{
    // 触发开局技能
    triggerSkills(SkillTrigger::BATTLE_START);

    while (result_ == Result::ONGOING) {
        executeRound();
    }
    
    switch (result_) {
        case Result::WIN:  LOG_INFO("=== 战斗胜利！ ==="); break;
        case Result::LOSE: LOG_INFO("=== 战斗失败！ ==="); break;
        case Result::DRAW: LOG_INFO("=== 战斗平局！ ==="); break;
        default:
            LOG_ERROR("Unexpected battle result");
            break;
    }
    
    return result_;
}
// todo 优化整体战斗逻辑，可以先不做，等到程序可以运行之后，进行优化，这样可以看效果
BattleManager::Result BattleManager::executeRound()
{
    round_++;
    log("----- 回合 " + to_string(round_) + " 开始 -----");
    // 1. 回合开始阶段
    onRoundStart();
    
    // 2. 计算行动顺序
    calculateActionOrder();
    
    // 3. 依次行动
    for (BattleCharacter* actor : actionOrder_) {
        if (!actor->isAlive || actor->hasActed) {
            continue;
        }

        triggerSkills(SkillTrigger::TURN_START, actor);
        executeAction(actor);
        triggerSkills(SkillTrigger::TURN_END, actor);
        
        result_ = checkBattleResult();
        if (result_ != Result::ONGOING) {
            return result_;
        }
    }
    
    // 4. 回合结束阶段，处理buff等
    onRoundEnd();
    
    // 5. 检查战斗结果
    result_ = checkBattleResult();
    
    // 6. 检查回合上限
    if (round_ >= maxRounds_ && result_ == Result::ONGOING) {
        result_ = Result::DRAW;
    }
    
    return result_;
}

// ==================== 回合流程 ====================
void BattleManager::onRoundStart()
{
    // 重置行动标记
    for (auto& ch : userTeam_) {
        ch.hasActed = false;
    }
    for (auto& ch : enemyTeam_) {
        ch.hasActed = false;
    }
    
    // 触发回合开始技能
    triggerSkills(SkillTrigger::ROUND_START);
}

void BattleManager::onRoundEnd() {
    // 处理Buff（包括持续伤害）
    for (auto& ch : userTeam_) {
        if (ch.isAlive) {
            ch.tickBuffs();
        }
    }
    for (auto& ch : enemyTeam_) {
        if (ch.isAlive) {
            ch.tickBuffs();
        }
    }
    
    // 触发回合结束技能
    triggerSkills(SkillTrigger::ROUND_END);
    
    // 检查死亡
    checkDeaths();
}

void BattleManager::calculateActionOrder()
{
    actionOrder_.clear();
    
    for (auto& ch : userTeam_) {
        if (ch.isAlive) {
            actionOrder_.push_back(&ch);
        }
    }
    for (auto& ch : enemyTeam_) {
        if (ch.isAlive) {
            actionOrder_.push_back(&ch);
        }
    }
    
    // 按速度降序排序
    sort(actionOrder_.begin(), actionOrder_.end(),
        [](BattleCharacter* a, BattleCharacter* b) {
            if (a->currentAttr.speed != b->currentAttr.speed) {
                return a->currentAttr.speed > b->currentAttr.speed;
            }
            // 速度相同，ID大的先动
            return a->battleId > b->battleId;
        });
}

// ==================== 行动执行（最核心战斗逻辑） ====================
void BattleManager::executeAction(BattleCharacter* actor)
{
    actor->hasActed = true;
    
    // 被控制无法行动
    if (actor->isControlled()) {
        log(actor->name + " 被控制，无法行动！");
        return;
    }
    
    // 选择技能：优先怒气技能
    Skill* skill = actor->getRageSkill();
    if (skill) {
        actor->currentAttr.rage -= 4;
        log(actor->name + " 释放 [" + skill->name + "]！");
    } else {  // 怒气不足或者无怒气技能
        skill = actor->getNormalAttack();
        if (!skill) {
            LOG_ERROR("错误：%s 无法进行普通攻击！", actor->name.c_str());
            return;
        }
        log(actor->name + " 进行普通攻击");
    }
    
    executeSkill(actor, skill);
    
    // 普攻回怒
    if (skill->trigger == SkillTrigger::NORMAL_ATTACK) {
        actor->addRage(1);
    }

    checkDeaths();
}

void BattleManager::executeSkill(BattleCharacter* caster, Skill* skill)
{
    for (const SkillEffect& effect : skill->effects) {
        executeEffect(caster, effect, skill->id);
    }
}

// ====================== 效果执行函数，最终都会调用这个函数 =======================
void BattleManager::executeEffect(BattleCharacter* caster, const SkillEffect& effect, int skillId)
{
    // 概率判定
    if (effect.chance < 100 && !rollChance(effect.chance)) {
        log("  效果: " + getEffectName(effect.effect) + " 未触发");
        return;
    }
    
    // 获取目标
    vector<BattleCharacter*> targets = getTargets(caster, effect.target);
    if (targets.empty()) {
        return;
    }
    
    // 对每个目标应用效果
    for (BattleCharacter* target : targets) {
        if (target && target->isAlive) {
            applyEffect(caster, target, effect, skillId);
        }
    }
}

void BattleManager::applyEffect(BattleCharacter* caster, BattleCharacter* target,
    const SkillEffect& effect, int skillId)
{
    switch (effect.effect) {
        // ===== 即时伤害 =====
        case EffectType::DAMAGE: 
        case EffectType::PIERCE: {
            int64_t damage = calculateDamage(caster, target, effect);
            target->takeDamage(damage, effect.effect != EffectType::PIERCE);
            if (!target->isAlive) {
                triggerSkills(SkillTrigger::ON_KILL, caster);
                break;
            }
            log("  - " + target->name + " 受到 " + to_string(damage) + " 点伤害 (剩余HP: " +
                to_string(target->currentAttr.hp) + ")");

            if (damage > 0 && caster->currentAttr.mutiHitRate > 0) {
                if (rollChance(caster->currentAttr.mutiHitRate)) {
                    log("    连击触发，" + caster->name + " 进行额外一次攻击！");
                    applyEffect(caster, target, effect, skillId);
                }
            }
            triggerSkills(SkillTrigger::ON_HIT, target);
            break;
        }
        case EffectType::TRUE_DAMAGE: {
            int64_t damage = calculateTrueDamage(caster, target, effect);
            target->takeDamage(damage, false);
            log("  - " + target->name + " 受到 " + to_string(damage) + " 点真实伤害 (剩余HP: " +
                to_string(target->currentAttr.hp) + ")");
            triggerSkills(SkillTrigger::ON_HIT, target);
            break;
        }
        // ===== 治疗 =====
        case EffectType::HEAL: {
            int64_t heal = calculateHeal(caster, target, effect);
            target->heal(heal);
            log("  - " + target->name + " 恢复 " + to_string(heal) + " 点生命" +
                " (当前HP: " + to_string(target->currentAttr.hp) + ")");
            break;
        }
        
        // ===== 怒气操作 =====
        case EffectType::RAGE_ADD:
        case EffectType::RAGE_REDUCE: {
            int amount = static_cast<int>(effect.value);
            bool isAdd = (effect.effect == EffectType::RAGE_ADD);
            string action = isAdd ? "获得" : "减少";
            target->addRage(isAdd ? amount : -amount);
            log("  - " + target->name + " " + action + " " + to_string(amount) + " 点怒气");
            break;
        }
        
        // ===== 护盾 =====
        case EffectType::SHIELD: {
            int64_t shield = calculateValue(caster, effect);
            target->addShield(shield);
            break;
        }
        // ===== Buff效果（全部取%） =====
        case EffectType::BUFF_MAX_HP:
        case EffectType::BUFF_ATK:
        case EffectType::BUFF_DEF:
        case EffectType::BUFF_SPEED:
        case EffectType::BUFF_CRIT_RATE:
        case EffectType::BUFF_CRIT_RESIST: 
        case EffectType::BUFF_HIT_RATE:
        case EffectType::BUFF_DODGE_RATE:
        // ===== 持续BUFF =====
        case EffectType::BUFF_REGEN: {
            int64_t buffValue = calculateValue(caster, effect, target);
            target->addBuff(effect.effect, buffValue, effect.duration, skillId);
            log("  - " + target->name + " 获得状态 " + getEffectName(effect.effect));
            break;
        }
        // ===== 控制效果 =====
        case EffectType::STUN:
        case EffectType::SILENCE:
        case EffectType::FREEZE:
            triggerSkills(SkillTrigger::ON_CONTROL, target);
        // ====== 特殊状态 ======
        case EffectType::BARRIER:
        case EffectType::INJURY:
        case EffectType::LOCK_BLEED:
        case EffectType::IMMUNITY:
        case EffectType::INVINCIBLE: {
            target->addBuff(effect.effect, effect.value, effect.duration, skillId);
            log("  - " + target->name + " 获得状态 " + getEffectName(effect.effect));
            break;
        }
        
        // ===== 持续伤害 =====
        case EffectType::POISON:
        case EffectType::BURN:
        case EffectType::BLEED:
        case EffectType::CURSE: {
            int64_t dotDamage = calculateValue(caster, effect, target);
            target->addBuff(effect.effect, dotDamage, effect.duration, skillId);
            break;
        }
        
        // ===== 嘲讽 =====
        case EffectType::TAUNT: {
            triggerSkills(SkillTrigger::ON_CONTROL, target);
            target->addBuff(effect.effect, caster->battleId, effect.duration, skillId);
            break;
        }
        
        // ====== 特殊效果2：清除/转移 ======
        case EffectType::DISPEL: {
            int limit = static_cast<int>(effect.value);
            int removed = target->dispelBuffs(limit);
            string msg = removed > 0 ?
                ("  - " + target->name + " 被驱散 " + to_string(removed) + " 个增益") :
                ("  - " + target->name + " 没有可驱散的增益");
            log(msg);
            break;
        }
        case EffectType::CLEANSE: {
            int limit = static_cast<int>(effect.value);
            int removed = target->cleanseDebuffs(limit);
            string msg = removed > 0 ?
                ("  - " + target->name + " 净化 " + to_string(removed) + " 个负面状态") :
                ("  - " + target->name + " 没有可净化的负面状态");
            log(msg);
            break;
        }
        case EffectType::TRANSFER_DEBUFF: {
            int limit = static_cast<int>(effect.value);
            int moved = caster->transferDebuffsTo(target, limit);
            string msg = moved > 0 ?
                ("  - " + caster->name + " 将 " + to_string(moved) + " 个负面状态转移给 " + target->name) :
                ("  - " + caster->name + " 没有可转移的负面状态");
            log(msg);
            break;
        }

        case EffectType::REVIVE: {
            // 复活需要特殊处理，目标选择时需要包含死亡单位
            if (!target->isAlive) {
                target->isAlive = true;
                int64_t reviveHp = target->currentAttr.maxHp * effect.value / 100;
                target->currentAttr.hp = reviveHp;
            }
            break;
        }
        
        default:
            break;
    }
}

// ==================== 数值计算 ====================
int64_t BattleManager::calculateValue(BattleCharacter* caster, const SkillEffect& effect, BattleCharacter* target)
{
    int64_t baseValue = effect.value;
    
    switch (effect.valueType) {
        case ValueType::FIXED:
            return baseValue;
            
        case ValueType::PERCENT_ATK:
            return caster->currentAttr.atk * baseValue / 100;
            
        case ValueType::PERCENT_DEF:
            return caster->currentAttr.def * baseValue / 100;
            
        case ValueType::PERCENT_MAX_HP:
            return caster->currentAttr.maxHp * baseValue / 100;
            
        case ValueType::PERCENT_CUR_HP:
            return caster->currentAttr.hp * baseValue / 100;
            
        case ValueType::PERCENT_LOST_HP: {
            int64_t lostHp = caster->currentAttr.maxHp - caster->currentAttr.hp;
            return lostHp * baseValue / 100;
        }
        case ValueType::PERCENT_TARGET_MAXHP:
            return target->currentAttr.maxHp * baseValue / 100;
        default:
            return baseValue;
    }
}

int64_t BattleManager::calculateDamage(BattleCharacter* caster, BattleCharacter* target,
    const SkillEffect& effect)
{
    // 1. 闪避判定
    int dodgeRate = target->currentAttr.dodgeRate - caster->currentAttr.hitRate;
    dodgeRate = clamp(dodgeRate, 0, 50);
    if (rollChance(dodgeRate)) {
        return 0;
    }

    // 2. 基础伤害
    int64_t baseDamage = calculateValue(caster, effect, target);
    
    // 3. 防御减伤 (先扣防御)
    int64_t defense = max(static_cast<int64_t>(0), target->currentAttr.def);
    int64_t damage = max(static_cast<int64_t>(1), baseDamage - defense);
    
    // 4. 暴击判定 (对扣完防御后的伤害暴击)
    int critRate = caster->currentAttr.critRate - target->currentAttr.critResist;
    critRate = clamp(critRate, 0, 100);
    if (rollChance(critRate)) {
        damage = damage * caster->currentAttr.critDamage / 100;
        LOG_INFO("  - 暴击！");
    }
    
    // 5. 伤害加成/减免
    damage = damage * (100 + caster->currentAttr.damageBonus) / 100;
    damage = damage * (100 - target->currentAttr.damageReduction) / 100;
    
    // 6. 保底伤害
    damage = max(static_cast<int64_t>(1), damage);
    
    return damage;
}


int64_t BattleManager::calculateHeal(BattleCharacter* caster, BattleCharacter* target,
     const SkillEffect& effect)
{
    int64_t heal = calculateValue(caster, effect);
    // 治疗加成
    heal = heal * (100 + caster->currentAttr.healBonus) / 100;
    return max(static_cast<int64_t>(0), heal);
}

// ==================== 目标选择 ====================
vector<BattleCharacter*> BattleManager::getTargets(BattleCharacter* caster, TargetType type)
{
    vector<BattleCharacter*> targets;
    vector<BattleCharacter>& allyTeam = caster->battleId > 0 ? userTeam_ : enemyTeam_;
    vector<BattleCharacter>& enemyTeamRef = caster->battleId > 0 ? enemyTeam_ : userTeam_;

    // 检查嘲讽，如果为真则必须只攻击嘲讽者
    for (const Buff& b : caster->buffs) {
        if (b.type == EffectType::TAUNT) {
            targets.push_back(unitMap[b.value]);
            return targets;
        }
    }
    
    switch (type) {
        // ===== 基础目标 =====
        case TargetType::SELF:
            return {caster};
            
        case TargetType::ALLY_SINGLE:
            return {selectRandomAlive(allyTeam)};
            
        case TargetType::ALLY_ALL:
            for (auto& ch : allyTeam) {
                if (ch.isAlive) targets.push_back(&ch);
            }
            return targets;
            
        case TargetType::ENEMY_SINGLE: {
            BattleCharacter* t = selectEnemyTarget(caster, enemyTeamRef);
            if (t) targets.push_back(t);
            return targets;
        }

        case TargetType::ENEMY_ALL:
            for (auto& ch : enemyTeamRef) {
                if (ch.isAlive) targets.push_back(&ch);
            }
            return targets;
        
        // ===== 前后排 =====
        case TargetType::ALLY_FRONT_ROW:
            for (auto& ch : allyTeam) {
                if (ch.isAlive && ch.isInFrontRow()) targets.push_back(&ch);
            }
            return targets;
            
        case TargetType::ALLY_BACK_ROW:
            for (auto& ch : allyTeam) {
                if (ch.isAlive && !ch.isInFrontRow()) targets.push_back(&ch);
            }
            return targets;
            
        case TargetType::ENEMY_FRONT_ROW:
            for (auto& ch : enemyTeamRef) {
                if (ch.isAlive && ch.isInFrontRow()) targets.push_back(&ch);
            }
            return targets;
            
        case TargetType::ENEMY_BACK_ROW:
            for (auto& ch : enemyTeamRef) {
                if (ch.isAlive && !ch.isInFrontRow()) targets.push_back(&ch);
            }
            return targets;
        
        // ===== 按攻击力排序 =====
        case TargetType::ALLY_ATK_TOP1:
        case TargetType::ALLY_ATK_TOP2:
        case TargetType::ALLY_ATK_TOP3:
            return selectByAttr(allyTeam, type, TargetType::ALLY_ATK_TOP1, true, true);
        
        case TargetType::ENEMY_ATK_TOP1:
        case TargetType::ENEMY_ATK_TOP2:
        case TargetType::ENEMY_ATK_TOP3:
            return selectByAttr(enemyTeamRef, type, TargetType::ENEMY_ATK_TOP1, true, true);
        
        // ===== 按血量排序 =====
        case TargetType::ALLY_HP_LOW1:
        case TargetType::ALLY_HP_LOW2:
        case TargetType::ALLY_HP_LOW3:
            return selectByAttr(allyTeam, type, TargetType::ALLY_HP_LOW1, false, false);
        
        case TargetType::ENEMY_HP_LOW1:
        case TargetType::ENEMY_HP_LOW2:
        case TargetType::ENEMY_HP_LOW3:
            return selectByAttr(enemyTeamRef, type, TargetType::ENEMY_HP_LOW1, false, false);
        
        // ===== 随机目标 =====
        case TargetType::ALLY_RANDOM_1:
        case TargetType::ALLY_RANDOM_2:
        case TargetType::ALLY_RANDOM_3: {
            int count = static_cast<int>(type) - static_cast<int>(TargetType::ALLY_RANDOM_1) + 1;
            return selectRandom(allyTeam, count);
        }
        
        case TargetType::ENEMY_RANDOM_1:
        case TargetType::ENEMY_RANDOM_2:
        case TargetType::ENEMY_RANDOM_3: {
            int count = static_cast<int>(type) - static_cast<int>(TargetType::ENEMY_RANDOM_1) + 1;
            return selectRandom(enemyTeamRef, count);
        }
        
        default:
            return {nullptr};
    }
    
    return targets;
}

BattleCharacter* BattleManager::selectEnemyTarget(BattleCharacter* caster,
    vector<BattleCharacter>& enemies)
{
    // 优先选择同Id的敌人，否则随机
    if (enemies[abs(caster->battleId)].isAlive) {
        return &enemies[abs(caster->battleId)];
    } else {
        return selectRandomAlive(enemies);
    }
    
    return nullptr;
}

BattleCharacter* BattleManager::selectRandomAlive(vector<BattleCharacter>& team)
{
    vector<BattleCharacter*> alive;
    for (auto& ch : team) {
        if (ch.isAlive) {
            alive.push_back(&ch);
        }
    }
    
    if (alive.empty()) {
        return nullptr;
    }
    
    uniform_int_distribution<size_t> dist(0, alive.size() - 1);
    return alive[dist(rng_)];
}

vector<BattleCharacter*> BattleManager::selectRandom(vector<BattleCharacter>& team, 
    int count)
{
    vector<BattleCharacter*> alive;
    for (auto& ch : team) {
        if (ch.isAlive) {
            alive.push_back(&ch);
        }
    }
    
    // 打乱顺序
    shuffle(alive.begin(), alive.end(), rng_);
    
    // 截取指定数量
    if (static_cast<int>(alive.size()) > count) {
        alive.resize(count);
    }
    
    return alive;
}

vector<BattleCharacter*> BattleManager::selectByAttr(vector<BattleCharacter>& team,
    TargetType now, TargetType base, bool byAtk, bool highest)
{
    int count = static_cast<int>(now) - static_cast<int>(base) + 1;
    vector<BattleCharacter*> alive;
    for (auto& ch : team) {
        if (ch.isAlive) {
            alive.push_back(&ch);
        }
    }
    
    // 排序
    if (byAtk) {
        sort(alive.begin(), alive.end(),
            [highest](BattleCharacter* a, BattleCharacter* b) {
                return highest ? 
                    (a->currentAttr.atk > b->currentAttr.atk) :
                    (a->currentAttr.atk < b->currentAttr.atk);
            });
    } else {
        // 按当前HP排序
        sort(alive.begin(), alive.end(),
            [highest](BattleCharacter* a, BattleCharacter* b) {
                return highest ? 
                    (a->currentAttr.hp > b->currentAttr.hp) :
                    (a->currentAttr.hp < b->currentAttr.hp);
            });
    }
    
    // 截取指定数量
    if (static_cast<int>(alive.size()) > count) {
        alive.resize(count);
    }
    
    return alive;
}

// ==================== 技能触发 ====================
void BattleManager::triggerSkills(SkillTrigger trigger)
{
    dispatchTrigger(trigger, nullptr, trigger == SkillTrigger::ON_DEATH);
}

void BattleManager::triggerSkills(SkillTrigger trigger, BattleCharacter* specificCharacter)
{
    if (!specificCharacter) {
        return;
    }
    const bool allowDead = (trigger == SkillTrigger::ON_DEATH);
    dispatchTrigger(trigger, specificCharacter, allowDead);
}

void BattleManager::dispatchTrigger(SkillTrigger trigger, BattleCharacter* filter, bool allowDead)
{
    const auto index = static_cast<size_t>(trigger);
    if (index >= triggerBuckets_.size()) {
        return;
    }

    triggerDispatchBuffer_.clear();
    auto& registry = triggerBuckets_[index];
    triggerDispatchBuffer_.reserve(registry.size());

    for (BattleCharacter* ch : registry) {
        if (!ch) {
            continue;
        }
        if (filter && ch != filter) {
            continue;
        }
        if (allowDead) {
            if (!filter && ch->isAlive) {
                continue;
            }
        } else if (!ch->isAlive) {
            continue;
        }
        triggerDispatchBuffer_.push_back(ch);
    }

    if (triggerDispatchBuffer_.empty()) {
        return;
    }

    sort(triggerDispatchBuffer_.begin(), triggerDispatchBuffer_.end(),
        [](BattleCharacter* a, BattleCharacter* b) {
            return a->currentAttr.speed > b->currentAttr.speed;
        });

    for (BattleCharacter* ch : triggerDispatchBuffer_) {
        const auto* skillList = ch->getSkills(trigger);
        if (!skillList) {
            continue;
        }
        for (const Skill& skill : *skillList) {
            if (skill.id == 0) {
                continue;
            }
            if (trigger == SkillTrigger::ON_DEATH) {
                LOG_INFO("%s 触发阵亡技能 [%s]", ch->name.c_str(), skill.name.c_str());
            } else if (trigger != SkillTrigger::ON_HIT) {
                log(ch->name + "触发技能: " + skill.name);
            }
            for (const SkillEffect& effect : skill.effects) {
                executeEffect(ch, effect, skill.id);
            }
        }
    }
}

// ==================== 状态检查 ====================
BattleManager::Result BattleManager::checkBattleResult()
{
    bool userAlive = isTeamAlive(userTeam_);
    bool enemyAlive = isTeamAlive(enemyTeam_);
    
    if (!userAlive && !enemyAlive) {
        return Result::DRAW;
    }
    if (!enemyAlive) {
        return Result::WIN;
    }
    if (!userAlive) {
        return Result::LOSE;
    }
    
    return Result::ONGOING;
}

void BattleManager::checkDeaths()
{
    // 检查己方死亡
    for (auto& ch : userTeam_) {
        if (ch.currentAttr.hp <= 0 && ch.isAlive) {
            ch.isAlive = false;
            ch.currentAttr.hp = 0;
            LOG_INFO("%s 阵亡！", ch.name.c_str());
            triggerSkills(SkillTrigger::ON_DEATH, &ch);
        }
    }
    
    // 检查敌方死亡
    for (auto& ch : enemyTeam_) {
        if (ch.currentAttr.hp <= 0 && ch.isAlive) {
            ch.isAlive = false;
            ch.currentAttr.hp = 0;
            triggerSkills(SkillTrigger::ON_DEATH, &ch);
        }
    }
}

bool BattleManager::isTeamAlive(const vector<BattleCharacter>& team) const
{
    for (const auto& ch : team) {
        if (ch.isAlive) {
            return true;
        }
    }
    return false;
}

int BattleManager::countAlive(const vector<BattleCharacter>& team) const
{
    int count = 0;
    for (const auto& ch : team) {
        if (ch.isAlive) {
            count++;
        }
    }
    return count;
}

// ==================== 工具函数 ====================

string BattleManager::getEffectName(EffectType type) const {
    switch (type) {
        case EffectType::DAMAGE:        return "伤害";
        case EffectType::HEAL:          return "治疗";
        case EffectType::RAGE_ADD:      return "怒气增加";
        case EffectType::RAGE_REDUCE:   return "怒气减少";
        case EffectType::BUFF_ATK:      return "攻击力";
        case EffectType::BUFF_DEF:      return "防御力";
        case EffectType::BUFF_SPEED:    return "速度";
        case EffectType::BUFF_CRIT_RATE: return "暴击率";
        case EffectType::BUFF_CRIT_RESIST: return "抗暴率";
        case EffectType::BARRIER:       return "屏障";
        case EffectType::BUFF_REGEN:    return "持续治疗";
        // 补充于此
        case EffectType::STUN:          return "眩晕";
        case EffectType::SILENCE:       return "沉默";
        case EffectType::FREEZE:        return "冰冻";
        case EffectType::POISON:        return "中毒";
        case EffectType::BURN:          return "灼烧";
        case EffectType::BLEED:         return "流血";
        case EffectType::CURSE:         return "诅咒";
        case EffectType::SHIELD:        return "护盾";
        case EffectType::TAUNT:         return "嘲讽";
        case EffectType::INJURY:        return "重伤";
        case EffectType::LOCK_BLEED:    return "锁血";
        case EffectType::IMMUNITY:      return "免疫控制";
        case EffectType::INVINCIBLE:    return "无敌";
        case EffectType::DISPEL:        return "驱散";
        case EffectType::CLEANSE:       return "净化";
        case EffectType::TRANSFER_DEBUFF: return "负面转移";
        case EffectType::REVIVE:        return "复活";
        default:                        return "未知效果";
    }
}

bool BattleManager::rollChance(int percent) {
    if (percent <= 0) return false;
    if (percent >= 100) return true;
    
    uniform_int_distribution<int> dist(1, 100);
    return dist(rng_) <= percent;
}

void BattleManager::setLogCallback(LogCallback callback)
{
    logCallback_ = callback;
}

int64_t BattleManager::calculateTrueDamage(BattleCharacter* caster, BattleCharacter* target,
    const SkillEffect& effect)
{
    // 1. 闪避判定
    int dodgeRate = target->currentAttr.dodgeRate - caster->currentAttr.hitRate;
    dodgeRate = clamp(dodgeRate, 0, 50);
    if (rollChance(dodgeRate)) {
        return 0;
    }

    // 2. 基础伤害
    int64_t baseDamage = calculateValue(caster, effect, target);
    int critRate = caster->currentAttr.critRate - target->currentAttr.critResist;
    critRate = clamp(critRate, 0, 100);
    if (rollChance(critRate)) {
        baseDamage = baseDamage * caster->currentAttr.critDamage / 100;
        LOG_INFO("  - 真伤暴击！");
    }

    // 5. 伤害加成/减免
    baseDamage = baseDamage * (100 + caster->currentAttr.damageBonus) / 100;
    baseDamage = baseDamage * (100 - target->currentAttr.damageReduction) / 100;

    return max(static_cast<int64_t>(1), baseDamage);
}

BattleManager::Result BattleManager::getResult() const {
    return result_;
}

int BattleManager::getRound() const {
    return round_;
}

bool BattleManager::isOver() const {
    return result_ != Result::ONGOING;
}

const vector<BattleCharacter>& BattleManager::getUserTeam() const {
    return userTeam_;
}

const vector<BattleCharacter>& BattleManager::getEnemyTeam() const {
    return enemyTeam_;
}