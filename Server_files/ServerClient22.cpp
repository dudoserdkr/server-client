#include "ServerSocket.h"

class ServerClient {
    int sockfd;
public:
    ServerClient (int sockfd) : sockfd(sockfd) {}

    void sendMsgToClient(int client_sockfd, char buffer[1024]) {
        size_t bufferSize = sizeof(buffer);
        send(client_sockfd, buffer, bufferSize, 0);
    }

    int acceptClient() {
        sockaddr_storage addr{};
        socklen_t addrlen = sizeof(addr);
        int new_socket = accept(this->sockfd, (sockaddr*)&addr, &addrlen);

        if (new_socket == -1) {
            cout << "Error while accepting new connection" << strerror(errno) << endl; // TODO: Зробити норм
        }
        else {
            string msg = "New connection from";
            ServerSocket::print_connection(addr, msg);
        }

        return new_socket;
    }

    int recieveFromClient(int client_sockfd, char buffer[1024]) {
        size_t bufferSize = sizeof(buffer);
        return read(client_sockfd, buffer, bufferSize);


    }
};