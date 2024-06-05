#pragma once
#include <WinSock2.h>
#include <map>
#pragma comment(lib, "ws2_32.lib")

#define MaxBufferSize 4096					// max size of buffer

class ConnectHandler {
private:
	SOCKET client;							// connected client
	char buff[MaxBufferSize];				// recv/send buff
	std::map<SOCKET, SOCKET>& clientSockets;
public:
	ConnectHandler(SOCKET client, std::map<SOCKET, SOCKET>& clientSockets) : 
			client(client), clientSockets(clientSockets) {}
	void HandConnect();
};

