#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include "ServerSocket.h"
#include <string>
#include <iostream>
#include <poll.h>

class ServerClient {
    int sockfd;

public:
    explicit ServerClient(int sockfd);
    void sendToClientsExceptOne(std::vector<pollfd> fds, char buffer[1024], int index_of_excepted_user);
    void sendMsgToClient(int client_sockfd, char buffer[1024]);
    int acceptClient();
    int receiveFromClient(int client_sockfd, char buffer[1024]);

private:
    static void handleError(const std::string& message, bool condition);
};

#endif // SERVERCLIENT_H
