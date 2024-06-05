#pragma comment(lib,"ws2_32.lib")
#include <Winsock2.h>
#include <iostream>
#include <string>
#include <map>
#include "Utility.h"

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
    char sendBuf[100];//发送至客户端的字符串
    char recvBuf[100];//接受客户端返回的字符串

    while (true) {
        std::cout << "Wait Connected...." << std::endl;
        //会阻塞进程，直到有客户端连接上来为止
        SOCKET sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len);

        if (sockClient != INVALID_SOCKET) {
            clientSockets[sockClient] = sockClient;
            std::cout << "Client: " << sockClient << " connected" << std::endl;

            while (true) {
                memset(recvBuf, 0, sizeof(recvBuf));
                int bytesReceived = recv(sockClient, recvBuf, sizeof(recvBuf), 0);
                if (bytesReceived > 0) {
                    recvBuf[bytesReceived] = '\0'; // Null-terminate the string
                    std::cout << "Received message: " << UTF8ToGBEx(recvBuf) << std::endl;
                    std::cout << "Send back message: " << UTF8ToGBEx(recvBuf) << std::endl;
                    // Send message back to client
                    send(sockClient, recvBuf, strlen(recvBuf), 0);
                }
                else if (bytesReceived == 0) { // Client closed connection
                    std::cout << "Disconnect Client " << sockClient << std::endl;
                    closesocket(sockClient);
                    clientSockets.erase(sockClient);
                    break;
                }
            }
        }
    }

    closesocket(sockServer);
    WSACleanup();

}