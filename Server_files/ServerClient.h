#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include "ServerSocket.h"
#include <string>
#include <iostream>

class ServerClient {
    int sockfd;

public:
    explicit ServerClient(int sockfd);

    void sendMsgToClient(int client_sockfd, char buffer[1024]);
    int acceptClient();
    int receiveFromClient(int client_sockfd, char buffer[1024]);

private:
    static void handleError(const std::string& message, bool condition);
};

#endif // SERVERCLIENT_H
