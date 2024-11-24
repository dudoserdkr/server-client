#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

class UnixClient {
    int sockfd;
    char *IP{};
    char *PORT{};

    char serveraddress[INET6_ADDRSTRLEN];
    char buffer[1024] = {0};
    const char *hello = "Hello from client";

    void *get_in_addr(sockaddr *sa) {
        if (sa->sa_family == AF_INET) {
            return &(((sockaddr_in*)sa)->sin_addr);
        }
        return &(((sockaddr_in6*)sa)->sin6_addr);
    }

    void init_hints() {
        addrinfo hints;
        memset(&hints, 0 , sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        init_getaddrinfo(hints);
    }


    void init_getaddrinfo(addrinfo &hints) {
        addrinfo *result;
        int status = getaddrinfo(this->IP, this->PORT, &hints, &result);

        if (status != 0) {
            cerr << "Error in getaddrinfo: " << gai_strerror(status) << endl;
            exit(1); //TODO change to throw
        }
        connect_to_server(result);
    }

    void connect_to_server(addrinfo *result) {
        addrinfo *p;

        for (p = result; p != nullptr; p = p->ai_next) {
            this->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (this->sockfd == -1) {
                continue;
            }
            if (connect(this->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(this->sockfd);
                continue;
            }
            break;
        }
        if (p == nullptr) {
            cerr << "Client: error to connect to server: " << strerror(errno) << endl;
            exit(2);
        }
        inet_ntop(p->ai_family, get_in_addr((sockaddr *)p->ai_addr), serveraddress, sizeof(serveraddress));

        cout << "Client: conneting to " << serveraddress << endl;

        freeaddrinfo(result);
    }

public:
    UnixClient(char *ip, char *port) : IP(ip), PORT(port) {
        init_hints();
    }
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return -1;
    }
    UnixClient uc(argv[1], argv[2]);
}