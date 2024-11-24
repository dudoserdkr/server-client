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

class Server {

    string PORT;
    vector<pollfd> fds;

    int BACKLOG;
    int sockfd;

    addrinfo hints{};

    void init_hints() {
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
    }

    void init_serverinfo() {
        addrinfo *result_of_getaddr;

        int status = getaddrinfo(nullptr, PORT.c_str(), &hints, &result_of_getaddr);
        if (status != 0) {
            cerr << "Problem in init_serverinfo: " << gai_strerror(status) << endl;
            exit(1);
        }

        bind_socket(result_of_getaddr);

    }

    void bind_socket(addrinfo *result_of_getaddr) {
        addrinfo *p;
        for (p = result_of_getaddr; p != nullptr; p = p->ai_next) {
            this->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

            if (this->sockfd == -1) {
                continue;
            }

            if (::bind(this->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(this->sockfd);
                continue;
            }

            break;
        }

        if (p == nullptr) {
            cerr << "Error to bind socket: " << strerror(errno) << endl;
            freeaddrinfo(result_of_getaddr);
            exit(2);
        }

        freeaddrinfo(result_of_getaddr);

        cout << "Socket successfully bound to port " << PORT << endl;
        print_bound_adress();
    }

    void print_bound_adress() {
        sockaddr_storage local_adress;
        socklen_t addr_len = sizeof(local_adress);

        if (getsockname(this->sockfd, (sockaddr*)&local_adress, &addr_len) == -1) {
            cerr << "Error getting socket name: " << strerror(errno);
            return;
        }

        char ip_str[INET6_ADDRSTRLEN];

        print_connection(local_adress, "Server bound to");
    }

    void print_connection(sockaddr_storage addr, string message) {
        char ip_str[INET6_ADDRSTRLEN];

        if (addr.ss_family == AF_INET) {
            sockaddr_in *addr_in = (sockaddr_in*)&addr;
            inet_ntop(AF_INET, &(addr_in->sin_addr), ip_str, sizeof(ip_str));
            cout << message << " IPv4 adress: " << ip_str << ":" << ntohs(addr_in->sin_port) << endl;
        }
        else {
            sockaddr_in6 *addr_in6 = (sockaddr_in6*)&addr;
            inet_ntop(AF_INET, &(addr_in6->sin6_addr), ip_str, sizeof(ip_str));
            cout << message << " IPv6 adress: " << ip_str << ":" << ntohs(addr_in6->sin6_port) << endl;
        }
    }

public:
    Server(string port, int backlog) : PORT(port), BACKLOG(backlog) {
        init_hints();
        init_serverinfo();
        fds.push_back({sockfd, POLLIN, 0});
        start_listen();
    }

    void start_listen() {
        if (listen(this->sockfd, this->BACKLOG) == -1) {
            cerr << "Error to listen" << strerror(errno) << endl;
            exit(3);
        }

        cout << "Server is listening on port " << PORT << endl;

        sockaddr_storage their_addr;
        socklen_t addr_size = sizeof their_addr;

        int new_fd = accept(this->sockfd, (struct sockaddr *)&their_addr, &addr_size);

        cout << "Aliluya" << endl;

    }


};

int main() {
    Server serv("45678", 5);
}