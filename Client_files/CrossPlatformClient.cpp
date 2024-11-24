#include "UnixSocketClient.h"
#include <string>
#include <iostream>
#include <poll.h>

using namespace std;

class CrossPlatformClient {
    SocketClient *client;

    pollfd fd;

    int sockfd;
    char *IP;
    char *PORT;

public:
    CrossPlatformClient(SocketClient *c, char *IP, char *PORT) : client(c), IP(IP), PORT(PORT) {
        connectToServer();
        initSockfd();
        fd = {this->sockfd, POLLIN, 0};
    }

    void connectToServer() {
        client->connectToServer(this->IP, this->PORT);
    }

    void initSockfd() {
        this->sockfd = client->getSocket();
    }

    void sendToServer(string &message) {
        client->sendMessage(message);
    }

    void recieveFromServer() {
        char buffer[1024];
        int valread = client->recieveMessage(buffer);

        if (valread == 0) {
            cout << "Connection closed" << endl;
        }
        else {
            cout << "Received from server: " << buffer << endl;
        }
    }

    void start_handlers() {
        while (true) {
            if (poll(&fd, 1, -1) == -1) {
                cerr << "Error while poll()" << strerror(errno) << endl;
                exit(3);
            }
            if (fd.revents & POLLIN) {
                recieveFromServer();
            }
            else {
                string message;

                getline(cin, message);

                if (!message.empty()) {
                    sendToServer(message);
                }
            }
        }
    }

    ~CrossPlatformClient() {
        if (this->sockfd > 0) {
            close(this->sockfd);
        }
    }



};