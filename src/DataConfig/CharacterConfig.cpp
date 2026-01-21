#include "CharacterConfig.h"
#include "SkillConfig.h"
#include <cmath>
#include <algorithm>

// ==================== 创建武将实例 ====================
Character CharacterConfig::create(int charId) const
{
    const auto* tmpl = get(charId);
    if (!tmpl) {
        return Character();  // 返回空武将
    }
    
    // 1. 创建基础角色
    Character ch(charId, tmpl->name, tmpl->quality, tmpl->position);
    
    // 2. 计算属性
    ch.originAttr.InitAttr(ch.position, ch.quality); // 根据定位和品质初始化基础属性
    ch.baseAttr = ch.originAttr;  // 初始时，当前属性 = 基础属性
    
    // 3. 初始化技能
    for (size_t i = 0; i < tmpl->skillIds.size(); ++i) {
        int skillId = tmpl->skillIds[i];
        Skill aSkill = GET_SKILL(skillId);
        ch.setSkill(aSkill);
    }
    
    return ch;
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

void CharacterConfig::initCharacters()
{
    // ===== 关羽（力量型） =====
    {
        CharacterTemplate tmpl(
            1001, "关羽", QualityType::RED, Position::WARRIOR,
            {2001, 2002, 2003}
        );
        
        // 个性化调整（可选）
        
        reg(tmpl);
    }
    
    // ===== 张飞（坦克型） =====
    {
        CharacterTemplate tmpl(
            1002, "张飞", QualityType::RED, Position::TANK,
            {2001, 2004, 2005}
        );
        
        reg(tmpl);
    }
    
    // ===== 赵云（敏捷型） =====
    {
        CharacterTemplate tmpl(
            1003, "赵云", QualityType::RED, Position::ASSASSIN,
            {2001, 2006, 2007}
        );
        
        reg(tmpl);
    }
    
    // ===== 诸葛亮（法师型） =====
    {
        CharacterTemplate tmpl(
            1004, "诸葛亮", QualityType::RED, Position::MAGE,
            {2001, 2008, 2009}
        );
        
        reg(tmpl);
    }
}
