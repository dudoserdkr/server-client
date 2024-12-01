#include "Server.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <poll.h>

Server::Server(const char *IP, const char *PORT, const int backlog) : IP(IP), PORT(PORT) {
    this->server = new ServerSocket(IP, PORT, backlog);
    this->sockfd = server->runSocket();
    this->client = new ServerClient(sockfd);
    this->fds.push_back({this->sockfd, POLLIN, 0});
    this->clients_names[this->sockfd] = L"Server";
    hande_connections();
}

Server::~Server() {
    delete this->server;
    delete this->client;
}

void Server::addClient(int new_socket) {
    this->clients_names[new_socket] = L"[Client " + std::to_wstring(this->clients_names.size()) + L"]";
    this->fds.push_back({new_socket, POLLIN, 0});
}

void Server::readMsgFromUser(int index_of_skipped_client) {
    wchar_t buffer[BUFFER_SIZE] = {0};
    int skipped_client_socket = fds[index_of_skipped_client].fd;
    int valread = client->receiveFromClient(skipped_client_socket, buffer);

    if (valread == 0) {
        handleClientDisconnection(index_of_skipped_client, skipped_client_socket);
    } else if (wcsncmp(buffer, L"/name", 5) == 0) {
        handleClientNameChange(skipped_client_socket, buffer);
    } else {
        handleClientMessage(index_of_skipped_client, skipped_client_socket, buffer);
    }
}

void Server::handleClientDisconnection(int index_of_skipped_client, int skipped_client_socket) {
    std::wcout << "Client " << this->clients_names[skipped_client_socket] << " has closed connection" << std::endl;
    close(skipped_client_socket);
    fds.erase(fds.begin() + index_of_skipped_client);
}

void Server::handleClientNameChange(int skipped_client_socket, wchar_t buffer[BUFFER_SIZE]) {
    std::wstring new_name = L"[" + std::wstring(buffer).substr(6) + L"]";
    this->clients_names[skipped_client_socket] = new_name;
    std::wcout << "Client " << this->clients_names[skipped_client_socket] << " has changed name" << std::endl;
}

void Server::handleClientMessage(int index_of_skipped_client, int skipped_client_socket, wchar_t buffer[BUFFER_SIZE]) {
    std::wstring client_name = clients_names[skipped_client_socket] + L": ";
    size_t total_length = client_name.length() + wcslen(buffer);

    if (total_length > BUFFER_SIZE) {
        size_t excess_length = total_length - BUFFER_SIZE;
        buffer[wcslen(buffer) - excess_length] = L'\0';
    }

    wchar_t name_with_buffer[BUFFER_SIZE];
    wcsncpy(name_with_buffer, (client_name + buffer).c_str(), BUFFER_SIZE - 1);
    name_with_buffer[BUFFER_SIZE - 1] = L'\0';
    std::wcout << L"Received from " << name_with_buffer << std::endl;
    client->sendToClientsExceptOne(fds, name_with_buffer, index_of_skipped_client);
}

void Server::hande_connections() {
    while (true) {
        if (poll(this->fds.data(), this->fds.size(), -1) == -1) {
            std::cerr << "Error while poll(): " << strerror(errno) << std::endl;
        }

        for (int i = 0; i < fds.size(); i++) {
            pollfd user_pollfd = fds[i];

            if (user_pollfd.fd == this->sockfd && (user_pollfd.revents & POLLIN)) {
                int new_socket = this->client->acceptClient();
                addClient(new_socket);
            } else if (user_pollfd.revents & POLLIN) {
                readMsgFromUser(i);
            }
        }
    }
}