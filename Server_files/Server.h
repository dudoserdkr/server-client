#ifndef SERVER_H
#define SERVER_H

#include "ServerSocket.h"
#include "ServerClient.h"
#include <map>
#include <vector>
#include <poll.h>
#include <string>

class Server {
public:

    ServerSocket *server;
    ServerClient *client;

    std::map<int, std::wstring> clients_names;
    std::vector<pollfd> fds; // File Descriptors

    int sockfd;
    const char *IP;
    const char *PORT;

public:
    Server(const char *IP, const char *PORT, const int backlog);
    ~Server();

    void addClient(int new_socket);
    void readMsgFromUser(int index_of_skipped_client);
    void handleClientDisconnection(int index_of_skipped_client, int skipped_client_socket);
    void handleClientNameChange(int skipped_client_socket, wchar_t buffer[BUFFER_SIZE]);
    void handleClientMessage(int index_of_skipped_client, int skipped_client_socket, wchar_t buffer[BUFFER_SIZE]);
    void hande_connections();
};

#endif // SERVER_H