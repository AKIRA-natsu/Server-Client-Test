#pragma comment(lib,"ws2_32.lib")
#include <Winsock2.h>
#include <iostream>
#include <string>
#include <map>
#include "Utility.h"
#include "ConnectHandler.h"
#include <thread>

int main()
{
    WSADATA wsaData;
    SOCKET sockServer;
    SOCKADDR_IN addrServer;
    SOCKADDR_IN addrClient;

    std::map<SOCKET, SOCKET> clientSockets; // Map to store client sockets
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sockServer = socket(AF_INET, SOCK_STREAM, 0);
    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY表示任何IP
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(6000);//绑定端口6000
    bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

    //Listen监听端
    listen(sockServer, 5);//5为等待连接数目
    printf("Server Started:\n");
    int len = sizeof(SOCKADDR);

    while (true) {
        std::cout << "Wait Connected...." << std::endl;
        // stop main thread and wait until client connected
        SOCKET sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len);

        if (sockClient != INVALID_SOCKET) {
            clientSockets[sockClient] = sockClient;
            std::cout << "Client: " << sockClient << " connected" << std::endl;

            ConnectHandler hander(sockClient, clientSockets);
            std::thread t(&ConnectHandler::HandConnect, hander);
            t.detach(); // Detach the thread to keep running independently
        }
    }

    closesocket(sockServer);
    WSACleanup();

}