#include "ServerSocket.h"
#include "ServerClient.h"
#include <cstring>
#include <cstdio>

class Server {
    ServerSocket *server;
    ServerClient *client;

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
        hande_connections();
    }

    ~Server() {
        delete this->server;
        delete this->client;
    }


    void readMsgFromUser(int index_of_skipped_client) {
        char buffer[1024] = {0};

        int skipped_user_socket = fds[index_of_skipped_client].fd;

        int valread = this->client->receiveFromClient(skipped_user_socket, buffer);

        if (valread == 0) {
            cout << "Client " << index_of_skipped_client << " has closed connection" << endl;
            close(skipped_user_socket);
            fds.erase(fds.begin() + index_of_skipped_client);
        }
        else {
            cout << "Recieved: " << buffer << endl;
            this->client->sendToClientsExceptOne(fds, buffer, index_of_skipped_client);
        }
    }

    void hande_connections() {
        while (true) {
            if(poll(this->fds.data(), this->fds.size(), -1) == -1) {
                cout << "Error while poll()" << strerror(errno) << endl; // TODO: Зробити норм
            }

            for (int i = 0; i < fds.size(); i++) {
                pollfd user_pollfd = fds[i];
                if (user_pollfd.fd == this->sockfd && (user_pollfd.revents & POLLIN)) {
                        int new_socket = this->client->acceptClient();

                        if (new_socket != -1) {
                            this->fds.push_back({new_socket, POLLIN, 0});
                    }
                }
                else if (user_pollfd.revents & POLLIN) {
                    readMsgFromUser(i);
                }
            }
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        const char *IP = "0.0.0.0";
        const char *PORT = "1701";
    }

    const char *IP = argv[1];
    const char *PORT = argv[2];
    Server(IP, PORT, 10);
}