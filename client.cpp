#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>

using namespace std;

class Client {
    addrinfo hints{};

    int sockfd;

    string IP;
    string PORT;

    void init_serverinfo() {
        addrinfo *result_of_getaddr;

        int status = getaddrinfo(IP.c_str(), PORT.c_str(), &hints, &result_of_getaddr);
        if (status != 0) {
            cerr << "Problem in init_serverinfo: " << gai_strerror(status) << endl;
            exit(1);
        }

        connect_socket(result_of_getaddr);

    }

    void connect_socket(addrinfo *result_of_getaddr) {
        addrinfo *p;
        for (p = result_of_getaddr; p != nullptr; p = p->ai_next) {
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
            cerr << "Error to connect socket: " << strerror(errno) << endl;
            freeaddrinfo(result_of_getaddr);
            exit(2);
        }

        freeaddrinfo(result_of_getaddr);

        cout << "Socket successfully connected to port " << PORT << endl;
    }

public:
    Client(string IP, string PORT) : IP(IP), PORT(PORT) {
        init_serverinfo();
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "No args";
        return -1;
    }

    string IP = argv[1];
    string PORT = argv[2];

    Client cl(IP, PORT);

    while (true) {

    }

}