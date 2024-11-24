#include "SocketClient.h"
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

class UnixSocketClient : public SocketClient {
protected:
    // region Variables

    int sockfd;
    char serverAddress[INET6_ADDRSTRLEN];

    //endregion


    //region static methods

    static void *get_in_addr(sockaddr *sa) {
        if (sa->sa_family == AF_INET) {
            return &(((sockaddr_in*)sa)->sin_addr);
        }
        return &(((sockaddr_in6*)sa)->sin6_addr);
    }

    static string get_port(addrinfo *p) {
        string PORT;
        if (p->ai_family == AF_INET) {
            sockaddr_in *addr_in = (sockaddr_in *)p->ai_addr;
            PORT = to_string(ntohs(addr_in->sin_port));
        } else {
            sockaddr_in6 *addr_in6 = (sockaddr_in6 *)p->ai_addr;
            PORT = to_string(ntohs(addr_in6->sin6_port));
        }

        return PORT;
    }

    static string get_address(addrinfo *p) {
        char bufferIP[INET6_ADDRSTRLEN];
        inet_ntop(p->ai_family, get_in_addr((sockaddr *)p->ai_addr), bufferIP, sizeof(bufferIP));

        return string(bufferIP);
    }

    static addrinfo get_hints() {
        addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        return hints;
    }

    static addrinfo *init_getaddrinfo(const char *IP, const char *PORT, addrinfo &hints) {
        addrinfo *result;
        int status = getaddrinfo(IP, PORT, &hints, &result);

        if (status != 0) {
            throw runtime_error("Error in getaddrinfo: " + string(gai_strerror(status)));
        }
        return result;
    }

    static int connect_to_server(addrinfo *result) {
        addrinfo *p;
        int sockfd;

        for (p = result; p != nullptr; p = p->ai_next) {
            sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (sockfd == -1) {
                continue;
            }
            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                continue;
            }
            break;
        }
        if (p == nullptr) {
            freeaddrinfo(result);
            throw runtime_error("Client: error connecting to the server: " + string(strerror(errno)));
        }

        printSuccessfullConnection(p); // TODO: possible to make this method independent of conect_to_server(), but it's not necessary

        freeaddrinfo(result);

        return sockfd;
    }

    static void printSuccessfullConnection(addrinfo *p) {
        string IP = get_address(p);
        string PORT = get_port(p);

        cout << "Client: connetected to " << IP << ":" << PORT << endl;
    }

    //endregion

public:
    void connectToServer(const char *IP, const char *PORT) {
        addrinfo hints = get_hints();
        addrinfo *result = init_getaddrinfo(IP, PORT, hints);
        this->sockfd = connect_to_server(result);
    }

    ~UnixSocketClient() {
        if (this->sockfd > 0) {
            close(this->sockfd);
        }
    }
};
