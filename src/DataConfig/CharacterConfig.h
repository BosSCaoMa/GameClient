#pragma once

#include "Character.h"
#include "SkillConfig.h"
#include "BattleAttr.h"
#include "BattleTypes.h"
#include <unordered_map>
#include <string>
#include <vector>

// ==================== 武将模板 ====================
struct CharacterTemplate {
    int id;
    std::string name;
    int quality;                    // 品质 1-5
    
    // ===== 属性成长 =====
    BattleAttr baseAttr;            // 1级基础属性
    BattleAttr growthAttr;          // 每级成长属性
    
    // ===== 技能 =====
    std::vector<int> skillIds;      // 技能ID列表 [普通攻击, 怒气技能, 开局技能...]
    
    // ===== 新增：经验/突破配置 ===== ⭐
    int64_t baseExp = 100;          // 1级升2级所需经验
    double expGrowthRate = 1.5;     // 经验成长系数（exp = baseExp * level^expGrowthRate）
    int maxBreakthrough = 10;       // 最大突破次数
    int levelPerBreakthrough = 10;  // 每次突破提升等级上限
    
    CharacterTemplate() : id(0), quality(1) {}
    
    CharacterTemplate(int id_, const std::string& name_, int quality_,
                      const BattleAttr& base_, const BattleAttr& growth_,
                      std::initializer_list<int> skills_)
        : id(id_), name(name_), quality(quality_)
        , baseAttr(base_), growthAttr(growth_)
        , skillIds(skills_) {}
};

// ==================== 武将配置管理器 ====================
class CharacterConfig {
public:
    static CharacterConfig& instance() {
        static CharacterConfig inst;
        return inst;
    }
    
    // 获取武将模板
    const CharacterTemplate* get(int charId) const {
        auto it = templates_.find(charId);
        return it != templates_.end() ? &it->second : nullptr;
    }
    
    // ===== 创建武将实例 ===== ⭐ 更新签名
    Character create(int charId, int level = 1, int star = 1, int breakthrough = 0) const;
    
    // ===== 新增：计算指定等级的属性 ===== ⭐
    BattleAttr calculateAttrForLevel(int charId, int level, int star = 1, int breakthrough = 0) const;
    
    // 注册武将模板
    void reg(const CharacterTemplate& tmpl) {
        templates_[tmpl.id] = tmpl;
    }
    
    // 获取所有武将ID
    std::vector<int> getAllIds() const;

private:
    CharacterConfig() { initCharacters(); }
    CharacterConfig(const CharacterConfig&) = delete;
    CharacterConfig& operator=(const CharacterConfig&) = delete;
    
    void initCharacters();
    
    std::unordered_map<int, CharacterTemplate> templates_;
};

// ==================== 便捷访问宏 ====================
#define GET_CHAR_TMPL(id) CharacterConfig::instance().get(id)
#define CREATE_CHAR(id, lv, star) CharacterConfig::instance().create(id, lv, star)
