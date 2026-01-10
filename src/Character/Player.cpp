#include "Player.h"
#include "Character.h"  // 必须包含，因为要访问 Character 的 baseAttr 成员

// 构造函数实现
Player::Player() {}

// 修改属性值
void Player::modifyAttribute(PlayerAttrType type, int value) {
    attributes[type] += value;
}

// 获取属性值（const 函数保持 const 修饰）
int Player::getAttribute(PlayerAttrType type) const {
    auto it = attributes.find(type);
    return it != attributes.end() ? it->second : 0;
}

// 更新总战力
void Player::updateCombatPower() {
    combatPower = 0;
    if (character) {
        combatPower += calculateCharacterPower(character);
    }
    for (int id : characterOrder) {
        auto it = characters.find(id);
        if (it != characters.end()) {
            combatPower += calculateCharacterPower(&it->second);
        }
    }
}

// 存档到服务器（TODO 待实现）
bool Player::saveDataToServer() {
    // TODO: 实现存档逻辑
    return true;
}

// 计算单个角色的战力（私有函数）
uint64_t Player::calculateCharacterPower(Character* ch) {
    const BattleAttr& attr = ch->baseAttr;
    // 简单战力公式
    return static_cast<uint64_t>(
        attr.maxHp / 10 + 
        attr.atk * 5 + 
        attr.defence * 3 + 
        attr.speed * 2
    );
}

// ------------战斗相关-----------------------
// 获取上阵角色列表（包含主角）
std::vector<Character*> Player::getBattleTeam() {
    std::vector<Character*> team;
    if (character) {
        team.push_back(character);
    }
    for (int id : characterOrder) {
        auto it = characters.find(id);
        if (it != characters.end()) {
            team.push_back(&it->second);
        }
    }
    return team;
}