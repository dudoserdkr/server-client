#ifndef UNIX_SOCKET_CLIENT_H
#define UNIX_SOCKET_CLIENT_H

#include "SocketClient.h"
#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class UnixSocketClient : public SocketClient {
protected:
    // region Variables
    int sockfd;
    char serverAddress[INET6_ADDRSTRLEN];
    //endregion

    //region static methods
    static void *get_in_addr(sockaddr *sa);
    static std::string get_port(addrinfo *p);
    static std::string get_address(addrinfo *p);
    static addrinfo get_hints();
    static addrinfo *init_getaddrinfo(const char *IP, const char *PORT, addrinfo &hints);
    static int connect_to_server(addrinfo *result);
    static void printSuccessfullConnection(addrinfo *p);
    //endregion

public:
    void connectToServer(const char *IP, const char *PORT);
    int sendMessage(const std::string &message);
    int recieveMessage(char buffer[1024]);
    ~UnixSocketClient();
};

#endif // UNIX_SOCKET_CLIENT_H
