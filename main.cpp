#include <iostream>
#include <vector>
#include <string>

#include "Player.h"
#include "SkillConfig.h"
#include "CharacterConfig.h"
#include "BattleManager.h"
namespace {

Character createHero(int id,
					 const std::string& name,
					 int level,
					 const BattleAttr& attr,
					 int normalSkillId,
					 int rageSkillId,
					 std::initializer_list<int> extraSkills = {})
{
	Character hero(id, name, level, 1);
	hero.originAttr = attr;
	hero.baseAttr = attr;
	hero.setSkill(GET_SKILL(normalSkillId));
	hero.setSkill(GET_SKILL(rageSkillId));
	for (int skillId : extraSkills) {
		hero.setSkill(GET_SKILL(skillId));
	}
	hero.recalculateAttr();
	return hero;
}

void registerTeam(Player& player, const std::vector<Character>& roster)
{
	for (const auto& hero : roster) {
		if (!player.addCharacter(hero)) {
			continue;
		}
		player.addToBattleTeam(hero.id);
		if (!player.mainCharacter) {
			player.mainCharacter = player.getCharacter(hero.id);
		}
	}
}

std::string toString(BattleManager::Result result)
{
	switch (result) {
		case BattleManager::Result::WIN: return "胜利";
		case BattleManager::Result::LOSE: return "失败";
		case BattleManager::Result::DRAW: return "平局";
		default: return "进行中";
	}
}

} // namespace

int main()
{
	auto logFn = [](const std::string& msg) {
		std::cout << msg << std::endl;
	};

	Player user(1, "青龙军");
	Player enemy(2, "白虎军");

	std::vector<Character> userRoster = {
		createHero(1001, "关羽", 50, BattleAttr(6200, 420, 190, 105), 1, 101, {601}),
		createHero(1002, "张飞", 48, BattleAttr(6800, 360, 230, 95), 1, 105, {602}),
		createHero(1003, "赵云", 49, BattleAttr(5800, 410, 180, 115), 1, 103, {603})
	};

	std::vector<Character> enemyRoster = {
		createHero(2001, "吕布", 52, BattleAttr(6500, 450, 200, 100), 2, 106, {601, 606}),
		createHero(2002, "董卓", 47, BattleAttr(7000, 340, 260, 90), 3, 401, {602}),
		createHero(2003, "贾诩", 50, BattleAttr(5400, 380, 170, 110), 4, 502, {604})
	};

	registerTeam(user, userRoster);
	registerTeam(enemy, enemyRoster);

	BattleManager battle(&user, &enemy, logFn);
	BattleManager::Result result = battle.runBattle();

	std::cout << "==============================\n";
	std::cout << "战斗结束，结果：" << toString(result)
			  << "，共经历 " << battle.getRound() << " 回合" << std::endl;

	return 0;
}
