#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "GameManager.h"
#include "NetworkClient.h"

#pragma comment(lib, "Ws2_32.lib")

namespace {

constexpr char kDefaultHost[] = "127.0.0.1";
constexpr char kDefaultPort[] = "8080";

class ClientApplication {
public:
    ClientApplication()
        : m_client(kDefaultHost, kDefaultPort)
        , m_gameManager(GameManager::getInstance())
    {
    }

    int run()
    {
        if (!initialize()) {
            return EXIT_FAILURE;
        }

        runGameLoop();
        shutdown();
        return EXIT_SUCCESS;
    }

private:
    bool initialize()
    {
        m_gameManager.initialize();
        m_gameManager.loadLogin();

        if (!m_client.connect()) {
            std::cerr << "[Main] Failed to connect to server." << std::endl;
            return false;
        }

        m_client.startReceiveThread();
        return true;
    }

    void runGameLoop()
    {
        using namespace std::chrono_literals;

        while (m_client.isConnected()) {
            processIncomingMessages();

            // TODO: 添加实际的游戏逻辑更新
            std::this_thread::sleep_for(16ms);
        }
    }

    void processIncomingMessages()
    {
        while (m_client.hasMessage()) {
            std::string message = m_client.popMessage();
            std::cout << "[Main] Processing message: " << message << std::endl;

            // TODO: 在这里解析消息并更新游戏状态
        }
    }

    void shutdown()
    {
        m_client.stopReceiveThread();
        m_client.disconnect();
    }

    NetworkClient m_client;
    GameManager& m_gameManager;
};

} // namespace

int main()
{
    ClientApplication app;
    return app.run();
}
