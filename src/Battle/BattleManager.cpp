#include "BattleManager.h"
#include "Player.h"
#include <algorithm>
#include <cmath>
#include "LogM.h"
#include <iostream>
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
        setLogCallback([](const std::string& msg) {
            std::cout << msg << std::endl;
        });
    }
    rng_.seed(std::random_device{}());
    initBattle();
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

BattleManager::Result BattleManager::executeRound()
{
    if (result_ != Result::ONGOING) {
        return result_;
    }
    
    round_++;
    LOG_INFO("===== 第 %d 回合 =====", round_);
    
    // 1. 回合开始阶段
    onRoundStart();
    
    // 2. 计算行动顺序
    calculateActionOrder();
    
    // 3. 依次行动
    for (BattleCharacter* actor : actionOrder_) {
        if (!actor->isAlive || actor->hasActed) {
            continue;
        }
        
        executeAction(actor);
        
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

BattleManager::Result BattleManager::getResult() const {
    return result_;
}

int BattleManager::getRound() const {
    return round_;
}

bool BattleManager::isOver() const {
    return result_ != Result::ONGOING;
}

const std::vector<BattleCharacter>& BattleManager::getUserTeam() const {
    return userTeam_;
}

const std::vector<BattleCharacter>& BattleManager::getEnemyTeam() const {
    return enemyTeam_;
}

// ==================== 初始化 ====================
void BattleManager::initBattle() {
    userTeam_.clear();
    enemyTeam_.clear();
    actionOrder_.clear();
    round_ = 0;
    result_ = Result::ONGOING;
    
    createBattleCharacters();
    
    log("战斗开始");
}

void BattleManager::createBattleCharacters() {
    int battleId = 1;
    
    // 己方主角
    if (userPlayer_->character) {
        userTeam_.push_back(BattleCharacter(
            userPlayer_->character, battleId++));
    }
    
    // 己方武将
    for (size_t i = 0; i < userPlayer_->characterOrder.size() && i < 5; ++i) {
        int charId = userPlayer_->characterOrder[i];
        auto it = userPlayer_->characters.find(charId);
        if (it != userPlayer_->characters.end()) {
            userTeam_.push_back(BattleCharacter(
                &it->second, battleId++));
        }
    }
    
    // 敌方主角（负ID）
    battleId = -1;
    if (enemyPlayer_->character) {
        enemyTeam_.push_back(BattleCharacter(
            enemyPlayer_->character, battleId--));
    }
    
    // 敌方武将
    for (size_t i = 0; i < enemyPlayer_->characterOrder.size() && i < 5; ++i) {
        int charId = enemyPlayer_->characterOrder[i];
        auto it = enemyPlayer_->characters.find(charId);
        if (it != enemyPlayer_->characters.end()) {
            enemyTeam_.push_back(BattleCharacter(
                &it->second, battleId--));
        }
    }
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

void BattleManager::calculateActionOrder() {
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
    std::sort(actionOrder_.begin(), actionOrder_.end(),
        [](BattleCharacter* a, BattleCharacter* b) {
            if (a->currentAttr.speed != b->currentAttr.speed) {
                return a->currentAttr.speed > b->currentAttr.speed;
            }
            // 速度相同，ID大的先动
            return a->battleId > b->battleId;
        });
}

// ==================== 行动执行 ====================
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

void BattleManager::executeEffect(BattleCharacter* caster, const SkillEffect& effect, int skillId)
{
    // 概率判定
    if (effect.chance < 100 && !rollChance(effect.chance)) {
        return;
    }
    
    // 获取目标
    std::vector<BattleCharacter*> targets = getTargets(caster, effect.target);
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
        case EffectType::DAMAGE: {
            int64_t damage = calculateDamage(caster, target, effect);
            target->takeDamage(damage);
            log("  - " + target->name + " 受到 " + std::to_string(damage) + " 点伤害 (剩余HP: " + std::to_string(target->currentAttr.hp) + ")");
            triggerOnHit(target, caster);
            break;
        }
        
        // ===== 治疗 =====
        case EffectType::HEAL: {
            int64_t heal = calculateHeal(caster, target, effect);
            target->heal(heal);
            log("  - " + target->name + " 恢复 " + std::to_string(heal) + " 点生命" +
                " (当前HP: " + std::to_string(target->currentAttr.hp) + ")");
            break;
        }
        
        // ===== 怒气操作 =====
        case EffectType::RAGE_ADD: {
            int amount = static_cast<int>(effect.value);
            target->addRage(amount);
            log("  - " + target->name + " 获得 " + std::to_string(amount) + " 点怒气");
            break;
        }
        
        case EffectType::RAGE_REDUCE: {
            int amount = static_cast<int>(effect.value);
            target->addRage(-amount);
            log("  - " + target->name + " 减少 " + std::to_string(amount) + " 点怒气");
            break;
        }
        
        // ===== 护盾 =====
        case EffectType::SHIELD: {
            int64_t shield = calculateValue(caster, effect);
            target->addShield(shield);
            break;
        }
        
        // ===== Buff效果 =====
        case EffectType::BUFF_ATK:
        case EffectType::BUFF_DEF:
        case EffectType::BUFF_SPEED:
        case EffectType::BUFF_CRIT_RATE:
        case EffectType::BUFF_CRIT_RESIST: 
        case EffectType::BUFF_HIT_RATE:
        case EffectType::BUFF_DODGE_RATE:
        // ===== 控制效果 =====
        case EffectType::STUN:
        case EffectType::SILENCE:
        case EffectType::FREEZE: {
            target->addBuff(effect.effect, effect.value, effect.duration, skillId);
            std::string buffName = getEffectName(effect.effect);
            std::string sign = effect.value >= 0 ? "+" : "";
            LOG_INFO(" %s 获得 %s %s%d (%d回合)", target->name.c_str(), buffName.c_str(), sign.c_str(), 
                static_cast<int>(effect.value), effect.duration);
            break;
        }
        
        // ===== 持续伤害 =====
        case EffectType::POISON:
        case EffectType::BURN:
        case EffectType::BLEED: {
            int64_t dotDamage = calculateValue(caster, effect, target);
            target->addBuff(effect.effect, dotDamage, effect.duration, skillId);
            break;
        }
        
        // ===== 嘲讽 =====
        case EffectType::TAUNT: {
            target->addBuff(effect.effect, caster->battleId, effect.duration, skillId);
            break;
        }
        
        // ===== 复活 =====
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
        case ValueType::PERCENT_TARGET_HP:
            return target->currentAttr.maxHp * baseValue / 100;
        case ValueType::PERCENT:
            return baseValue; // 通用百分比，直接返回数值，调用处自行处理
        default:
            return baseValue;
    }
}

int64_t BattleManager::calculateDamage(BattleCharacter* caster, BattleCharacter* target,
    const SkillEffect& effect)
{
    // 1. 闪避判定
    int dodgeRate = target->currentAttr.dodgeRate - caster->currentAttr.hitRate;
    dodgeRate = std::clamp(dodgeRate, 0, 50); // 闪避率最大50%，clamp将数值限制在指定的最小值和最大值之间
    if (rollChance(dodgeRate)) {
        return 0;
    }

    // 2. 基础伤害
    int64_t baseDamage = calculateValue(caster, effect, target);
    
    // 3. 暴击判定
    bool isCrit = false;
    int critRate = caster->currentAttr.critRate - target->currentAttr.critResist;
    critRate = std::clamp(critRate, 0, 100);
    if (rollChance(critRate)) {
        isCrit = true;
        baseDamage = baseDamage * caster->currentAttr.critDamage / 100;
        LOG_INFO("  - 暴击！");
    }
    
    // 4. 防御减伤: 实际伤害 = 基础伤害 * 200 / (200 + 防御)
    int64_t defense = std::max(static_cast<int64_t>(0), target->currentAttr.def);
    int64_t damage = baseDamage - defense;
    
    // 5. 伤害加成/减免
    damage = damage * (100 + caster->currentAttr.damageBonus) / 100;
    damage = damage * (100 - target->currentAttr.damageReduction) / 100;
    
    // 6. 保底伤害
    damage = std::max(static_cast<int64_t>(1), damage);
    
    return damage;
}

int64_t BattleManager::calculateHeal(BattleCharacter* caster, BattleCharacter* target,
     const SkillEffect& effect)
{
    int64_t heal = calculateValue(caster, effect);
    return std::max(static_cast<int64_t>(0), heal);
}

// ==================== 目标选择 ====================
std::vector<BattleCharacter*> BattleManager::getTargets(BattleCharacter* caster, TargetType type)
{
    std::vector<BattleCharacter*> targets;
    
    std::vector<BattleCharacter>& allyTeam = caster->battleId > 0 ? userTeam_ : enemyTeam_;
    std::vector<BattleCharacter>& enemyTeamRef = caster->battleId > 0 ? enemyTeam_ : userTeam_;
    
    switch (type) {
        // ===== 基础目标 =====
        case TargetType::SELF:
            targets.push_back(caster);
            break;
            
        case TargetType::ALLY_SINGLE: {
            BattleCharacter* t = selectRandomAlive(allyTeam);
            if (t) targets.push_back(t);
            break;
        }
            
        case TargetType::ALLY_ALL:
            for (auto& ch : allyTeam) {
                if (ch.isAlive) targets.push_back(&ch);
            }
            break;
            
        case TargetType::ENEMY_SINGLE: {
            BattleCharacter* t = selectEnemyTarget(caster, enemyTeamRef);
            if (t) targets.push_back(t);
            break;
        }
            
        case TargetType::ENEMY_ALL:
            for (auto& ch : enemyTeamRef) {
                if (ch.isAlive) targets.push_back(&ch);
            }
            break;
        
        // ===== 前后排 =====
        case TargetType::ALLY_FRONT_ROW:
            for (auto& ch : allyTeam) {
                if (ch.isAlive && ch.isInFrontRow()) targets.push_back(&ch);
            }
            break;
            
        case TargetType::ALLY_BACK_ROW:
            for (auto& ch : allyTeam) {
                if (ch.isAlive && !ch.isInFrontRow()) targets.push_back(&ch);
            }
            break;
            
        case TargetType::ENEMY_FRONT_ROW:
            for (auto& ch : enemyTeamRef) {
                if (ch.isAlive && ch.isInFrontRow()) targets.push_back(&ch);
            }
            break;
            
        case TargetType::ENEMY_BACK_ROW:
            for (auto& ch : enemyTeamRef) {
                if (ch.isAlive && !ch.isInFrontRow()) targets.push_back(&ch);
            }
            break;
        
        // ===== 按攻击力排序 =====
        case TargetType::ALLY_ATK_TOP1:
        case TargetType::ALLY_ATK_TOP2:
        case TargetType::ALLY_ATK_TOP3: {
            int count = static_cast<int>(type) - static_cast<int>(TargetType::ALLY_ATK_TOP1) + 1;
            targets = selectByAttr(allyTeam, count, true, true);
            break;
        }
        
        case TargetType::ENEMY_ATK_TOP1:
        case TargetType::ENEMY_ATK_TOP2:
        case TargetType::ENEMY_ATK_TOP3: {
            int count = static_cast<int>(type) - static_cast<int>(TargetType::ENEMY_ATK_TOP1) + 1;
            targets = selectByAttr(enemyTeamRef, count, true, true);
            break;
        }
        
        // ===== 按血量排序 =====
        case TargetType::ALLY_HP_LOW1:
        case TargetType::ALLY_HP_LOW2:
        case TargetType::ALLY_HP_LOW3: {
            int count = static_cast<int>(type) - static_cast<int>(TargetType::ALLY_HP_LOW1) + 1;
            targets = selectByAttr(allyTeam, count, false, false);
            break;
        }
        
        case TargetType::ENEMY_HP_LOW1:
        case TargetType::ENEMY_HP_LOW2:
        case TargetType::ENEMY_HP_LOW3: {
            int count = static_cast<int>(type) - static_cast<int>(TargetType::ENEMY_HP_LOW1) + 1;
            targets = selectByAttr(enemyTeamRef, count, false, false);
            break;
        }
        
        // ===== 随机目标 =====
        case TargetType::ALLY_RANDOM_1:
        case TargetType::ALLY_RANDOM_2:
        case TargetType::ALLY_RANDOM_3: {
            int count = static_cast<int>(type) - static_cast<int>(TargetType::ALLY_RANDOM_1) + 1;
            targets = selectRandom(allyTeam, count);
            break;
        }
        
        case TargetType::ENEMY_RANDOM_1:
        case TargetType::ENEMY_RANDOM_2:
        case TargetType::ENEMY_RANDOM_3: {
            int count = static_cast<int>(type) - static_cast<int>(TargetType::ENEMY_RANDOM_1) + 1;
            targets = selectRandom(enemyTeamRef, count);
            break;
        }
        
        default:
            break;
    }
    
    return targets;
}

BattleCharacter* BattleManager::selectEnemyTarget(BattleCharacter* caster,
                                                   std::vector<BattleCharacter>& enemies) {
    // 检查嘲讽，如果为真则必须只攻击嘲讽者
    for (const Buff& b : caster->buffs) {
        if (b.type == EffectType::TAUNT) {
            for (auto& ch : enemies) {
                if (ch.battleId == b.value && ch.isAlive) {
                    return &ch;
                }
            }
        }
    }
    // 优先选择同Id的敌人，否则随机
    if (enemies[abs(caster->battleId)].isAlive) {
        return &enemies[abs(caster->battleId)];
    } else {
        return selectRandomAlive(enemies);
    }
    
    
    return nullptr;
}

BattleCharacter* BattleManager::selectRandomAlive(std::vector<BattleCharacter>& team)
{
    std::vector<BattleCharacter*> alive;
    for (auto& ch : team) {
        if (ch.isAlive) {
            alive.push_back(&ch);
        }
    }
    
    if (alive.empty()) {
        return nullptr;
    }
    
    std::uniform_int_distribution<size_t> dist(0, alive.size() - 1);
    return alive[dist(rng_)];
}

std::vector<BattleCharacter*> BattleManager::selectRandom(std::vector<BattleCharacter>& team, 
                                                           int count) {
    std::vector<BattleCharacter*> alive;
    for (auto& ch : team) {
        if (ch.isAlive) {
            alive.push_back(&ch);
        }
    }
    
    // 打乱顺序
    std::shuffle(alive.begin(), alive.end(), rng_);
    
    // 截取指定数量
    if (static_cast<int>(alive.size()) > count) {
        alive.resize(count);
    }
    
    return alive;
}

std::vector<BattleCharacter*> BattleManager::selectByAttr(std::vector<BattleCharacter>& team,
    int count, bool byAtk, bool highest)
{
    std::vector<BattleCharacter*> alive;
    for (auto& ch : team) {
        if (ch.isAlive) {
            alive.push_back(&ch);
        }
    }
    
    // 排序
    if (byAtk) {
        std::sort(alive.begin(), alive.end(),
            [highest](BattleCharacter* a, BattleCharacter* b) {
                return highest ? 
                    (a->currentAttr.atk > b->currentAttr.atk) :
                    (a->currentAttr.atk < b->currentAttr.atk);
            });
    } else {
        // 按当前HP排序
        std::sort(alive.begin(), alive.end(),
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
void BattleManager::triggerSkills(SkillTrigger trigger) {
    // 收集所有存活角色
    std::vector<BattleCharacter*> allChars;
    
    for (auto& ch : userTeam_) {
        if (ch.isAlive) {
            allChars.push_back(&ch);
        }
    }
    for (auto& ch : enemyTeam_) {
        if (ch.isAlive) {
            allChars.push_back(&ch);
        }
    }
    
    // 按速度排序（速度高的先触发）
    std::sort(allChars.begin(), allChars.end(),
        [](BattleCharacter* a, BattleCharacter* b) {
            return a->currentAttr.speed > b->currentAttr.speed;
        });
    
    // 依次检查并触发技能
    for (BattleCharacter* ch : allChars) {
        if (!ch->isAlive) {
            continue;
        }
        
        Skill* skill = ch->getSkill(trigger);
        if (skill && skill->id != 0) {
            LOG_INFO("%s 触发 [%s]", ch->name.c_str(), skill->name.c_str());
            
            for (const SkillEffect& effect : skill->effects) {
                executeEffect(ch, effect, skill->id);
            }
        }
    }
}

void BattleManager::triggerOnHit(BattleCharacter* defender, BattleCharacter* attacker)
{
    if (!defender || !defender->isAlive) {
        return;
    }
    
    Skill* skill = defender->getSkill(SkillTrigger::ON_HIT);
    if (skill && skill->id != 0) {
        
        for (const SkillEffect& effect : skill->effects) {
            executeEffect(defender, effect, skill->id);
        }
    }
}

void BattleManager::triggerOnLowHp(BattleCharacter* character) {
    if (!character || !character->isAlive) {
        return;
    }
    
    // 检查血量是否低于30%
    int64_t hpPercent = character->currentAttr.hp * 100 / character->currentAttr.maxHp;
    if (hpPercent > 30) {
        return;
    }
    
    Skill* skill = character->getSkill(SkillTrigger::ON_LOW_HP);
    if (skill && skill->id != 0) {   
        for (const SkillEffect& effect : skill->effects) {
            executeEffect(character, effect, skill->id);
        }
    }
}

void BattleManager::triggerOnDeath(BattleCharacter* character)
{
    if (!character) {
        return;
    }
    
    Skill* skill = character->getSkill(SkillTrigger::ON_DEATH);
    if (skill && skill->id != 0) {
        LOG_INFO("%s 触发阵亡技能 [%s]", character->name.c_str(), skill->name.c_str());
        
        // 阵亡技能特殊处理：即使死亡也能释放
        for (const SkillEffect& effect : skill->effects) {
            executeEffect(character, effect, skill->id);
        }
    }
}

// ==================== 状态检查 ====================
BattleManager::Result BattleManager::checkBattleResult() {
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

void BattleManager::checkDeaths() {
    // 检查己方死亡
    for (auto& ch : userTeam_) {
        if (ch.currentAttr.hp <= 0 && ch.isAlive) {
            ch.isAlive = false;
            ch.currentAttr.hp = 0;
            LOG_INFO("%s 阵亡！", ch.name.c_str());
            triggerOnDeath(&ch);
        }
    }
    
    // 检查敌方死亡
    for (auto& ch : enemyTeam_) {
        if (ch.currentAttr.hp <= 0 && ch.isAlive) {
            ch.isAlive = false;
            ch.currentAttr.hp = 0;
            triggerOnDeath(&ch);
        }
    }
}

bool BattleManager::isTeamAlive(const std::vector<BattleCharacter>& team) const {
    for (const auto& ch : team) {
        if (ch.isAlive) {
            return true;
        }
    }
    return false;
}

int BattleManager::countAlive(const std::vector<BattleCharacter>& team) const {
    int count = 0;
    for (const auto& ch : team) {
        if (ch.isAlive) {
            count++;
        }
    }
    return count;
}

// ==================== 工具函数 ====================

std::string BattleManager::getEffectName(EffectType type) const {
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
        // 补充于此
        case EffectType::STUN:          return "眩晕";
        case EffectType::SILENCE:       return "沉默";
        case EffectType::FREEZE:        return "冰冻";
        case EffectType::POISON:        return "中毒";
        case EffectType::BURN:          return "灼烧";
        case EffectType::BLEED:         return "流血";
        case EffectType::SHIELD:        return "护盾";
        case EffectType::TAUNT:         return "嘲讽";
        case EffectType::REVIVE:        return "复活";
        default:                        return "未知效果";
    }
}

bool BattleManager::rollChance(int percent) {
    if (percent <= 0) return false;
    if (percent >= 100) return true;
    
    std::uniform_int_distribution<int> dist(1, 100);
    return dist(rng_) <= percent;
}

void BattleManager::setLogCallback(LogCallback callback)
{
    logCallback_ = callback;
}
