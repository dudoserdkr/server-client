#include "CrossPlatformClient.h"

CrossPlatformClient::CrossPlatformClient(SocketClient *c, const char *IP, const char *PORT) : client(c), IP(IP), PORT(PORT) {
    connectToServer();
    initSockfd();
    start();
}

void CrossPlatformClient::connectToServer() {
    client->connectToServer(this->IP, this->PORT);
}

void CrossPlatformClient::initSockfd() {
    this->sockfd = client->getSocket();
}

void CrossPlatformClient::sendToServer(wstring &message) {
    client->sendMessage(message);
}

void CrossPlatformClient::receiveFromServer() {
    wchar_t buffer[BUFFER_SIZE] = {0};

    int valread = client->receiveMessage(buffer);

    if (valread == 0) {
        cout << "Connection closed" << endl;
        closeConnection();
        exit(0);
    } else {
        wcout << buffer << endl;
    }
}

void CrossPlatformClient::sendHandler() {

    while (true) {
        wstring message = L"";
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

void CrossPlatformClient::receiveHandler() {
    while (true) {
        receiveFromServer();
    }
}

void CrossPlatformClient::closeConnection() {
    if (this->sockfd > 0) {
        close(this->sockfd);
    }
}

void CrossPlatformClient::start() {
    thread thread1(bind(&CrossPlatformClient::sendHandler, this));
    thread thread2(bind(&CrossPlatformClient::receiveHandler, this));

    thread1.join();
    thread2.join();
}

CrossPlatformClient::~CrossPlatformClient() {
    if (this->sockfd > 0) {
        close(this->sockfd);
    }
}