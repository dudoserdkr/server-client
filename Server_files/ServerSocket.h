#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

class ServerSocket {
    int sockfd{};
    const int backlog;
    const char *IP;
    const char *PORT;

    static addrinfo init_hints();
    addrinfo *init_getaddrinfo(const addrinfo &hints);
    static int bind_socket_to_addr(addrinfo *result_of_getaddr);
    void bind_socket(addrinfo *result_of_getaddr);
    void start_listen();
    void print_bound_adress(string message = "Server socket is bound to") const;
public:
    ServerSocket(const char* IP, const char* PORT, const int backlog);
    int runSocket();
    static void print_connection(sockaddr_storage addr, string &message);

};

#endif // SERVERSOCKET_H
