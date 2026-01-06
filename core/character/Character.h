#pragma once

#include <string>
#include <vector>
class Character {
public:
    Character() = default;
    Character(int id, const std::string& name, const std::string& description,
              int level, int stars, long long hp, long long attack,
              long long phyDefense, long long magDefense, int speed,
              const std::vector<int>& skills)
        : id(id), name(name), description(description),
          level(level), stars(stars), hp(hp), attack(attack),
          phyDefense(phyDefense), magDefense(magDefense),
          speed(speed), skills(skills) {}
    ~Character() = default;
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
    std::vector<int> equipment;
};