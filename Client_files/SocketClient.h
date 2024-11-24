#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H
#include <string>

class SocketClient {
public:
    int sockfd;
    virtual void connectToServer(const char *ip, const char *port) = 0;
    int getSocket() const { return sockfd; }
    virtual int sendMessage(const std::string &message) = 0;
    virtual int receiveMessage(char buffer[1024], size_t bufferSize) = 0;
    virtual ~SocketClient() = default;
};

#endif
