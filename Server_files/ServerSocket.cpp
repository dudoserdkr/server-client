#include "ServerSocket.h"

addrinfo ServerSocket::init_hints() {
    addrinfo hints{};
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    return hints;
}

addrinfo* ServerSocket::init_getaddrinfo(const addrinfo &hints) {
    addrinfo *result_of_getaddr;
    int status = getaddrinfo(this->IP, this->PORT, &hints, &result_of_getaddr);
    if (status != 0) {
        cerr << "getaddrinfo error: " << gai_strerror(status) << endl;
        exit(1);
    }
    return result_of_getaddr;
}

int ServerSocket::bind_socket_to_addr(addrinfo *result_of_getaddr) {
    addrinfo *p;
    int sock_fd;
    for (p = result_of_getaddr; p != nullptr; p = p->ai_next) {
        sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock_fd == -1) {
            continue;
        }
        if (::bind(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock_fd);
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
    return sock_fd;
}

void ServerSocket::bind_socket(addrinfo *result_of_getaddr) {
    this->sockfd = bind_socket_to_addr(result_of_getaddr);
    print_bound_adress();
}

void ServerSocket::start_listen() {
    if (listen(this->sockfd, this->backlog) == -1) {
        cerr << "Error to listen: " << strerror(errno) << endl;
        exit(3);
    }
    cout << "Server is listening on port " << PORT << endl;
}

void ServerSocket::print_bound_adress(string message) const {
    sockaddr_storage local_adress{};
    socklen_t addr_len = sizeof(local_adress);
    if (getsockname(this->sockfd, (sockaddr*)&local_adress, &addr_len) == -1) {
        cerr << "Error getting socket name: " << strerror(errno) << endl;
        return;
    }
    print_connection(local_adress, message);
}

void ServerSocket::print_connection(sockaddr_storage addr, string &message) {
    char ip_str[INET6_ADDRSTRLEN];
    if (addr.ss_family == AF_INET) {
        auto *addr_in = (sockaddr_in*)&addr;
        inet_ntop(AF_INET, &(addr_in->sin_addr), ip_str, sizeof(ip_str));
        cout << message << " IPv4 address: " << ip_str << ":" << ntohs(addr_in->sin_port) << endl;
    } else {
        auto *addr_in6 = (sockaddr_in6*)&addr;
        inet_ntop(AF_INET6, &(addr_in6->sin6_addr), ip_str, sizeof(ip_str));
        cout << message << " IPv6 address: " << ip_str << ":" << ntohs(addr_in6->sin6_port) << endl;
    }
}

void ServerSocket::kill_needless_processes() {
    struct sigaction sa{};
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, nullptr) == -1) {
        cerr << "Error setting up SIGCHLD handler: " << strerror(errno) << endl;
        exit(1);
    }
}

void ServerSocket::sigchld_handler([[maybe_unused]] int s) {
    while (waitpid(-1, nullptr, WNOHANG) > 0);
}

ServerSocket::ServerSocket(const char* IP, const char* PORT, const int backlog) : IP(IP), PORT(PORT), backlog(backlog) {}
ServerSocket::~ServerSocket() {
    if (this->sockfd > 0) {
        close(this->sockfd);
    }
}

int ServerSocket::runSocket() {
    addrinfo hints = init_hints();
    addrinfo *result_of_getaddr = init_getaddrinfo(hints);
    bind_socket(result_of_getaddr);
    start_listen();
    kill_needless_processes();
    return this->sockfd;
}