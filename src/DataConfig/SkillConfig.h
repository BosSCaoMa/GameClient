#pragma once

#include "Skill.h"
#include <unordered_map>
#include <functional>

// ==================== 技能配置管理器 ====================
class SkillConfig {
public:
    // 单例访问
    static SkillConfig& instance() {
        static SkillConfig inst;
        return inst;
        
    }
    
    // 获取技能（返回副本）
    Skill get(int skillId) const {
        auto it = skills_.find(skillId);
        if (it != skills_.end()) {
            return it->second;
        }
        return Skill(0, "未知技能", SkillTrigger::None);
    }
    
    // 检查技能是否存在
    bool has(int skillId) const {
        return skills_.find(skillId) != skills_.end();
    }
    
    // 注册技能
    void reg(const Skill& skill) {
        skills_[skill.id] = skill;
    }

private:
    SkillConfig() { initSkills(); }
    SkillConfig(const SkillConfig&) = delete;
    SkillConfig& operator=(const SkillConfig&) = delete;
    
    void initSkills();
    
    std::unordered_map<int, Skill> skills_;
};

// ==================== 便捷访问宏 ====================
#define GET_SKILL(id) SkillConfig::instance().get(id)
