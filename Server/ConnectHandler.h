#pragma once
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#define MaxBufferSize 4096						// max size of buffer

namespace AKIRA_Net {
	class ConnectHandler {
	private:
		SOCKET client;							// connected client
		char buff[MaxBufferSize];				// recv/send buff
	public:
		ConnectHandler(SOCKET client) : client(client) {}
		void HandConnect();
	};
}

