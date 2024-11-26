#include "ServerClient.h"

ServerClient::ServerClient(int sockfd) : sockfd(sockfd) {}

void ServerClient::sendMsgToClient(int client_sockfd, char buffer[1024]) {
    size_t bufferSize = sizeof(buffer);
    send(client_sockfd, buffer, bufferSize, 0);
}

int ServerClient::acceptClient() {
    sockaddr_storage addr{};
    socklen_t addrlen = sizeof(addr);
    int new_socket = accept(sockfd, (sockaddr*)&addr, &addrlen);

    if (new_socket == -1) {
        handleError("Error while accepting new connection: " + std::string(strerror(errno)), true);
    } else {
        std::string msg = "New connection from ";
        ServerSocket::print_connection(addr, msg);
    }

    return new_socket;
}

void ServerClient::sendToClientsExceptOne(vector<pollfd> fds, char buffer[1024], int index_of_excepted_user) {
    for (int i = 1; i < fds.size(); i++) {
        if (i == index_of_excepted_user) {
            continue;
        }
        sendMsgToClient(fds[i].fd, buffer);
    }
}

int ServerClient::receiveFromClient(int client_sockfd, char buffer[1024]) {
    size_t bufferSize = sizeof(buffer);
    return read(client_sockfd, buffer, bufferSize);
}

void ServerClient::handleError(const std::string& message, bool condition) {
    if (condition) {
        std::cerr << message << std::endl;
        exit(EXIT_FAILURE);
    }
}
