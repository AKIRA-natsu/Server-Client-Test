#pragma comment(lib,"ws2_32.lib")
#include <Winsock2.h>
#include <iostream>
#include <string>
#include "Utility.h"

int main()
{
    WSADATA wsaData;
    SOCKET sockServer;
    SOCKADDR_IN addrServer;
    SOCKET sockClient;
    SOCKADDR_IN addrClient;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sockServer = socket(AF_INET, SOCK_STREAM, 0);
    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY表示任何IP
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(6000);//绑定端口6000
    bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

    //Listen监听端
    listen(sockServer, 5);//5为等待连接数目
    printf("服务器已启动:\n监听中...\n");
    int len = sizeof(SOCKADDR);
    // 字符串数组的最后一位需要置为“\0”，所以初始化，并预留一位即可
    char sendBuf[101] = { 0 };//发送至客户端的字符串
    char recvBuf[101] = { 0 };//接受客户端返回的字符串

    //会阻塞进程，直到有客户端连接上来为止
    sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len);
    printf("client connected?\n");

    //接收并打印客户端数据
    recv(sockClient, recvBuf, 100, 0);
    std::cout << recvBuf << std::endl;

    std::cout << UTF8ToGBEx(recvBuf) << std::endl;

    //std::cout << "send: " << sendBuf << std::endl;
    //sockClient = send(sockServer, sendBuf, len, 0);

    //关闭socket
    closesocket(sockClient);
    WSACleanup();
}