#include "BattleManager.h"
#include "Player.h"
#include "Character.h"
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
    while (!over) {
        GetCharacterActionPriority();
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
        speedMap[chara->speed] = i++;
    }
    i = -1;
    for (const auto& chara : ememyCharacters) {
        speedMap[chara->speed] = i--;
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
        // 做事情
        // if (IsGameOver()) {
        //     return 
        // }
    }
    return Result();
}
