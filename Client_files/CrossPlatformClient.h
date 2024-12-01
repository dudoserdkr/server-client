#ifndef CROSSPLATFORMCLIENT_H
#define CROSSPLATFORMCLIENT_H

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
    CrossPlatformClient(SocketClient *c, const char *IP, const char *PORT);

    void connectToServer();
    void initSockfd();
    void sendToServer(wstring &message);
    void receiveFromServer();
    void sendHandler();
    void receiveHandler();
    void closeConnection();
    void start();

    ~CrossPlatformClient();
};

#endif // CROSSPLATFORMCLIENT_H