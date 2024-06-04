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
    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY��ʾ�κ�IP
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(6000);//�󶨶˿�6000
    bind(sockServer, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));

    //Listen������
    listen(sockServer, 5);//5Ϊ�ȴ�������Ŀ
    printf("������������:\n������...\n");
    int len = sizeof(SOCKADDR);
    // �ַ�����������һλ��Ҫ��Ϊ��\0�������Գ�ʼ������Ԥ��һλ����
    char sendBuf[101] = { 0 };//�������ͻ��˵��ַ���
    char recvBuf[101] = { 0 };//���ܿͻ��˷��ص��ַ���

    //���������̣�ֱ���пͻ�����������Ϊֹ
    sockClient = accept(sockServer, (SOCKADDR*)&addrClient, &len);
    printf("client connected?\n");

    //���ղ���ӡ�ͻ�������
    recv(sockClient, recvBuf, 100, 0);
    std::cout << recvBuf << std::endl;

    std::cout << UTF8ToGBEx(recvBuf) << std::endl;

    //std::cout << "send: " << sendBuf << std::endl;
    //sockClient = send(sockServer, sendBuf, len, 0);

    //�ر�socket
    closesocket(sockClient);
    WSACleanup();
}