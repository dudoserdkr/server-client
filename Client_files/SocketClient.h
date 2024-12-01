#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H
#include <string>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

class SocketClient {
public:
    int sockfd;
    virtual void connectToServer(const char *IP, const char *PORT) = 0;
    int getSocket() const { return sockfd; }
    virtual int sendMessage(const std::wstring &message) = 0;
    virtual int receiveMessage(wchar_t buffer[BUFFER_SIZE]) = 0;
    virtual ~SocketClient() = default;
};

#endif
