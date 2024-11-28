#include "ServerSocket.h"
#include "ServerClient.h"
#include <map>
#include <cstring>

class Server {
    ServerSocket *server;
    ServerClient *client;

    std::map<int, std::wstring> clients_names;
    vector<pollfd> fds; // File Descriptors

    int sockfd;
    const char *IP;
    const char *PORT;

public:
    Server(const char *IP, const char *PORT, const int backlog) : IP(IP), PORT(PORT) {
        this->server = new ServerSocket(IP, PORT, backlog);
        this->sockfd = server->runSocket();
        this->client = new ServerClient(sockfd);
        this->fds.push_back({this->sockfd, POLLIN, 0});
        this->clients_names[this->sockfd] = L"Server";
        hande_connections();
    }

    ~Server() {
        delete this->server;
        delete this->client;
    }

    void addClient(int new_socket) {
        this->clients_names[new_socket] = L"[Client " + to_wstring(this->clients_names.size()) + L"]";
        this->fds.push_back({new_socket, POLLIN, 0});
    }

    void readMsgFromUser(int index_of_skipped_client) {
        wchar_t buffer[BUFFER_SIZE] = {0};
        int skipped_client_socket = fds[index_of_skipped_client].fd;
        int valread = ServerClient::receiveFromClient(skipped_client_socket, buffer);

        if (valread == 0) {
            handleClientDisconnection(index_of_skipped_client, skipped_client_socket);
        } else if (wcsncmp(buffer, L"/name", 5) == 0) {
            handleClientNameChange(skipped_client_socket, buffer);
        } else {
            handleClientMessage(index_of_skipped_client, skipped_client_socket, buffer);
        }
    }

    void handleClientDisconnection(int index_of_skipped_client, int skipped_client_socket) {
        wcout << "Client " << this->clients_names[skipped_client_socket] << " has closed connection" << endl;
        close(skipped_client_socket);
        fds.erase(fds.begin() + index_of_skipped_client);
    }

    void handleClientNameChange(int skipped_client_socket, wchar_t buffer[BUFFER_SIZE]) {
        wstring new_name = L"[" + wstring(buffer).substr(6) + L"]";
        this->clients_names[skipped_client_socket] = new_name;
        wcout << "Client " << this->clients_names[skipped_client_socket] << " has changed name" << endl;
    }

    void handleClientMessage(int index_of_skipped_client, int skipped_client_socket, wchar_t buffer[BUFFER_SIZE]) {
        wstring client_name = clients_names[skipped_client_socket] + L": ";
        size_t total_length = client_name.length() + wcslen(buffer);

        if (total_length > BUFFER_SIZE) {
            size_t excess_length = total_length - BUFFER_SIZE;
            buffer[wcslen(buffer) - excess_length] = L'\0';
        }

        wchar_t name_with_buffer[BUFFER_SIZE];
        wcsncpy(name_with_buffer, (client_name + buffer).c_str(), BUFFER_SIZE - 1);
        name_with_buffer[BUFFER_SIZE - 1] = L'\0';
        wcout << L"Received from " << name_with_buffer << endl;
        ServerClient::sendToClientsExceptOne(fds, name_with_buffer, index_of_skipped_client);
    }

    void hande_connections() {
        while (true) {
            if(poll(this->fds.data(), this->fds.size(), -1) == -1) {
                cerr << "Error while poll()" << strerror(errno) << endl;
            }

            for (int i = 0; i < fds.size(); i++) {
                pollfd user_pollfd = fds[i];

                if (user_pollfd.fd == this->sockfd && (user_pollfd.revents & POLLIN)) {
                    int new_socket = this->client->acceptClient();
                    addClient(new_socket);
                }

                else if (user_pollfd.revents & POLLIN) {
                    readMsgFromUser(i);
                }
            }
        }
    }
};

int main(int argc, char *argv[]) {
    const char *IP;
    const char *PORT;
    if (argc != 3) {
        IP = "0.0.0.0";
        PORT = "1701";
    }
    else {
        IP = argv[1];
        PORT = argv[2];
    }

    Server(IP, PORT, 10);
}