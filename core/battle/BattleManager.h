#pragma once

#include <vector>

class Player;
class Character;
class BattleManager {
enum class Result {
    WIN,
    LOSE,
    DRAW
};
public:
    BattleManager(Player* user, Player* ememy);
    BattleManager::Result StartRound();

private:
    int round;
    int maxRounds = 50;
    bool over = false;
    Player* user;
    Player* ememy;
    std::vector<Character*> userCharacters;
    std::vector<Character*> ememyCharacters;
    std::vector<int> actionPriority; // 己方编号为正，对方为负。每轮更新。速度低的在前

    void GetCharacterActionPriority();
    Result Fight();
};