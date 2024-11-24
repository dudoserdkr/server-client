#include "UnixSocketClient.h"
#include <string>
#include <iostream>
#include <thread>
#include <functional>

using namespace std;

class CrossPlatformClient {
    SocketClient *client;

    int sockfd;
    char *IP;
    char *PORT;

public:
    CrossPlatformClient(SocketClient *c, char *IP, char *PORT) : client(c), IP(IP), PORT(PORT) {
        connectToServer();
        initSockfd();
        start();
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

    void receiveFromServer() {
        char buffer[1024];
        int valread = client->receiveMessage(buffer);

        if (valread == 0) {
            cout << "Connection closed" << endl;
            closeConnection();
            exit(0);
        } else {
            cout << "Received from server: " << buffer << endl;
        }
    }

    // region Handlers
    void sendHandler() {
        string message;
        while (true) {
            getline(cin, message);

            if (message == "exit") {
                closeConnection();
                exit(0);
            }

            if (!message.empty()) {
                sendToServer(message);
            }
        }
    }

    void receiveHandler() {
        while (true) {
            receiveFromServer();
        }
    }
    // endregion

    void closeConnection() {
        if (this->sockfd > 0) {
            close(this->sockfd);
        }
    }

    void start() {
        thread thread1(bind(&CrossPlatformClient::sendHandler, this));
        thread thread2(bind(&CrossPlatformClient::receiveHandler, this));

        thread1.join();
        thread2.join();
    }

    ~CrossPlatformClient() {
        if (this->sockfd > 0) {
            close(this->sockfd);
        }
    }
};