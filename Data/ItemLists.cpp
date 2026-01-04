#include "ItemLists.h"
#include <map>

using namespace std;

static const map<int, Item> itemDatabase = {
    {1, Item(1, 0, 100, "血瓶", "恢复30体力值", std::move(map<int, int>{{0, 50}}))},
    {2, Item(2, 0, 200, "口服液", "恢复魔法值30点", std::move(map<int, int>{{1, 30}}))}
    // 可以在这里添加更多物品
};

const Item* GetItemById(int itemId)
{
    auto it = itemDatabase.find(itemId);
    if (it != itemDatabase.end()) {
        return &(it->second);
    }
    return nullptr; // Return a default item if not found
}