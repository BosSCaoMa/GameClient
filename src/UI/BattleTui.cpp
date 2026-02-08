#include "UI/BattleTui.h"

#include <algorithm>
#include <clocale>
#include <sstream>

#include "Player.h"

using namespace std;

namespace {
constexpr int kMinMidWidth = 48;
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

    const int minLeft = 24;
    const int minRight = 24;
    const int minBattlefield = 20;

    int leftWidth = 0;
    int rightWidth = 0;
    int midWidth = 0;

    if (cols <= minLeft + minRight + minBattlefield) {
        leftWidth = max(12, cols / 4);
        rightWidth = max(12, cols / 4);
        midWidth = cols - leftWidth - rightWidth;
        if (midWidth < minBattlefield) {
            midWidth = max(8, midWidth);
        }
    } else {
        leftWidth = max(minLeft + 6, cols / 5);
        rightWidth = max(minRight + 2, cols / 6);
        midWidth = cols - leftWidth - rightWidth;

        if (midWidth < kMinMidWidth) {
            int need = kMinMidWidth - midWidth;
            int shrinkLeft = min(need / 2 + need % 2, leftWidth - minLeft);
            leftWidth -= shrinkLeft;
            need -= shrinkLeft;
            int shrinkRight = min(need, rightWidth - minRight);
            rightWidth -= shrinkRight;
            need -= shrinkRight;
            midWidth = cols - leftWidth - rightWidth;
        }
    }

    if (midWidth < minBattlefield) {
        midWidth = max(minBattlefield, cols - (minLeft + minRight));
        if (midWidth < minBattlefield) {
            midWidth = minBattlefield;
        }
        leftWidth = max(minLeft, (cols - midWidth) / 2);
        rightWidth = max(minRight, cols - midWidth - leftWidth);
    }

    if (leftWidth < 1) {
        leftWidth = 1;
    }
    if (rightWidth < 1) {
        rightWidth = 1;
    }
    midWidth = cols - leftWidth - rightWidth;
    if (midWidth < 1) {
        midWidth = 1;
        if (rightWidth > leftWidth) {
            rightWidth = max(1, cols - leftWidth - midWidth);
        } else {
            leftWidth = max(1, cols - rightWidth - midWidth);
        }
        midWidth = max(1, cols - leftWidth - rightWidth);
    }

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
        renderStatusPane();
    }
    if (midWin_) {
        renderBattlefieldPane();
    }
    if (rightWin_) {
        renderLogPane();
    }
    if (screenRows_ > 0) {
        move(screenRows_ - 1, 0);
        clrtoeol();
        mvprintw(screenRows_ - 1, 1, "[TUI] q:退出  回车:退出");
        refresh();
    }
}

void BattleTui::renderStatusPane()
{
    werase(leftWin_);
    box(leftWin_, 0, 0);
    mvwprintw(leftWin_, 0, 2, "状态与统计");

    if (!manager_) {
        mvwprintw(leftWin_, 1, 1, "等待战斗开始...");
        wrefresh(leftWin_);
        return;
    }

    int row = 1;
    const int height = getmaxy(leftWin_);
    const auto& userTeam = manager_->getUserTeam();
    const auto& enemyTeam = manager_->getEnemyTeam();

    mvwprintw(leftWin_, row++, 1, "回合: %d/%d", manager_->getRound(), manager_->getMaxRounds());
    mvwprintw(leftWin_, row++, 1, "状态: %s", formatResult(manager_->getResult()).c_str());
    row++;
    mvwprintw(leftWin_, row++, 1, "我方存活: %d/%zu", countAlive(userTeam), userTeam.size());
    mvwprintw(leftWin_, row++, 1, "敌方存活: %d/%zu", countAlive(enemyTeam), enemyTeam.size());
    row++;
    mvwprintw(leftWin_, row++, 1, "我方伤害: %lld", static_cast<long long>(sumTeamDamage(true)));
    mvwprintw(leftWin_, row++, 1, "敌方伤害: %lld", static_cast<long long>(sumTeamDamage(false)));
    row += 2;

    if (row < height - 2) {
        row = drawTeamSection(leftWin_, userTeam, "我方", row);
    }
    if (row < height - 2) {
        row = drawTeamSection(leftWin_, enemyTeam, "敌方", row + 1);
    }

    if (height - 2 > row) {
        mvwprintw(leftWin_, height - 2, 1, "Tip: q 返回主菜单");
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

void BattleTui::renderBattlefieldPane()
{
    werase(midWin_);
    box(midWin_, 0, 0);
    mvwprintw(midWin_, 0, 2, "战斗场景");
    if (!manager_) {
        mvwprintw(midWin_, 1, 2, "暂无战斗数据");
        wrefresh(midWin_);
        return;
    }

    int height = 0;
    int width = 0;
    getmaxyx(midWin_, height, width);
    const int innerHeight = height - 2;
    const int innerWidth = width - 2;
    if (innerHeight <= 0 || innerWidth <= 0) {
        wrefresh(midWin_);
        return;
    }

    struct FormationRows {
        vector<const BattleCharacter*> front;
        vector<const BattleCharacter*> back;
    };

    auto splitTeam = [](const vector<BattleCharacter>& team) {
        FormationRows rows;
        rows.front.reserve(team.size());
        rows.back.reserve(team.size());
        for (const auto& ch : team) {
            if (ch.isInFrontRow()) {
                rows.front.push_back(&ch);
            } else {
                rows.back.push_back(&ch);
            }
        }
        return rows;
    };

    const auto friendlyRows = splitTeam(manager_->getUserTeam());
    const auto enemyRows = splitTeam(manager_->getEnemyTeam());

    const int gap = max(2, innerWidth / 20);
    int friendlyWidth = max(20, (innerWidth - gap) / 2);
    int enemyWidth = innerWidth - gap - friendlyWidth;
    if (enemyWidth < 20) {
        enemyWidth = max(12, enemyWidth);
        friendlyWidth = innerWidth - gap - enemyWidth;
    }
    if (friendlyWidth < 12) {
        friendlyWidth = max(12, friendlyWidth);
        enemyWidth = innerWidth - gap - friendlyWidth;
    }
    const int friendlyOrigin = 1;
    const int friendlyEnd = friendlyOrigin + friendlyWidth - 1;
    const int enemyOrigin = friendlyEnd + gap;
    int enemyWidthAdjusted = enemyWidth;
    if (enemyOrigin > innerWidth) {
        enemyWidthAdjusted = 0;
    } else if (enemyOrigin + enemyWidth - 1 > innerWidth) {
        enemyWidthAdjusted = innerWidth - enemyOrigin + 1;
    }
    const int enemyEnd = enemyOrigin + max(0, enemyWidthAdjusted) - 1;
    const int dividerCol = min(innerWidth, friendlyEnd + gap / 2);
    for (int y = 1; y <= innerHeight; ++y) {
        if (dividerCol > 0 && dividerCol < width - 1) {
            mvwaddch(midWin_, y, dividerCol, ACS_VLINE);
        }
    }

    const int spriteHeight = 3;
    constexpr double kMaxDisplayRage = 100.0;

    auto makeBar = [](double ratio, int barWidth, char fillChar) {
        barWidth = max(4, barWidth);
        ratio = clamp(ratio, 0.0, 1.0);
        int filled = static_cast<int>(ratio * static_cast<double>(barWidth) + 0.5);
        string bar(barWidth, ' ');
        for (int i = 0; i < filled && i < barWidth; ++i) {
            bar[i] = fillChar;
        }
        return string("[") + bar + "]";
    };

    auto drawCharacterSprite = [&](const BattleCharacter& ch, int top, int left, int slotWidth, bool friendly) {
        if (slotWidth <= 4 || top >= height - 1) {
            return;
        }
        const int maxWidth = max(8, slotWidth - 1);
        auto trim = [&](string& line) {
            if (static_cast<int>(line.size()) > maxWidth) {
                line.resize(maxWidth);
            }
        };

        double hpRatio = 0.0;
        if (ch.currentAttr.maxHp > 0) {
            hpRatio = static_cast<double>(max<int64_t>(0, ch.currentAttr.hp)) /
                static_cast<double>(ch.currentAttr.maxHp);
        }
        string hpLine = "HP " + makeBar(hpRatio, max(4, maxWidth - 6), '#');
        trim(hpLine);

        string marker = friendly ? ">> " : "<< ";
        string statusTag;
        if (!ch.isAlive) {
            statusTag = "阵亡";
        } else if (ch.hasActed) {
            statusTag = "已动";
        } else {
            statusTag = "待命";
        }
        string nameLine = marker + ch.name + " (" + to_string(ch.battleId) + ") " + statusTag;
        trim(nameLine);

        double rageValue = static_cast<double>(max(0, static_cast<int>(ch.currentAttr.rage)));
        double rageRatio = clamp(rageValue / kMaxDisplayRage, 0.0, 1.0);
        string rageLine = "怒 " + makeBar(rageRatio, max(4, maxWidth - 10), '=');
        ostringstream extra;
        extra << " " << static_cast<int>(rageValue) << " 盾:" << ch.shieldValue;
        rageLine += extra.str();
        trim(rageLine);

        int baseY = min(top, height - 1 - spriteHeight);
        mvwprintw(midWin_, baseY, left, "%s", hpLine.c_str());
        mvwprintw(midWin_, baseY + 1, left, "%s", nameLine.c_str());
        mvwprintw(midWin_, baseY + 2, left, "%s", rageLine.c_str());
    };

    auto drawRow = [&](const vector<const BattleCharacter*>& units, int topRow, int originX,
                        int widthAvail, bool friendly) {
        if (units.empty() || widthAvail <= 0 || topRow >= height - 1) {
            return;
        }
        int slots = static_cast<int>(units.size());
        int slotWidth = widthAvail / max(1, slots);
        if (slotWidth * slots > widthAvail) {
            slotWidth = widthAvail / max(1, slots);
        }
        if (slotWidth < 8) {
            slotWidth = max(8, widthAvail / max(1, slots));
        }
        if (slotWidth * slots > widthAvail) {
            slotWidth = max(8, widthAvail / max(1, slots));
        }
        if (slotWidth <= 0) {
            slotWidth = widthAvail / max(1, slots);
        }
        if (slotWidth <= 0) {
            slotWidth = widthAvail;
        }
        int totalWidth = slotWidth * slots;
        int offset = originX + max(0, (widthAvail - totalWidth) / 2);
        for (int i = 0; i < slots; ++i) {
            int column = offset + i * slotWidth;
            if (column >= originX + widthAvail) {
                break;
            }
            drawCharacterSprite(*units[i], topRow, column, slotWidth, friendly);
        }
    };

    int friendlyAreaWidth = max(0, friendlyEnd - friendlyOrigin + 1);
    int enemyAreaWidth = max(0, enemyEnd - enemyOrigin + 1);
    int frontRowTop = 1;
    int backRowTop = min(innerHeight - spriteHeight, frontRowTop + spriteHeight + 1);

    drawRow(friendlyRows.front, frontRowTop, friendlyOrigin, friendlyAreaWidth, true);
    drawRow(friendlyRows.back, backRowTop, friendlyOrigin, friendlyAreaWidth, true);

    if (enemyWidthAdjusted > 0) {
        drawRow(enemyRows.front, frontRowTop, enemyOrigin, enemyAreaWidth, false);
        drawRow(enemyRows.back, backRowTop, enemyOrigin, enemyAreaWidth, false);
    }

    int infoRow = min(innerHeight, backRowTop + spriteHeight + 1);
    if (infoRow < height - 1) {
        mvwprintw(midWin_, infoRow, 2, "队伍分列左右，血条/怒气实时刷新");
    }

    wrefresh(midWin_);
}

void BattleTui::renderLogPane()
{
    werase(rightWin_);
    box(rightWin_, 0, 0);
    mvwprintw(rightWin_, 0, 2, "战斗日志");
    int height = 0;
    int width = 0;
    getmaxyx(rightWin_, height, width);
    int innerHeight = height - 2;
    int innerWidth = width - 2;
    if (innerHeight <= 0 || innerWidth <= 0) {
        wrefresh(rightWin_);
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
        mvwprintw(rightWin_, row++, 1, "%s", line.c_str());
        if (row > innerHeight) {
            break;
        }
    }
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
