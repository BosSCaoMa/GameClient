#include "BattleAttr.h"
#include <vector>

using namespace std;
BattleAttr BattleAttr::warriorAttrs()
{
    BattleAttr attrs(15000, 1200, 200,350);
    return attrs;
}

BattleAttr BattleAttr::mageAttrs()
{
    BattleAttr attrs(10000, 1500, 80, 300);
    return attrs;
}

BattleAttr BattleAttr::tankAttrs()
{
    BattleAttr attrs(20000, 500, 500, 280);
    return attrs;
}

BattleAttr BattleAttr::healerAttrs()
{
    BattleAttr attrs(12000, 800, 100, 320);
    return attrs;
}

BattleAttr BattleAttr::assassinAttrs()
{
    BattleAttr attrs(9000, 1600, 70, 400);
    return attrs;
}

void BattleAttr::ExPandByQuality(QualityType quality)
{
    vector<int> expandRates = {0, 15, 40, 80, 100, 150, 200, 300}; // 从白色到金色的扩展百分比
    int rate = expandRates[static_cast<int>(quality)];
    hp += hp * rate / 100;
    maxHp += maxHp * rate / 100;
    atk += atk * rate / 100;
    def += def * rate / 100;
    speed += speed * rate / 100;
}

void BattleAttr::upgradeByLevel(int level)
{
    int rate = 3; // 每级提升百分比
    hp += hp * rate / 100 * level;
    maxHp += maxHp * rate / 100 * level;
    atk += atk * rate / 100 * level;
    def += def * rate / 100 * level;
    speed += speed * rate / 100 * level;
}