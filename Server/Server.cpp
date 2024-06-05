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
    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY��ʾ�κ�IP
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(6000);//�󶨶˿�6000
    bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

    //Listen������
    listen(sockServer, 5);//5Ϊ�ȴ�������Ŀ
    printf("Server Started:\n");

    int len = sizeof(SOCKADDR);
    char sendBuf[100];//�������ͻ��˵��ַ���
    char recvBuf[100];//���ܿͻ��˷��ص��ַ���

    while (true) {
        std::cout << "Wait Connected...." << std::endl;
        //���������̣�ֱ���пͻ�����������Ϊֹ
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