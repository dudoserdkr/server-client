#include "UnixSocketClient.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <cwchar>

using namespace std;

// region static methods
void *UnixSocketClient::get_in_addr(sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((sockaddr_in*)sa)->sin_addr);
    }
    return &(((sockaddr_in6*)sa)->sin6_addr);
}

string UnixSocketClient::get_port(addrinfo *p) {
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

string UnixSocketClient::get_address(addrinfo *p) {
    char bufferIP[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family, get_in_addr((sockaddr *)p->ai_addr), bufferIP, sizeof(bufferIP));
    return string(bufferIP);
}

addrinfo UnixSocketClient::get_hints() {
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    return hints;
}

addrinfo *UnixSocketClient::init_getaddrinfo(const char *IP, const char *PORT, addrinfo &hints) {
    addrinfo *result;
    int status = getaddrinfo(IP, PORT, &hints, &result);

    if (status != 0) {
        throw runtime_error("Error in getaddrinfo: " + string(gai_strerror(status)));
    }
    return result;
}

int UnixSocketClient::connect_to_server(addrinfo *result) {
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

    printSuccessfullConnection(p);

    freeaddrinfo(result);

    return sockfd;
}

void UnixSocketClient::printSuccessfullConnection(addrinfo *p) {
    string IP = get_address(p);
    string PORT = get_port(p);

    cout << "Client: connected to " << IP << ":" << PORT << endl;
}
//endregion

// public methods
void UnixSocketClient::connectToServer(const char *IP, const char *PORT) {
    addrinfo hints = get_hints();
    addrinfo *result = init_getaddrinfo(IP, PORT, hints);
    this->sockfd = connect_to_server(result);
}

int UnixSocketClient::sendMessage(const wstring &message) {
    wchar_t buffer[BUFFER_SIZE] = {0};
    wcsncpy(buffer, message.c_str(), sizeof(buffer) / sizeof(buffer[0]) - 1); // possibly could be problematic
    return send(this->sockfd, buffer, BUFFER_SIZE, 0);
}

int UnixSocketClient::receiveMessage(wchar_t buffer[BUFFER_SIZE]) {
    int valread = recv(this->sockfd, buffer, BUFFER_SIZE * sizeof(wchar_t), 0);
    if (valread >= 0) {
        buffer[valread / sizeof(wchar_t)] = L'\0'; // Ensure null-termination
    }
    return valread;
}

//int UnixSocketClient::receiveMessage(wchar_t buffer[BUFFER_SIZE]) { <- old function
  //  int valread = read(this->sockfd, buffer, BUFFER_SIZE);
    //return valread;
//}

UnixSocketClient::~UnixSocketClient() {
    if (this->sockfd > 0) {
        close(this->sockfd);
    }
}
//endregion