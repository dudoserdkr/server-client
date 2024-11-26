#include "UnixSocketClient.h"
#include <string>
#include <iostream>
#include <thread>
#include <functional>

using namespace std;

class CrossPlatformClient {
    SocketClient *client;

    int sockfd;
    const char *IP;
    const char *PORT;

public:
    CrossPlatformClient(SocketClient *c, const char *IP, const char *PORT) : client(c), IP(IP), PORT(PORT) {
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

    void sendToServer(wstring &message) {
        client->sendMessage(message);
    }

    void receiveFromServer() {
        wchar_t buffer[BUFFER_SIZE] = {0};

        int valread = client->receiveMessage(buffer);

        if (valread == 0) {
            cout << "Connection closed" << endl;
            closeConnection();
            exit(0);
        } else {
            wcout << L"Received from server: " << buffer << endl;
        }
    }

    // region Handlers
    void sendHandler() {
        wstring message;
        while (true) {
            getline(wcin, message);

            if (message == L"exit") {
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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        const char *IP = "0.0.0.0";
        const char *PORT = "1701";
    }

    const char *IP = argv[1];
    const char *PORT = argv[2];

    CrossPlatformClient client(new UnixSocketClient(), IP, PORT);
}