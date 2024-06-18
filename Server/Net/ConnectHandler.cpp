#include <iostream>
#include "ConnectHandler.h"
#include "Utility.h"

#include "../Protobuf/Test.pb.h";

namespace AKIRA_Net {
    void ConnectHandler::HandConnect() {
	    while (true) {
            memset(buff, 0, MaxBufferSize);
            int bytesReceived = recv(client, buff, MaxBufferSize, 0);
            if (bytesReceived > 0) {
                buff[bytesReceived] = '\0';         // Null-terminate the string
                
                Protobuf::TestRequest request;
                request.ParseFromArray(buff, strlen(buff));

                std::cout << "Get Request: " << std::endl;
                std::cout << "request id: " << request.id() << std::endl;
                std::cout << "request msg: " << UTF8ToGBEx(request.msg().c_str()) << ", change msg to \"send back msg\"" << std::endl;
                request.set_msg("send back msg");
                std::cout << "request param: " << request.params() << std::endl;
                std::cout << "send back request" << std::endl;

                auto res = request.SerializeAsString();

                //std::cout << "Received message: " << UTF8ToGBEx(buff) << std::endl;
                //std::cout << "Send back message: " << UTF8ToGBEx(buff) << std::endl;
                
                // Send message back to client
                send(client, res.c_str(), strlen(res.c_str()), 0);
                //send(client, buff, strlen(buff), 0);
            }
            else if (bytesReceived == 0) { // Client closed connection
                std::cout << "Disconnect Client " << client << std::endl;
                closesocket(client);
                break;
            }
	    }
    }
}
