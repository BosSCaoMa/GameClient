#include "CharacterConfig.h"
#include "SkillConfig.h"
#include <cmath>
#include <algorithm>

// ==================== 创建武将实例 ====================
Character CharacterConfig::create(int charId, int level, int star, int breakthrough) const
{
    const auto* tmpl = get(charId);
    if (!tmpl) {
        return Character();  // 返回空武将
    }
    
    // 1. 创建基础角色
    Character ch(charId, tmpl->name, level, star);
    ch.quality = tmpl->quality;
    ch.breakthrough = breakthrough;
    
    // 2. 计算属性
    ch.originAttr = tmpl->baseAttr;
    ch.baseAttr = ch.originAttr;  // 初始时，当前属性 = 基础属性
    
    // 3. 初始化技能
    for (size_t i = 0; i < tmpl->skillIds.size(); ++i) {
        int skillId = tmpl->skillIds[i];
        Skill aSkill = GET_SKILL(skillId);
        if (aSkill.id != 0) { // 确保技能存在
            SkillTrigger trigger = static_cast<SkillTrigger>(i); // 按顺序映射触发类型
            ch.setSkill(trigger, aSkill);
        }
    }
    
    return ch;
}

// ==================== 计算指定等级的属性 ====================
BattleAttr CharacterConfig::calculateAttrForLevel(int charId, int level, int star, int breakthrough) const {
    const auto* tmpl = get(charId);
    if (!tmpl) {
        return BattleAttr();
    }
    
    BattleAttr attr = tmpl->baseAttr;
    
    // 1. 等级成长
    if (level > 1) {
        attr.hp += tmpl->growthAttr.hp * (level - 1);
        attr.maxHp += tmpl->growthAttr.maxHp * (level - 1);
        attr.atk += tmpl->growthAttr.atk * (level - 1);
        attr.def += tmpl->growthAttr.def * (level - 1);
        attr.speed += tmpl->growthAttr.speed * (level - 1);
        
        // 扩展属性成长
        attr.critRate += tmpl->growthAttr.critRate * (level - 1);
        attr.critDamage += tmpl->growthAttr.critDamage * (level - 1);
    }
    
    // 2. 星级加成（每星 +10%）
    if (star > 1) {
        int starBonus = (star - 1) * 10;  // 10% per star
        attr.hp += tmpl->baseAttr.hp * starBonus / 100;
        attr.maxHp += tmpl->baseAttr.maxHp * starBonus / 100;
        attr.atk += tmpl->baseAttr.atk * starBonus / 100;
        attr.def += tmpl->baseAttr          .def * starBonus / 100;
    }
    
    // 3. 突破加成（每次突破 +5%）
    if (breakthrough > 0) {
        int breakthroughBonus = breakthrough * 5;
        attr.hp += tmpl->baseAttr.hp * breakthroughBonus / 100;
        attr.maxHp += tmpl->baseAttr.maxHp * breakthroughBonus / 100;
        attr.atk += tmpl->baseAttr.atk * breakthroughBonus / 100;
        attr.def += tmpl->baseAttr.def * breakthroughBonus / 100;
    }
    
    return attr;
}

// ==================== 获取所有武将ID ====================
std::vector<int> CharacterConfig::getAllIds() const {
    std::vector<int> ids;
    ids.reserve(templates_.size());
    for (const auto& [id, _] : templates_) {
        ids.push_back(id);
    }
    return ids;
}

// ==================== 预定义属性模板 ====================
namespace AttrTemplates {
    // 力量型（高攻击、中防御）
    const BattleAttr STRENGTH_BASE(5000, 350, 180, 95);
    const BattleAttr STRENGTH_GROWTH(500, 42, 20, 1);
    
    // 坦克型（高生命、高防御）
    const BattleAttr TANK_BASE(6000, 320, 220, 85);
    const BattleAttr TANK_GROWTH(600, 38, 25, 1);
    
    // 敏捷型（高速度、高暴击）
    const BattleAttr AGILE_BASE(5200, 360, 170, 100);
    const BattleAttr AGILE_GROWTH(520, 45, 18, 2);
    
    // 法师型（高攻击、低防御、低速度）
    const BattleAttr MAGE_BASE(4500, 380, 150, 80);
    const BattleAttr MAGE_GROWTH(450, 48, 15, 1);
    
    // 辅助型（中等全面）
    const BattleAttr SUPPORT_BASE(5500, 280, 200, 90);
    const BattleAttr SUPPORT_GROWTH(550, 32, 22, 1);
}

void CharacterConfig::initCharacters() {
    using namespace AttrTemplates;
    
    // ===== 关羽（力量型） =====
    {
        CharacterTemplate tmpl(
            1001, "关羽", 5,
            STRENGTH_BASE,      // ⭐ 使用预定义模板
            STRENGTH_GROWTH,
            {2001, 2002, 2003}
        );
        
        // 个性化调整（可选）
        tmpl.baseAttr.critRate = 20;
        tmpl.baseAttr.critDamage = 150;
        
        reg(tmpl);
    }
    
    // ===== 张飞（坦克型） =====
    {
        CharacterTemplate tmpl(
            1002, "张飞", 5,
            TANK_BASE,          // ⭐ 使用预定义模板
            TANK_GROWTH,
            {2001, 2004, 2005}
        );
        
        tmpl.baseAttr.critRate = 15;
        tmpl.baseAttr.critDamage = 150;
        
        reg(tmpl);
    }
    
    // ===== 赵云（敏捷型） =====
    {
        CharacterTemplate tmpl(
            1003, "赵云", 5,
            AGILE_BASE,         // ⭐ 使用预定义模板
            AGILE_GROWTH,
            {2001, 2006, 2007}
        );
        
        tmpl.baseAttr.critRate = 25;
        tmpl.baseAttr.critDamage = 160;
        
        reg(tmpl);
    }
    
    // ===== 诸葛亮（法师型） =====
    {
        CharacterTemplate tmpl(
            1004, "诸葛亮", 5,
            MAGE_BASE,
            MAGE_GROWTH,
            {2001, 2008, 2009}
        );
        
        tmpl.baseAttr.critRate = 30;
        tmpl.baseAttr.critDamage = 180;
        tmpl.baseAttr.skillDamage = 20;  // 技能伤害加成
        
        reg(tmpl);
    }
}
