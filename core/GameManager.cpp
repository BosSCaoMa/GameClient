#include "GameManager.h"
#include <thread>

/*
#include <QT>
*/
using namespace std;
void GameManager::initialize()
{
    // 初始化游戏管理器的实现
    thread(&GameManager::loadLogin, this).detach(); // 启动一个新线程加载登录界面
}

void GameManager::loadLogin()
{
    // 加载登录界面
}

