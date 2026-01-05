#pragma once

#include <string>
#include <map>
#include "Player.h"
/*
所有使用类物品的基类
*/
class Item {
public:
    Item() = default;
    Item(int id, int needType, int price, const std::string& name, const std::string& description,
        const std::map<int, int>& attributes)
    : id(id), needType(needType), price(price), name(name), description(description), 
        attributes(attributes) {}
    virtual ~Item() = default;
    int use(Player& p) {
        for (const auto& attr : attributes) {
            p.modifyAttribute(attr.first, attr.second);
        }
        return 0;
    };
private:
    int id;
    int needType;
    int price;
    std::string name;
    std::string description;
    std::map<int, int> attributes; // 物品属性，如体力、精力等
};