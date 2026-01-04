#include "ItemInterface.h"
#include "Item.h"
#include "ItemLists.h"
int UseItem(int itemId)
{
    const Item* item = GetItemById(itemId);
    if (item == nullptr) {
        return -1; // 后续添加枚举
    }
    // item->use(player);
    return 0;
}