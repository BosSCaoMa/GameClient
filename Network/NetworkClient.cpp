#include "NetworkClient.h"
#include <iostream>

NetworkClient::NetworkClient(const std::string& host, const std::string& port)
    : m_host(host)
    , m_port(port)
    , m_socket(INVALID_SOCKET)
    , m_connected(false)
    , m_wsaInitialized(false)
    , m_receiveRunning(false)
{
}

NetworkClient::~NetworkClient()
{
    stopReceiveThread();
    disconnect();
}

bool NetworkClient::initializeWinSock()
{
    if (m_wsaInitialized) {
        return true;
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[NetworkClient] Failed to initialize WinSock." << std::endl;
        return false;
    }

    m_wsaInitialized = true;
    return true;
}

void NetworkClient::cleanupWinSock()
{
    if (m_wsaInitialized) {
        WSACleanup();
        m_wsaInitialized = false;
    }
}

bool NetworkClient::connect()
{
    if (m_connected) {
        std::cerr << "[NetworkClient] Already connected." << std::endl;
        return true;
    }

    // 1. 初始化 WinSock
    if (!initializeWinSock()) {
        return false;
    }

    // 2. 配置地址信息
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* serverInfo = nullptr;
    if (getaddrinfo(m_host.c_str(), m_port.c_str(), &hints, &serverInfo) != 0) {
        std::cerr << "[NetworkClient] Unable to resolve server address." << std::endl;
        cleanupWinSock();
        return false;
    }

    // 3. 创建套接字
    m_socket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    if (m_socket == INVALID_SOCKET) {
        std::cerr << "[NetworkClient] Socket creation failed." << std::endl;
        freeaddrinfo(serverInfo);
        cleanupWinSock();
        return false;
    }

    // 4. 连接到服务器
    if (::connect(m_socket, serverInfo->ai_addr, static_cast<int>(serverInfo->ai_addrlen)) == SOCKET_ERROR) {
        std::cerr << "[NetworkClient] Unable to connect to server. Error: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        freeaddrinfo(serverInfo);
        cleanupWinSock();
        return false;
    }

    freeaddrinfo(serverInfo);
    m_connected = true;
    std::cout << "[NetworkClient] Connected to " << m_host << ":" << m_port << std::endl;
    return true;
}

void NetworkClient::disconnect()
{
    if (m_socket != INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        std::cout << "[NetworkClient] Disconnected." << std::endl;
    }
    m_connected = false;
    cleanupWinSock();
}

bool NetworkClient::send(const std::string& data)
{
    return send(data.c_str(), data.size());
}

bool NetworkClient::send(const char* data, size_t length)
{
    if (!m_connected) {
        std::cerr << "[NetworkClient] Not connected. Cannot send data." << std::endl;
        return false;
    }

    size_t totalSent = 0;
    while (totalSent < length) {
        int bytesSent = ::send(m_socket, 
                               data + totalSent, 
                               static_cast<int>(length - totalSent), 
                               0);
        
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "[NetworkClient] Send failed. Error: " << WSAGetLastError() << std::endl;
            return false;
        }
        
        totalSent += bytesSent;
    }

    std::cout << "[NetworkClient] Sent " << totalSent << " bytes." << std::endl;
    return true;
}

void NetworkClient::startReceiveThread()
{
    if (m_receiveRunning) {
        std::cerr << "[NetworkClient] Receive thread already running." << std::endl;
        return;
    }

    if (!m_connected) {
        std::cerr << "[NetworkClient] Not connected. Cannot start receive thread." << std::endl;
        return;
    }

    m_receiveRunning = true;
    m_receiveThread = std::thread(&NetworkClient::receiveThreadFunc, this);
    std::cout << "[NetworkClient] Receive thread started." << std::endl;
}

void NetworkClient::stopReceiveThread()
{
    if (!m_receiveRunning) {
        return;
    }

    m_receiveRunning = false;
    
    // 等待线程结束
    if (m_receiveThread.joinable()) {
        m_receiveThread.join();
    }
    
    std::cout << "[NetworkClient] Receive thread stopped." << std::endl;
}

void NetworkClient::receiveThreadFunc()
{
    char buffer[4096];
    
    while (m_receiveRunning && m_connected) {
        int bytesReceived = recv(m_socket, buffer, sizeof(buffer), 0);
        
        if (bytesReceived > 0) {
            std::string message(buffer, bytesReceived);
            
            // 加锁将消息放入队列
            {
                std::lock_guard<std::mutex> lock(m_queueMutex);
                m_messageQueue.push(message);
            }
            
            std::cout << "[NetworkClient] Received " << bytesReceived 
                      << " bytes. Queue size: " << getMessageCount() << std::endl;
        } 
        else if (bytesReceived == 0) {
            std::cout << "[NetworkClient] Connection closed by server." << std::endl;
            m_connected = false;
            break;
        }
        else {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) { // 忽略非阻塞模式的"无数据"错误
                std::cerr << "[NetworkClient] Receive error: " << error << std::endl;
                m_connected = false;
                break;
            }
        }
    }
    
    m_receiveRunning = false;
    std::cout << "[NetworkClient] Receive thread exiting." << std::endl;
}

bool NetworkClient::hasMessage() const
{
    std::lock_guard<std::mutex> lock(m_queueMutex);
    return !m_messageQueue.empty();
}

std::string NetworkClient::popMessage()
{
    std::lock_guard<std::mutex> lock(m_queueMutex);
    
    if (m_messageQueue.empty()) {
        return "";
    }
    
    std::string message = m_messageQueue.front();
    m_messageQueue.pop();
    return message;
}

size_t NetworkClient::getMessageCount() const
{
    std::lock_guard<std::mutex> lock(m_queueMutex);
    return m_messageQueue.size();
}

void NetworkClient::clearMessages()
{
    std::lock_guard<std::mutex> lock(m_queueMutex);
    
    // 清空队列
    std::queue<std::string> empty;
    std::swap(m_messageQueue, empty);
    
    std::cout << "[NetworkClient] Message queue cleared." << std::endl;
}
