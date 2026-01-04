#pragma once

#include <map>

class Package {
public:
    Package(int maxCapacity = 20) 
        : capacity(maxCapacity), currentSize(0) {}
    
    ~Package() = default;
    
    // 添加物品
    bool addItem(int itemId, int count = 1);
    
    // 移除物品
    bool removeItem(int itemId, int count = 1);
    
    // 获取物品数量
    int getItemCount(int itemId) const;
    
    // 检查是否有某物品
    bool hasItem(int itemId) const;
    
    // 检查是否已满
    bool isFull() const { return currentSize >= capacity; }
    
    // 获取剩余空间
    int getRemainingSpace() const { return capacity - currentSize; }
    
    // 获取所有物品
    const std::map<int, int>& getItems() const { return items; }
    
private:
    int capacity;      // 包裹容量
    int currentSize;   // 当前已使用空间
    std::map<int, int> items; // 物品ID -> 数量
};
