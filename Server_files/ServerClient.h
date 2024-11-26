#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include "ServerSocket.h"
#include <string>
#include <iostream>
#include <poll.h>
#include <cwchar>

const size_t BUFFER_SIZE = 1024;

class ServerClient {
    int sockfd;

public:
    explicit ServerClient(int sockfd);
    void sendToClientsExceptOne(std::vector<pollfd> fds, wchar_t buffer[BUFFER_SIZE], int index_of_excepted_user);
    void sendMsgToClient(int client_sockfd, wchar_t buffer[BUFFER_SIZE]);
    int acceptClient();
    int receiveFromClient(int client_sockfd, wchar_t buffer[BUFFER_SIZE]);

private:
    static void handleError(const std::string& message, bool condition);
};

#endif // SERVERCLIENT_H
