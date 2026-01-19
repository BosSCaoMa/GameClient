#include "BattleManager.h"
#include "Player.h"
#include "Character.h"
#include <map>
#include <vector>

using namespace std;
BattleManager::BattleManager(Player* user, Player* ememy)
    : round(0), user(user), ememy(ememy)
{
    // 初始化己方和敌方武将
    userCharacters.reserve(6);
    ememyCharacters.reserve(6);
    userCharacters.push_back(user->character);
    for (int i : ememy->characterOrder) {
        ememyCharacters.push_back(&ememy->characters[i]);
    }
    for (int i : user->characterOrder) {
        userCharacters.push_back(&user->characters[i]);
    }
    // 武将列表：主角 0 -> 武将 1-5
    ememyCharacters.push_back(ememy->character);
}

BattleManager::Result BattleManager::StartRound()
{
    bool roundOne = true;
    while (!over) {
        GetCharacterActionPriority();
        if (roundOne) {
            roundOne = false;
            ReleaseBeginSkills();
            continue;
        }
        Result result = Fight();
        if (over) {
            return result;
        }
        round++;
    }
    if (round >= maxRounds) {
        // 平局处理
        return Result::DRAW;
    }
}

void BattleManager::GetCharacterActionPriority()
{
    actionPriority.clear();
    std::map<int, int> speedMap; // 速度 -> 编号
    int i = 1;
    for (const auto& chara : userCharacters) {
        speedMap[chara->battleAttr.speed] = i++;
    }
    i = -1;
    for (const auto& chara : ememyCharacters) {
        speedMap[chara->battleAttr.speed] = i--;
    }
    // map自动排序，速度高的在后，actionPriority
    for (const auto& [speed, id] : speedMap) {
        actionPriority.push_back(id);
    }
}

BattleManager::Result BattleManager::Fight()
{
    // 逐个执行行动
    for (auto iter = actionPriority.end(); iter >= actionPriority.begin(); --iter) {
        auto action = GetCharacterAction(*iter); // 获取本轮要发动什么技能
        vector<int> targets;
        for (const auto& skill : action) {
            targets.clear(); // 清空上次的目标
            GetCharacterActionTargets(skill.targetType, *iter, targets); // 获得实际目标
        }
        vector<int> targets;
        // 执行技能效果
        bool gameResult = DoAction(targets, action); // 核心战斗逻辑
        // 检查战斗结束条件
        if (gameResult) {
            over = true;
            if (/*己方胜利条件*/) {
                return Result::WIN;
            } else {
                return Result::LOSE;
            }
        }
    }
    return Result();
}

vector<SkillTTE> BattleManager::GetCharacterAction(int characterId)
{
    Character* character = nullptr;
    if (characterId > 0) {
        character = userCharacters[characterId - 1];
    } else {
        character = ememyCharacters[-characterId - 1];
    }
    vector<SkillTTE> curActions;
    if (character != nullptr && character->battleAttr.rage >= 4) {
        curActions.push_back(character->battleAttr.skills[1]); // 怒气技能
    } else {
        curActions.push_back(character->battleAttr.skills[0]); // 普攻
    }
    for (int i = 6; i < character->battleAttr.skills.size(); ++i) {
        // 判断普攻还是放技能
        curActions.push_back(character->battleAttr.skills[i]);
    }
    return curActions;
}

void BattleManager::GetCharacterActionTargets(TargetType targetType, int character, std::vector<int> &targets)
{

}











// 武将阵亡触发判断
bool BattleManager::CheckBattleOver()
{
    for (const auto& chara : userCharacters) {
        if (chara->battleAttr.hp > 0) {
            return false; // 己方还有存活武将
        }
    }
    for (const auto& chara : ememyCharacters) {
        if (chara->battleAttr.hp > 0) {
            return false; // 敌方还有存活武将
        }
    }
    return true;
}
