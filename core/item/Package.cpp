// Package.cpp
#include "Package.h"

bool Package::addItem(int itemId, int count) {
    if (count <= 0) return false;
    
    // 检查容量（这里假设每个物品占1格，可根据需求调整）
    if (currentSize + count > capacity) {
        return false;
    }
    
    items[itemId] += count;
    currentSize += count;
    return true;
}

bool Package::removeItem(int itemId, int count) {
    if (count <= 0) return false;
    
    auto it = items.find(itemId);
    if (it == items.end() || it->second < count) {
        return false; // 物品不存在或数量不足
    }
    
    it->second -= count;
    currentSize -= count;
    
    // 如果数量为0，从map中移除
    if (it->second == 0) {
        items.erase(it);
    }
    
    return true;
}

int Package::getItemCount(int itemId) const {
    auto it = items.find(itemId);
    return it != items.end() ? it->second : 0;
}

bool Package::hasItem(int itemId) const {
    return items.find(itemId) != items.end();
}
