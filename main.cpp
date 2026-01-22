#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

#include "Player.h"
#include "CharacterConfig.h"
#include "BattleManager.h"

using namespace std;

namespace {

void ClearBadInput()
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void PrintTeam(const Player& player, const string& label)
{
	cout << label << "阵容(" << player.battleTeam.size() << "/6): ";
	if (player.battleTeam.empty()) {
		cout << "暂无武将";
	} else {
		for (int id : player.battleTeam) {
			const auto* tmpl = CharacterConfig::instance().get(id);
			if (tmpl) {
				cout << tmpl->name << "(" << tmpl->id << ") ";
			} else {
				cout << id << ' ';
			}
		}
	}
	cout << endl;
}

void ShowHeroSamples(int limit = 10)
{
	auto ids = CharacterConfig::instance().getAllIds();
	if (ids.empty()) {
		cout << "暂未加载武将配置。" << endl;
		return;
	}
	sort(ids.begin(), ids.end());
	limit = min(limit, static_cast<int>(ids.size()));
	cout << "示例武将：" << endl;
	for (int i = 0; i < limit; ++i) {
		const auto* tmpl = CharacterConfig::instance().get(ids[i]);
		if (!tmpl) {
			continue;
		}
		cout << "  " << tmpl->id << " - " << tmpl->name << endl;
	}
	cout << endl;
}

void ConfigureTeam(Player& player, const string& label)
{
	cout << "正在配置" << label << "阵容，输入-1查看示例武将，输入0结束。" << endl;
	while (true) {
		PrintTeam(player, label);
		if (player.isBattleTeamFull()) {
			cout << label << "阵容已满。" << endl;
			break;
		}
		cout << "请输入要添加的武将ID: ";
		int charId = 0;
		if (!(cin >> charId)) {
			ClearBadInput();
			cout << "输入无效，请输入数字。" << endl;
			continue;
		}
		if (charId == 0) {
			break;
		}
		if (charId == -1) {
			ShowHeroSamples();
			continue;
		}

		const auto* tmpl = CharacterConfig::instance().get(charId);
		if (!tmpl) {
			cout << "未找到该武将，请重新输入。" << endl;
			continue;
		}
		if (!player.hasCharacter(charId)) {
			player.addCharacter(CharacterConfig::instance().create(charId));
		}
		if (!player.addToBattleTeam(charId)) {
			cout << "添加失败：可能已在阵容或阵容已满。" << endl;
			continue;
		}
		cout << "已添加 " << tmpl->name << "(" << tmpl->id << ")" << endl;
	}
}

bool EnsureTeamReady(const Player& player, const string& label)
{
	if (player.battleTeam.empty()) {
		cout << label << "阵容为空，请先添加武将。" << endl;
		return false;
	}
	return true;
}

string ResultToText(BattleManager::Result result)
{
	switch (result) {
	case BattleManager::Result::WIN:
		return "我方胜利";
	case BattleManager::Result::LOSE:
		return "我方失败";
	case BattleManager::Result::DRAW:
		return "双方平局";
	default:
		return "战斗进行中";
	}
}

void StartBattle(Player& user, Player& enemy)
{
	if (!EnsureTeamReady(user, "玩家") || !EnsureTeamReady(enemy, "敌方")) {
		return;
	}
	PrintTeam(user, "玩家");
	PrintTeam(enemy, "敌方");
	BattleManager manager(&user, &enemy);
	auto result = manager.runBattle();
	cout << "战斗结束：" << ResultToText(result) << endl;
}

} // namespace

void ShowHome()
{
	cout<<"=== 游戏主界面 ==="<<endl;
	cout<<"1. 添加玩家队伍"<<endl;
	cout<<"2. 添加敌方队伍"<<endl;
	cout<<"3. 开始战斗"<<endl;
	cout<<"0. 退出"<<endl;
	cout<<"请选择操作: ";
}

int main()
{
	Player user(1, "玩家1");
	Player enemy(2, "敌人");
	bool running = true;
	while (running) {
		ShowHome();
		int choice = 0;
		if (!(cin >> choice)) {
			ClearBadInput();
			cout << "输入无效，请重新选择。" << endl;
			continue;
		}
		switch (choice) {
		case 1:
			ConfigureTeam(user, "玩家");
			break;
		case 2:
			ConfigureTeam(enemy, "敌方");
			break;
		case 3:
			StartBattle(user, enemy);
			break;
		case 0:
			running = false;
			break;
		default:
			cout << "请输入0-3之间的选项。" << endl;
			break;
		}
	}
	cout << "感谢体验，再见！" << endl;
	return 0;
}