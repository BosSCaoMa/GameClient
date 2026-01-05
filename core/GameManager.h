#pragma once

// 单例模式，游戏管理器
class GameManager {
public:
    static GameManager& getInstance() {
        static GameManager instance;
        return instance;
    }

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    void initialize();

    void loadLogin();
private:
    GameManager() = default;
    ~GameManager() = default;
};