#include "UI/BattleTui.h"

#include <algorithm>
#include <clocale>
#include <sstream>

#include "Player.h"

using namespace std;

namespace {
constexpr int kMinMidWidth = 32;
}

BattleTui::BattleTui() = default;

BattleTui::~BattleTui()
{
    shutdownCurses();
}

BattleManager::Result BattleTui::run(Player& user, Player& enemy)
{
    static bool localeInitialized = false;
    if (!localeInitialized) {
        setlocale(LC_ALL, "");
        localeInitialized = true;
    }
    initCurses();
    layoutWindows();
    logLines_.clear();

    auto callback = [this](const string& message) {
        this->pushLog(message);
        this->render();
    };
    manager_ = make_unique<BattleManager>(&user, &enemy, callback);
    BattleManager::Result result = manager_->runBattle();
    pushLog("=== TUI模式结束: " + formatResult(result) + " ===");
    render();

    mvprintw(screenRows_ > 0 ? screenRows_ - 1 : 0, 1, "战斗结束，按 q 返回主菜单");
    refresh();
    int ch = 0;
    while ((ch = getch()) != 'q' && ch != 'Q' && ch != '\n') {
        // keep waiting for exit key
    }

    manager_.reset();
    shutdownCurses();
    return result;
}

void BattleTui::initCurses()
{
    if (cursesInitialized_) {
        return;
    }
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, FALSE);
    start_color();
    use_default_colors();
    cursesInitialized_ = true;
}

void BattleTui::shutdownCurses()
{
    if (!cursesInitialized_) {
        return;
    }
    if (leftWin_) {
        delwin(leftWin_);
        leftWin_ = nullptr;
    }
    if (midWin_) {
        delwin(midWin_);
        midWin_ = nullptr;
    }
    if (rightWin_) {
        delwin(rightWin_);
        rightWin_ = nullptr;
    }
    endwin();
    cursesInitialized_ = false;
    screenRows_ = 0;
    screenCols_ = 0;
}

void BattleTui::layoutWindows()
{
    if (!cursesInitialized_) {
        return;
    }
    int rows = 0;
    int cols = 0;
    getmaxyx(stdscr, rows, cols);
    if (rows == 0 || cols == 0) {
        return;
    }
    if (rows == screenRows_ && cols == screenCols_ && leftWin_ && midWin_ && rightWin_) {
        return;
    }
    screenRows_ = rows;
    screenCols_ = cols;

    int leftWidth = max(28, cols / 5);
    int rightWidth = max(24, cols / 6);
    int midWidth = max(kMinMidWidth, cols - leftWidth - rightWidth);

    if (leftWin_) {
        delwin(leftWin_);
    }
    if (midWin_) {
        delwin(midWin_);
    }
    if (rightWin_) {
        delwin(rightWin_);
    }

    leftWin_ = newwin(rows - 1, leftWidth, 0, 0);
    midWin_ = newwin(rows - 1, midWidth, 0, leftWidth);
    rightWin_ = newwin(rows - 1, rightWidth, 0, leftWidth + midWidth);
}

void BattleTui::render()
{
    if (!cursesInitialized_) {
        return;
    }
    layoutWindows();
    if (leftWin_) {
        renderHealthPane();
    }
    if (midWin_) {
        renderLogPane();
    }
    if (rightWin_) {
        renderStatsPane();
    }
    if (screenRows_ > 0) {
        move(screenRows_ - 1, 0);
        clrtoeol();
        mvprintw(screenRows_ - 1, 1, "[TUI] q:退出  回车:退出");
        refresh();
    }
}

void BattleTui::renderHealthPane()
{
    werase(leftWin_);
    box(leftWin_, 0, 0);
    mvwprintw(leftWin_, 0, 2, "单位状态");
    int row = 1;
    if (manager_) {
        row = drawTeamSection(leftWin_, manager_->getUserTeam(), "我方", row);
        row = drawTeamSection(leftWin_, manager_->getEnemyTeam(), "敌方", row + 1);
    }
    wrefresh(leftWin_);
}

int BattleTui::drawTeamSection(WINDOW* win, const vector<BattleCharacter>& team,
    const string& label, int startRow)
{
    int width = getmaxx(win) - 2;
    if (width <= 0) {
        return startRow;
    }
    mvwprintw(win, startRow++, 1, "%s", label.c_str());
    for (const auto& ch : team) {
        string header = ch.name + " (" + to_string(ch.battleId) + ")";
        if (!ch.isAlive) {
            header += " [阵亡]";
        }
        if (static_cast<int>(header.size()) > width) {
            header.resize(width);
        }
        mvwprintw(win, startRow++, 1, "%s", header.c_str());

        int barWidth = max(4, width - 2);
        double ratio = 0.0;
        if (ch.currentAttr.maxHp > 0) {
            ratio = static_cast<double>(max<int64_t>(0, ch.currentAttr.hp)) /
                static_cast<double>(ch.currentAttr.maxHp);
        }
        ratio = clamp(ratio, 0.0, 1.0);
        int filled = static_cast<int>(ratio * barWidth + 0.5);
        string bar(barWidth, ' ');
        for (int i = 0; i < filled && i < barWidth; ++i) {
            bar[i] = '#';
        }
        string barLine = "[" + bar + "]";
        if (static_cast<int>(barLine.size()) > width) {
            barLine.resize(width);
        }
        mvwprintw(win, startRow++, 1, "%s", barLine.c_str());

        ostringstream stats;
        stats << "HP " << max<int64_t>(0, ch.currentAttr.hp) << "/" << ch.currentAttr.maxHp
              << "  怒 " << static_cast<int>(ch.currentAttr.rage)
              << "  盾 " << ch.shieldValue
              << "  Buff " << ch.buffs.size();
        string statsLine = stats.str();
        if (static_cast<int>(statsLine.size()) > width) {
            statsLine.resize(width);
        }
        mvwprintw(win, startRow++, 1, "%s", statsLine.c_str());
    }
    return startRow;
}

void BattleTui::renderLogPane()
{
    werase(midWin_);
    box(midWin_, 0, 0);
    mvwprintw(midWin_, 0, 2, "战斗日志");
    int height = 0;
    int width = 0;
    getmaxyx(midWin_, height, width);
    int innerHeight = height - 2;
    int innerWidth = width - 2;
    if (innerHeight <= 0 || innerWidth <= 0) {
        wrefresh(midWin_);
        return;
    }
    int start = static_cast<int>(logLines_.size()) - innerHeight;
    if (start < 0) {
        start = 0;
    }
    int row = 1;
    for (int i = start; i < static_cast<int>(logLines_.size()); ++i) {
        string line = logLines_[i];
        if (static_cast<int>(line.size()) > innerWidth) {
            line.resize(innerWidth);
        }
        mvwprintw(midWin_, row++, 1, "%s", line.c_str());
        if (row > innerHeight) {
            break;
        }
    }
    wrefresh(midWin_);
}

void BattleTui::renderStatsPane()
{
    werase(rightWin_);
    box(rightWin_, 0, 0);
    mvwprintw(rightWin_, 0, 2, "统计");
    if (!manager_) {
        wrefresh(rightWin_);
        return;
    }
    int row = 1;
    mvwprintw(rightWin_, row++, 1, "回合: %d/%d", manager_->getRound(), manager_->getMaxRounds());
    mvwprintw(rightWin_, row++, 1, "状态: %s", formatResult(manager_->getResult()).c_str());
    row++;

    const auto& userTeam = manager_->getUserTeam();
    const auto& enemyTeam = manager_->getEnemyTeam();
    mvwprintw(rightWin_, row++, 1, "我方存活: %d/%zu", countAlive(userTeam), userTeam.size());
    mvwprintw(rightWin_, row++, 1, "敌方存活: %d/%zu", countAlive(enemyTeam), enemyTeam.size());
    mvwprintw(rightWin_, row++, 1, "我方伤害: %lld", static_cast<long long>(sumTeamDamage(true)));
    mvwprintw(rightWin_, row++, 1, "敌方伤害: %lld", static_cast<long long>(sumTeamDamage(false)));
    row++;
    mvwprintw(rightWin_, row++, 1, "日志行数: %zu", logLines_.size());
    row++;
    mvwprintw(rightWin_, row++, 1, "Tip: 战斗结束后按 q 退出");
    wrefresh(rightWin_);
}

int BattleTui::countAlive(const vector<BattleCharacter>& team)
{
    int alive = 0;
    for (const auto& ch : team) {
        if (ch.isAlive) {
            ++alive;
        }
    }
    return alive;
}

void BattleTui::pushLog(const string& message)
{
    logLines_.push_back(message);
    while (logLines_.size() > maxLogLines_) {
        logLines_.pop_front();
    }
}

string BattleTui::formatResult(BattleManager::Result result) const
{
    switch (result) {
        case BattleManager::Result::WIN:
            return "我方胜利";
        case BattleManager::Result::LOSE:
            return "我方失败";
        case BattleManager::Result::DRAW:
            return "平局";
        default:
            return "进行中";
    }
}

int64_t BattleTui::sumTeamDamage(bool friendly) const
{
    if (!manager_) {
        return 0;
    }
    int64_t total = 0;
    const auto& stats = manager_->getDamageStats();
    for (const auto& entry : stats) {
        if ((friendly && entry.first > 0) || (!friendly && entry.first < 0)) {
            total += entry.second;
        }
    }
    return total;
}
