#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <Winsock2.h>
#include <iostream>
#include <thread>
#include "Utility.h"
#include "ConnectHandler.h"
#include "Singleton.h"

namespace AKIRA_Net {
	class ServerSystem : public AKIRA_Common::Singleton<ServerSystem> {
	public:
		ServerSystem() {
            WSADATA wsaData;

            WSAStartup(MAKEWORD(2, 2), &wsaData);
            sockServer = socket(AF_INET, SOCK_STREAM, 0);
            addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY��ʾ�κ�IP
            addrServer.sin_family = AF_INET;
            addrServer.sin_port = htons(6000);//�󶨶˿�6000
            bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

            //Listen������
            listen(sockServer, 5);//5Ϊ�ȴ�������Ŀ
            std::cout << "Server Started:" << std::endl;
            len = sizeof(SOCKADDR);
		}

        ~ServerSystem() {
            closesocket(sockServer);
            WSACleanup();
        }

        void Connect() {
            while (true) {
                std::cout << "Wait Connected...." << std::endl;
                // stop main thread and wait until client connected
                SOCKET sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len);

                if (sockClient != INVALID_SOCKET) {
                    std::cout << "Client: " << sockClient << " connected" << std::endl;

                    AKIRA_Net::ConnectHandler hander(sockClient);
                    std::thread t(&AKIRA_Net::ConnectHandler::HandConnect, hander);
                    t.detach(); // Detach the thread to keep running independently
                }
            }
        }
    private:
        SOCKET sockServer;
        SOCKADDR_IN addrServer;
        SOCKADDR_IN addrClient;
        int len;
	};
}