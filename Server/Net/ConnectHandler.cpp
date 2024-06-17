#include <iostream>
#include "ConnectHandler.h"
#include "Utility.h"

namespace AKIRA_Net {
    void ConnectHandler::HandConnect() {
	    while (true) {
            memset(buff, 0, MaxBufferSize);
            int bytesReceived = recv(client, buff, MaxBufferSize, 0);
            if (bytesReceived > 0) {
                buff[bytesReceived] = '\0';         // Null-terminate the string
                std::cout << "Received message: " << UTF8ToGBEx(buff) << std::endl;
                std::cout << "Send back message: " << UTF8ToGBEx(buff) << std::endl;
                // Send message back to client
                send(client, buff, strlen(buff), 0);
            }
            else if (bytesReceived == 0) { // Client closed connection
                std::cout << "Disconnect Client " << client << std::endl;
                closesocket(client);
                break;
            }
	    }
    }
}
