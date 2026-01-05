#pragma once

#include <string>
#include <vector>
class Character {

private:
    int id;
    std::string name;
    std::string description;

    int level;
    int stars;
    long long hp;
    long long attack;
    long long phyDefense;
    long long magDefense;
    int speed;
    std::vector<int> skills;
};