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

struct BaseCharDef {
    int id;
    const char* name;
    QualityType quality;
    Position position;
    std::initializer_list<int> skills;
};

void CharacterConfig::initCharacters()
{
    initBaseCharacters(); // 初始化基础武将
    InitShuCharacters(); // 初始化蜀国武将
    InitWuCharacters(); // 初始化吴国武将
    InitWeiCharacters(); // 初始化魏国武将
    InitQunCharacters(); // 初始化群雄武将
    InitShenCharacters(); // 初始化神将
}

void CharacterConfig::InitShuCharacters()
{
    InitShuRedCharacters();
    InitShuOrangeCharacters();
    InitShuPurpleCharacters();
    InitShuBlueCharacters();
    InitShuGreenCharacters();
}

void CharacterConfig::addHero(int id, const char* name, QualityType quality, Position pos,
    std::initializer_list<int> skills)
{
    reg(CharacterTemplate(id, name, quality, pos, skills));
}

void CharacterConfig::InitWuCharacters()
{
    InitWuRedCharacters();
    InitWuOrangeCharacters();
    InitWuPurpleCharacters();
    InitWuBlueCharacters();
    InitWuGreenCharacters();
}

void CharacterConfig::InitWeiCharacters()
{
    InitWeiRedCharacters();
    InitWeiOrangeCharacters();
    InitWeiPurpleCharacters();
    InitWeiBlueCharacters();
    InitWeiGreenCharacters();
}

void CharacterConfig::InitQunCharacters()
{
    InitQunRedCharacters();
    InitQunOrangeCharacters();
    InitQunPurpleCharacters();
    InitQunBlueCharacters();
    InitQunGreenCharacters();
}

void CharacterConfig::InitShenCharacters()
{

}

// ==================== 具体武将定义 ====================
void CharacterConfig::initBaseCharacters()
{
    CharacterTemplate tmpl(
        1, "步兵", QualityType::WHITE, Position::WARRIOR, {1}
    );
    reg(tmpl);
    CharacterTemplate archerTmpl(
        2, "弓箭手", QualityType::WHITE, Position::ASSASSIN, {4}
    );
    reg(archerTmpl);
    CharacterTemplate mageTmpl(
        3, "法师", QualityType::WHITE, Position::MAGE, {2}
    );
    reg(mageTmpl);
    CharacterTemplate healerTmpl(
        4, "随军医师", QualityType::WHITE, Position::HEALER, {10}
    );
    reg(healerTmpl);
    CharacterTemplate tankTmpl(
        5, "盾卫", QualityType::WHITE, Position::TANK, {1}
    );
    reg(tankTmpl);

    CharacterTemplate baierbing(
        6, "白毦兵", QualityType::GREEN, Position::ASSASSIN, {4, 106}
    );
    reg(baierbing);

    CharacterTemplate hubaoqi(
        7, "虎豹骑", QualityType::GREEN, Position::WARRIOR, {3, 1002}
    );
    reg(hubaoqi);

    CharacterTemplate jiefanwei(
        8, "解烦卫", QualityType::GREEN, Position::HEALER, {10, 1003}
    );
    reg(jiefanwei);

    CharacterTemplate xianzhenying(
        9, "陷阵营", QualityType::GREEN, Position::TANK, {2, 1004}
    );
    reg(xianzhenying);
}

void CharacterConfig::InitShuRedCharacters()
{
    addHero(2001, "关羽", QualityType::RED, Position::WARRIOR, {2001, 2002, 2003});
    addHero(2002, "张飞", QualityType::RED, Position::TANK, {2004, 2005, 2006});
    addHero(2003, "赵云", QualityType::RED, Position::ASSASSIN, {2007, 2008, 2009});
    addHero(2004, "诸葛亮", QualityType::RED, Position::MAGE, {2010, 2011, 2012});
    addHero(2005, "刘备", QualityType::RED, Position::HEALER, {2013, 2014, 2015});
}

void CharacterConfig::InitShuOrangeCharacters()
{

}

void CharacterConfig::InitShuPurpleCharacters()
{

}

void CharacterConfig::InitShuBlueCharacters()
{

}

void CharacterConfig::InitShuGreenCharacters()
{

}

void CharacterConfig::InitWuRedCharacters()
{
    addHero(3001, "周瑜", QualityType::RED, Position::MAGE, {1101, 1102, 1103});
    addHero(3002, "孙策", QualityType::RED, Position::WARRIOR, {1104, 1105, 1106});
    addHero(3003, "孙权", QualityType::RED, Position::HEALER, {1107, 1108, 1109});
}

void CharacterConfig::InitWuOrangeCharacters()
{

}

void CharacterConfig::InitWuPurpleCharacters()
{

}

void CharacterConfig::InitWuBlueCharacters()
{

}

void CharacterConfig::InitWuGreenCharacters()
{

}

void CharacterConfig::InitWeiRedCharacters()
{
    addHero(3101, "荀彧", QualityType::RED, Position::HEALER, {3001, 3002, 3003});
    addHero(3102, "司马懿", QualityType::RED, Position::MAGE, {3004, 3005, 3006});
    addHero(3103, "曹操", QualityType::RED, Position::WARRIOR, {3007, 3008, 3009});
}

void CharacterConfig::InitWeiOrangeCharacters()
{

}

void CharacterConfig::InitWeiPurpleCharacters()
{

}

void CharacterConfig::InitWeiBlueCharacters()
{

}

void CharacterConfig::InitWeiGreenCharacters()
{

}

void CharacterConfig::InitQunRedCharacters()
{
    addHero(4001, "吕布", QualityType::RED, Position::ASSASSIN, {4001, 4002, 4003});
    addHero(4002, "左慈", QualityType::RED, Position::MAGE, {4010, 4011, 4012});
    addHero(4003, "袁绍", QualityType::RED, Position::WARRIOR, {4013, 4014, 4015});
}

void CharacterConfig::InitQunOrangeCharacters()
{

}

void CharacterConfig::InitQunPurpleCharacters()
{

}

void CharacterConfig::InitQunBlueCharacters()
{

}

void CharacterConfig::InitQunGreenCharacters()
{

}

