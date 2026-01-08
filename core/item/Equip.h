#pragma once

/*装备、坐骑、宠物等等*/
#include <string>
#include <map>
#include <vector>

enum class EquipType {
    WEAPON = 1, // 武器
    ARMOR = 2, // 铠甲
    HELMET = 3, // 头盔
    BOOTS = 4, // 靴子
    MilitaryTally = 5, // 兵符
    FamousGeneral = 6, // 名将
    MOUNT = 7, // 坐骑
    PET = 8 // 宠物
};
class Equip {
    static constexpr int MAX_LEVEL = 100;
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
    int level;
    int quality; // 品质: 0-不涉及，1-普通，2-稀有，3-史诗，4-传说

    int needType;
    int price;
    std::string name;
    std::string description;
    std::map<int, int> attributes;
    std::vector<int> skills;
};