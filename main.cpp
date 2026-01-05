#include <iostream>
#include <sstream>
#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "GameManager.h"
#include "Player.h"
#include "Login.h"

#pragma comment(lib, "Ws2_32.lib")

Player* loadLogin()
{
    constexpr const char* kServerHost = "127.0.0.1";
    constexpr const char* kServerPort = "8080";

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[Login] Failed to initialize WinSock." << std::endl;
        return nullptr;
    }

    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* serverInfo = nullptr;
    if (getaddrinfo(kServerHost, kServerPort, &hints, &serverInfo) != 0) {
        std::cerr << "[Login] Unable to resolve server address." << std::endl;
        WSACleanup();
        return nullptr;
    }

    SOCKET serverSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "[Login] Socket creation failed." << std::endl;
        freeaddrinfo(serverInfo);
        WSACleanup();
        return nullptr;
    }

    if (connect(serverSocket, serverInfo->ai_addr, static_cast<int>(serverInfo->ai_addrlen)) == SOCKET_ERROR) {
        std::cerr << "[Login] Unable to connect to server. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        freeaddrinfo(serverInfo);
        WSACleanup();
        return nullptr;
    }

    freeaddrinfo(serverInfo);

    std::string username;
    std::string password;
    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    const std::string payload =
        "{ \"username\": \"" + username + "\", \"password\": \"" + password + "\" }";

    std::ostringstream requestBuilder;
    requestBuilder << "POST /login HTTP/1.1\r\n"
                   << "Host: " << kServerHost << ":" << kServerPort << "\r\n"
                   << "Content-Type: application/json\r\n"
                   << "Content-Length: " << payload.size() << "\r\n"
                   << "Connection: close\r\n\r\n"
                   << payload;

    const std::string request = requestBuilder.str();
    const int bytesSent = send(serverSocket, request.c_str(), static_cast<int>(request.size()), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "[Login] Failed to send login request." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return nullptr;
    }

    std::string response;
    char buffer[1024];
    int bytesReceived = 0;
    do {
        bytesReceived = recv(serverSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            response.append(buffer, bytesReceived);
        }
    } while (bytesReceived > 0);

    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "[Login] Failed to receive server response." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return nullptr;
    }

    closesocket(serverSocket);
    WSACleanup();

    if (response.find("200") == std::string::npos) {
        std::cerr << "[Login] Server rejected the login request." << std::endl;
        return nullptr;
    }

    auto* player = new Player();
    std::cout << "Login succeeded. Player session created." << std::endl;
    return player;
}

int main()
{
    Player* player = loadLogin();
    if (player == nullptr) {
        std::cerr << "Failed to load player." << std::endl;
        return -1;
    }
    GameManager& gameManager = GameManager::getInstance();
    std::cout << "Game Manager initialized." << std::endl;

	return 0;
}
