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

#define BUFFER_SIZE 1024

class Server {

    string PORT;
    vector<pollfd> fds;

    int BACKLOG;
    int sockfd;

    char buffer[BUFFER_SIZE] = {0};
    char hello[100] = "Hello from server";

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
            cout << message << " IPv6 adress: " << ip_str << " " << ntohs(addr_in6->sin6_port) << endl;
        }
    }

public:
    Server(string port, int backlog) : PORT(port), BACKLOG(backlog) {
        init_hints();
        init_serverinfo();
        fds.push_back({sockfd, POLLIN, 0});
        start_listen();
        handle_connections();
    }

    void start_listen() {
        if (listen(this->sockfd, this->BACKLOG) == -1) {
            cerr << "Error to listen" << strerror(errno) << endl;
            exit(3);
        }

        cout << "Server is listening on port " << PORT << endl;

    }

    void handle_connections() {
        while (true) {

            if( poll(fds.data(), fds.size(), -1) == -1 ) {
                cout << "Error while poll()" << strerror(errno) << endl;
            }

            for (int i = 0; i < fds.size(); i++) {
                pollfd sock_info = this->fds[i];

                if (sock_info.fd == this->sockfd && (sock_info.revents & POLLIN)) {
                    sockaddr_storage addr;
                    socklen_t addrlen = sizeof(addr);
                    int new_socket = accept(this->sockfd, (sockaddr*)&addr, &addrlen);

                    if (new_socket == -1) {
                        cout << "Error while accepting new connection" << strerror(errno);
                    }
                    else {
                        print_connection(addr, "New connection from");

                        this->fds.push_back({new_socket, POLLIN, 0});
                    }
                }
                else if (sock_info.revents & POLLIN) {
                    int valread = read(sock_info.fd, buffer, BUFFER_SIZE);

                    if (valread == 0) {
                        cout << "Connection closed" << endl;
                        close(sock_info.fd);
                        fds.erase(fds.begin() + i);
                    }

                    else {
                        cout << "Received from client: " << buffer << endl;
                        send(sock_info.fd, hello, strlen(hello), 0);
                    }
                }
            }
        }
    }


};

int main() {
    Server serv("8080", 5);
}