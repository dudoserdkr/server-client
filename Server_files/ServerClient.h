#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include "ServerSocket.h"
#include <string>
#include <iostream>
#include <poll.h>
#include <cwchar>
#include <cerrno>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

class ServerClient {
    int sockfd;

public:
    explicit ServerClient(int sockfd);
    static void sendToClientsExceptOne(std::vector<pollfd> fds, wchar_t buffer[BUFFER_SIZE], int index_of_excepted_user);
    static void sendMsgToClient(int client_sockfd, wchar_t buffer[BUFFER_SIZE]);
    int acceptClient() const;
    static int receiveFromClient(int client_sockfd, wchar_t buffer[BUFFER_SIZE]);

private:
    static void handleError(const std::string& message, bool condition);
};

#endif // SERVERCLIENT_H
