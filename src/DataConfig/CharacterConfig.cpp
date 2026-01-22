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
    addHero(2006, "黄忠", QualityType::ORANGE, Position::ASSASSIN, {2016, 2017, 2018});
    addHero(2007, "马超", QualityType::ORANGE, Position::WARRIOR, {2019, 2020, 2021});
    addHero(2008, "庞统", QualityType::ORANGE, Position::MAGE, {2022, 2023, 2024});
    addHero(2009, "姜维", QualityType::ORANGE, Position::MAGE, {2025, 2026, 2027});
    addHero(2010, "魏延", QualityType::ORANGE, Position::WARRIOR, {2028, 2029, 2030});
    addHero(2011, "张苞", QualityType::ORANGE, Position::TANK, {2031, 2032, 2033});
    addHero(2012, "关银屏", QualityType::ORANGE, Position::ASSASSIN, {2034, 2035, 2036});
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
    addHero(3004, "甘宁", QualityType::ORANGE, Position::ASSASSIN, {1110, 1111, 1112});
    addHero(3005, "吕蒙", QualityType::ORANGE, Position::MAGE, {1113, 1114, 1115});
    addHero(3006, "陆逊", QualityType::ORANGE, Position::MAGE, {1116, 1117, 1118});
    addHero(3007, "凌统", QualityType::ORANGE, Position::WARRIOR, {1119, 1120, 1121});
    addHero(3008, "丁奉", QualityType::ORANGE, Position::TANK, {1122, 1123, 1124});
    addHero(3009, "大乔", QualityType::ORANGE, Position::HEALER, {1125, 1126, 1127});
    addHero(3010, "孙尚香", QualityType::ORANGE, Position::ASSASSIN, {1128, 1129, 1130});
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
    addHero(3104, "张辽", QualityType::ORANGE, Position::ASSASSIN, {3010, 3011, 3012});
    addHero(3105, "许褚", QualityType::ORANGE, Position::TANK, {3013, 3014, 3015});
    addHero(3106, "夏侯惇", QualityType::ORANGE, Position::WARRIOR, {3016, 3017, 3018});
    addHero(3107, "夏侯渊", QualityType::ORANGE, Position::ASSASSIN, {3019, 3020, 3021});
    addHero(3108, "郭嘉", QualityType::ORANGE, Position::MAGE, {3022, 3023, 3024});
    addHero(3109, "张郃", QualityType::ORANGE, Position::WARRIOR, {3025, 3026, 3027});
    addHero(3110, "于禁", QualityType::ORANGE, Position::TANK, {3028, 3029, 3030});
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
    addHero(4004, "贾诩", QualityType::ORANGE, Position::MAGE, {4016, 4017, 4018});
    addHero(4005, "华雄", QualityType::ORANGE, Position::WARRIOR, {4019, 4020, 4021});
    addHero(4006, "董卓", QualityType::ORANGE, Position::TANK, {4022, 4023, 4024});
    addHero(4007, "貂蝉", QualityType::ORANGE, Position::ASSASSIN, {4025, 4026, 4027});
    addHero(4008, "公孙瓒", QualityType::ORANGE, Position::WARRIOR, {4028, 4029, 4030});
    addHero(4009, "颜良", QualityType::ORANGE, Position::WARRIOR, {4031, 4032, 4033});
    addHero(4010, "文丑", QualityType::ORANGE, Position::WARRIOR, {4034, 4035, 4036});
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

