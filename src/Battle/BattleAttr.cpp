#include "BattleAttr.h"
#include <cstddef>
#include <cstdint>

namespace {
constexpr int kQualityExpandRates[] = {0, 15, 40, 80, 100, 150, 200, 300};
constexpr std::size_t kQualityExpandRatesCount = sizeof(kQualityExpandRates) / sizeof(kQualityExpandRates[0]);
constexpr int kLevelGrowthPercent = 3; // 每级统一成长百分比

constexpr int qualityRate(QualityType quality)
{
    const auto index = static_cast<std::size_t>(quality);
    return index < kQualityExpandRatesCount ? kQualityExpandRates[index] : kQualityExpandRates[0];
}
} // namespace
void BattleAttr::InitAttr(Position pos, QualityType quality)
{
    BattleAttr attrs;
    switch (pos) {
        case Position::WARRIOR:
            attrs = warriorAttrs();
            break;
        case Position::MAGE:
            attrs = mageAttrs();
            break;
        case Position::TANK:
            attrs = tankAttrs();
            break;
        case Position::HEALER:
            attrs = healerAttrs();
            break;
        case Position::ASSASSIN:
            attrs = assassinAttrs();
            break;
        default:
            attrs = BattleAttr(); // 默认属性
            break;
    }
    *this = attrs;
    ExPandByQuality(quality);
}
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
    const int rate = qualityRate(quality);
    auto scale = [rate](Scalar& value) {
        value += value * rate / 100;
    };

    scale(hp);
    scale(maxHp);
    scale(atk);
    scale(def);
    scale(speed);
}

void BattleAttr::upgradeByLevel(int level)
{
    hp += hp * kLevelGrowthPercent / 100 * level;
    maxHp += maxHp * kLevelGrowthPercent / 100 * level;
    atk += atk * kLevelGrowthPercent / 100 * level;
    def += def * kLevelGrowthPercent / 100 * level;
    speed += speed * kLevelGrowthPercent / 100 * level;
}

BattleAttr& BattleAttr::operator+=(const BattleAttr& other)
{
    hp += other.hp;
    maxHp += other.maxHp;
    atk += other.atk;
    def += other.def;
    speed += other.speed;

    critRate += other.critRate;
    critDamage += other.critDamage;
    critResist += other.critResist;
    hitRate += other.hitRate;
    dodgeRate += other.dodgeRate;
    rage += other.rage;

    damageBonus += other.damageBonus;
    damageReduction += other.damageReduction;
    skillDamage += other.skillDamage;

    lifesteal += other.lifesteal;
    counterRate += other.counterRate;
    multiHitRate += other.multiHitRate;
    healBonus += other.healBonus;

    for (std::uint8_t idx = 0; idx < static_cast<std::uint8_t>(Resistance::Count); ++idx) {
        const auto resistanceType = static_cast<Resistance>(idx);
        resistance(resistanceType) += other.getResistance(resistanceType);
    }
    return *this;
}

std::uint64_t BattleAttr::calculateCombatPower() const
{
    std::uint64_t power = 0;
    // todo
    return power;
}