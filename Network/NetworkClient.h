#pragma once
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class NetworkClient {
public:
    NetworkClient(const std::string& host, const std::string& port);
    ~NetworkClient();

    // 禁止拷贝
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;

    // 连接管理
    bool connect();
    void disconnect();
    bool isConnected() const { return m_connected; }

    // 发送接口
    bool send(const std::string& data);
    bool send(const char* data, size_t length);

    // 接收线程管理
    void startReceiveThread();
    void stopReceiveThread();
    bool isReceiveThreadRunning() const { return m_receiveRunning; }

    // 消息队列接口
    bool hasMessage() const;
    std::string popMessage();
    size_t getMessageCount() const;
    void clearMessages();

private:
    // 网络相关
    std::string m_host;
    std::string m_port;
    SOCKET m_socket;
    std::atomic<bool> m_connected;
    bool m_wsaInitialized;

    // 接收线程相关
    std::thread m_receiveThread;
    std::atomic<bool> m_receiveRunning;
    
    // 消息队列
    std::queue<std::string> m_messageQueue;
    mutable std::mutex m_queueMutex;

    // 内部方法
    bool initializeWinSock();
    void cleanupWinSock();
    void receiveThreadFunc();
};
