#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "GameManager.h"
#include "NetworkClient.h"
#include "LogM.h"

#pragma comment(lib, "Ws2_32.lib")

namespace { // 匿名命名空间。作用是把其中声明的东西限制在当前翻译单元（main.cpp)中可见

constexpr char kDefaultHost[] = "127.0.0.1";
constexpr char kDefaultPort[] = "8080";

class ClientApplication {
public:
    ClientApplication()
        : m_client(kDefaultHost, kDefaultPort)
        , m_gameManager(GameManager::getInstance())
        , m_running(false)
        , m_messageThreadRunning(false)
    {
    }

    int run() {
        if (!initialize()) {
            return EXIT_FAILURE;
        }

        // 启动消息处理线程
        startMessageProcessThread();
        
        // 运行游戏主循环
        runGameLoop();
        
        shutdown();
        return EXIT_SUCCESS;
    }

private:
    bool initialize() {
        if (!m_client.connect()) {
            std::cerr << "[App] Failed to connect to server." << std::endl;
            return false;
        }

        m_client.startReceiveThread();
        m_running = true;
        return true;
    }

    // ========== 独立的消息处理线程 ==========
    
    void startMessageProcessThread()
    {
        m_messageThreadRunning = true;
        m_messageThread = std::thread(&ClientApplication::messageProcessThreadFunc, this);
        LOG_DEBUG("[App] Message processing thread started.");
    }
    
    void messageProcessThreadFunc()
    {
        while (m_messageThreadRunning && m_client.isConnected()) {
            // 处理所有待处理的消息
            while (m_client.hasMessage()) {
                std::string message = m_client.popMessage();
                handleServerMessage(message);
            }
            
            // 短暂休眠避免空转 CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::cout << "[App] Message processing thread exiting." << std::endl;
    }
    
    void handleServerMessage(const std::string& message)
    {
        LOG_DEBUG("[Network] Processing: %s", message.c_str());
        
        // ⚠️ 注意：这里需要线程安全地更新游戏状态
        std::lock_guard<std::mutex> lock(m_gameStateMutex);
        
    }

    // ========== 游戏主循环 ==========
    
    void runGameLoop()
    {
        // 创建游戏窗口，控制权交给QT
        using namespace std::chrono;
        const auto frameDuration = milliseconds(16);

        LOG_DEBUG("[App] Entering game loop...");
        while (m_running && m_client.isConnected()) {
            auto frameStart = steady_clock::now();
            
            
            // 帧率控制
            auto elapsed = duration_cast<milliseconds>(steady_clock::now() - frameStart);
            if (elapsed < frameDuration) {
                std::this_thread::sleep_for(frameDuration - elapsed);
            }
        }
    }
    
    void shutdown() {
        m_running = false;
        m_messageThreadRunning = false;
        
        // 等待消息处理线程结束
        if (m_messageThread.joinable()) {
            m_messageThread.join();
        }
        
        m_client.stopReceiveThread();
        m_client.disconnect();
    }

    NetworkClient m_client;
    GameManager& m_gameManager;
    bool m_running;
    
    // 消息处理线程
    std::thread m_messageThread;
    std::atomic<bool> m_messageThreadRunning;
    
    // 游戏状态保护锁
    std::mutex m_gameStateMutex;
};

} // namespace

int main()
{
    ClientApplication app;
    return app.run();
}
