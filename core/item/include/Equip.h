#pragma once

/*装备、坐骑、宠物等等*/
#include <string>
#include <map>
#include <vector>

class Equip {
public:
    Equip() = default;
    Equip(int id, int needType, int price, const std::string& name, const std::string& description,
        std::map<int, int> attributes, std::vector<int> skills)
        : id(id), needType(needType), price(price), name(name), description(description),
            attributes(attributes), skills(skills) {} ;
    ~Equip() = default;

private:
    int id;

    int equipType;
    int grade;
    int quality; // 品质: 0-不涉及，1-普通，2-稀有，3-史诗，4-传说

    int needType;
    int price;
    std::string name;
    std::string description;
    std::map<int, int> attributes;
    std::vector<int> skills;
};