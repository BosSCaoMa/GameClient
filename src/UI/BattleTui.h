#pragma once

#include <deque>
#include <memory>
#include <string>
#include <vector>

#if __has_include(<ncursesw/curses.h>)
#include <ncursesw/curses.h>
#elif __has_include(<ncursesw/ncurses.h>)
#include <ncursesw/ncurses.h>
#elif __has_include(<ncurses.h>)
#include <ncurses.h>
#else
#include <curses.h>
#endif

#include "BattleManager.h"

class Player;

class BattleTui {
public:
    BattleTui();
    ~BattleTui();

    BattleManager::Result run(Player& user, Player& enemy);

private:
    void initCurses();
    void shutdownCurses();
    void layoutWindows();
    void render();
    void renderHealthPane();
    void renderLogPane();
    void renderStatsPane();
    int drawTeamSection(WINDOW* win, const std::vector<BattleCharacter>& team,
        const std::string& label, int startRow);
    static int countAlive(const std::vector<BattleCharacter>& team);
    void pushLog(const std::string& message);
    std::string formatResult(BattleManager::Result result) const;
    int64_t sumTeamDamage(bool friendly) const;

    bool cursesInitialized_ = false;
    WINDOW* leftWin_ = nullptr;
    WINDOW* midWin_ = nullptr;
    WINDOW* rightWin_ = nullptr;
    int screenRows_ = 0;
    int screenCols_ = 0;
    std::deque<std::string> logLines_;
    size_t maxLogLines_ = 300;
    std::unique_ptr<BattleManager> manager_;
};
