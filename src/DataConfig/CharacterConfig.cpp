#include "CharacterConfig.h"

void CharacterConfig::initCharacters() {
    // ==================== 属性说明 ====================
    // BattleAttr(hp, atk, def, speed)
    // 其他属性使用默认值，或单独设置
    
    // ==================== 蜀国武将 (1000-1099) ====================
    
    reg(CharacterTemplate(
        1001, "关羽", 5,
        BattleAttr(5000, 350, 200, 100),    // 1级基础属性
        BattleAttr(500, 35, 20, 2),         // 每级成长
        {1, 102, 601}                       // 普攻, 旋风斩, 战意(被动)
    ));
    
    reg(CharacterTemplate(
        1002, "张飞", 5,
        BattleAttr(6000, 300, 250, 90),
        BattleAttr(600, 30, 25, 2),
        {1, 103, 304, 602}                  // 普攻, 雷霆一击, 嘲讽, 铁甲(被动)
    ));
    
    reg(CharacterTemplate(
        1003, "赵云", 5,
        BattleAttr(4500, 380, 180, 120),
        BattleAttr(450, 38, 18, 3),
        {1, 106, 605}                       // 普攻, 斩首, 死战不退(被动)
    ));
    
    reg(CharacterTemplate(
        1004, "诸葛亮", 5,
        BattleAttr(4000, 400, 150, 110),
        BattleAttr(400, 40, 15, 2),
        {1, 502, 301}                       // 普攻, 烈焰, 战鼓
    ));
    
    reg(CharacterTemplate(
        1005, "黄忠", 4,
        BattleAttr(4200, 360, 160, 95),
        BattleAttr(420, 36, 16, 2),
        {1, 104, 601}                       // 普攻, 穿云箭, 战意(被动)
    ));
    
    reg(CharacterTemplate(
        1006, "马超", 4,
        BattleAttr(4800, 340, 190, 115),
        BattleAttr(480, 34, 19, 3),
        {1, 108, 603}                       // 普攻, 暴风突刺, 迅捷(被动)
    ));
    
    // ==================== 魏国武将 (1100-1199) ====================
    
    reg(CharacterTemplate(
        1101, "曹操", 5,
        BattleAttr(5500, 320, 220, 105),
        BattleAttr(550, 32, 22, 2),
        {1, 301, 702}                       // 普攻, 战鼓, 怒气燃烧
    ));
    
    reg(CharacterTemplate(
        1102, "典韦", 5,
        BattleAttr(6500, 280, 280, 85),
        BattleAttr(650, 28, 28, 1),
        {1, 105, 304, 607}                  // 普攻, 横扫千军, 嘲讽, 同归于尽
    ));
    
    reg(CharacterTemplate(
        1103, "许褚", 5,
        BattleAttr(7000, 260, 300, 80),
        BattleAttr(700, 26, 30, 1),
        {1, 103, 602}                       // 普攻, 雷霆一击, 铁甲(被动)
    ));
    
    reg(CharacterTemplate(
        1104, "司马懿", 5,
        BattleAttr(4200, 380, 160, 108),
        BattleAttr(420, 38, 16, 2),
        {1, 402, 703}                       // 普攻, 沉默, 夺气
    ));
    
    reg(CharacterTemplate(
        1105, "张辽", 4,
        BattleAttr(4600, 350, 200, 112),
        BattleAttr(460, 35, 20, 3),
        {1, 107, 603}                       // 普攻, 连环击, 迅捷(被动)
    ));
    
    // ==================== 吴国武将 (1200-1299) ====================
    
    reg(CharacterTemplate(
        1201, "孙权", 5,
        BattleAttr(5200, 300, 210, 100),
        BattleAttr(520, 30, 21, 2),
        {1, 302, 305}                       // 普攻, 铁壁, 护盾
    ));
    
    reg(CharacterTemplate(
        1202, "周瑜", 5,
        BattleAttr(4000, 420, 140, 115),
        BattleAttr(400, 42, 14, 2),
        {1, 502, 401}                       // 普攻, 烈焰, 冰冻术
    ));
    
    reg(CharacterTemplate(
        1203, "吕蒙", 4,
        BattleAttr(4400, 340, 190, 105),
        BattleAttr(440, 34, 19, 2),
        {1, 501, 606}                       // 普攻, 毒刃, 反击(被动)
    ));
    
    reg(CharacterTemplate(
        1204, "陆逊", 4,
        BattleAttr(3800, 400, 130, 118),
        BattleAttr(380, 40, 13, 3),
        {1, 502, 603}                       // 普攻, 烈焰, 迅捷(被动)
    ));
    
    // ==================== 群雄武将 (1300-1399) ====================
    
    reg(CharacterTemplate(
        1301, "吕布", 5,
        BattleAttr(5000, 450, 180, 100),
        BattleAttr(500, 45, 18, 2),
        {1, 101, 605, 601}                  // 普攻, 破甲斩, 死战不退, 战意
    ));
    
    reg(CharacterTemplate(
        1302, "貂蝉", 5,
        BattleAttr(3500, 380, 120, 125),
        BattleAttr(350, 38, 12, 3),
        {1, 403, 402}                       // 普攻, 群体眩晕, 沉默
    ));
    
    reg(CharacterTemplate(
        1303, "董卓", 4,
        BattleAttr(6000, 280, 260, 75),
        BattleAttr(600, 28, 26, 1),
        {1, 304, 602}                       // 普攻, 嘲讽, 铁甲(被动)
    ));
    
    reg(CharacterTemplate(
        1304, "华佗", 5,
        BattleAttr(4000, 200, 180, 110),
        BattleAttr(400, 20, 18, 2),
        {1, 201, 202, 701}                  // 普攻, 甘霖普降, 妙手回春, 复活术
    ));
    
    reg(CharacterTemplate(
        1305, "张角", 4,
        BattleAttr(4200, 360, 150, 105),
        BattleAttr(420, 36, 15, 2),
        {1, 503, 604}                       // 普攻, 裂伤, 回合恢复(被动)
    ));
}

Character CharacterConfig::create(int charId, int level = 1, int star = 1) const {
    const auto* tmpl = get(charId);
    if (!tmpl) {
        return Character(0, "未知武将", 1, 1);
    }
    
    Character ch(tmpl->id, tmpl->name, level, star);
    
    // 计算属性：基础 + 成长 * (等级-1)
    ch.baseAttr.hp = tmpl->baseAttr.hp + tmpl->growthAttr.hp * (level - 1);
    ch.baseAttr.atk = tmpl->baseAttr.atk + tmpl->growthAttr.atk * (level - 1);
    ch.baseAttr.def = tmpl->baseAttr.def + tmpl->growthAttr.def * (level - 1);
    ch.baseAttr.speed = tmpl->baseAttr.speed + tmpl->growthAttr.speed * (level - 1);
    ch.baseAttr.maxHp = ch.baseAttr.hp;
    
    // 复制其他属性
    ch.baseAttr.critRate = tmpl->baseAttr.critRate;
    ch.baseAttr.critDamage = tmpl->baseAttr.critDamage;
    ch.baseAttr.hitRate = tmpl->baseAttr.hitRate;
    ch.baseAttr.dodgeRate = tmpl->baseAttr.dodgeRate;
    
    // 配置技能
    for (size_t i = 0; i < tmpl->skillIds.size() && i < 6; ++i) {
        int skillId = tmpl->skillIds[i];
        if (skillId > 0) {
            Skill skill = SKILL(skillId);
            ch.setSkill(skill.trigger, skill);
        }
    }
    
    return ch;
}

std::vector<int> CharacterConfig::getAllIds() const {
    std::vector<int> ids;
    for (const auto& pair : templates_) {
        ids.push_back(pair.first);
    }
    return ids;
}