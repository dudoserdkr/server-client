#include "ServerSocket.h"
#include <poll.h>
#include <cstring>
#include <cstdio>

class ServerClient {
    ServerSocket *server;

    vector<pollfd> fds; // File Descriptors

    int sockfd;
    const char *IP;
    const char *PORT;

public:
    ServerClient(const char *IP, const char *PORT, const int backlog) : IP(IP), PORT(PORT) {
        this->server = new ServerSocket(IP, PORT, backlog);
        this->sockfd = server->runSocket();
        this->fds.push_back({this->sockfd, POLLIN, 0});
        hande_connections();
    }

    void send2users(char buffer[1024], size_t &bufferSize, int index_of_excepted_user = 0) {
        for (int i = 1; i < fds.size(); i++) {
            if (i == index_of_excepted_user) {
                continue;
            }
            send(fds[i].fd, buffer, bufferSize, 0);
        }
    }


    void readMsgFromUser(int index_of_skipped_user) {
        char buffer[1024] = {0};
        size_t bufferSize = sizeof(buffer);

        pollfd skipped_user = fds[index_of_skipped_user];
        int valread = read(skipped_user.fd, buffer, bufferSize);

        if (valread == 0) {
            cout << "User has closed connection" << endl; // TODO: ADD WHICH USER
            close(skipped_user.fd);
            fds.erase(fds.begin() + index_of_skipped_user);
        }
        else {
            const char *prepend_format = "[Client %d]: ";
            prepend_to_buffer(buffer, bufferSize, prepend_format, index_of_skipped_user);
            cout << "Recieved: " << buffer << endl;
            send2users(buffer, bufferSize, index_of_skipped_user);
        }
    }

    void prepend_to_buffer(char* buffer, size_t buffer_size, const char* prepend_format, int client_id) { // TODO: Сделать эту функцию нормальной
        char prepend[256]; // Временный буфер для отформатированной строки
        snprintf(prepend, sizeof(prepend), prepend_format, client_id);

        size_t prepend_len = strlen(prepend);
        size_t buffer_len = strlen(buffer);

        // Если итоговый размер превышает размер буфера
        if (prepend_len + buffer_len >= buffer_size) {
            size_t available_space = buffer_size - prepend_len - 1;
            memmove(buffer + prepend_len, buffer, available_space);
            buffer[prepend_len + available_space] = '\0'; // Завершаем строку
        } else {
            memmove(buffer + prepend_len, buffer, buffer_len + 1); // Сдвигаем текущий текст
        }
        memcpy(buffer, prepend, prepend_len); // Копируем добавку в начало
    }

    void hande_connections() {
        while (true) {
            if(poll(this->fds.data(), this->fds.size(), -1) == -1) {
                cout << "Error while poll()" << strerror(errno) << endl; // TODO: Зробити норм
            }

            for (int i = 0; i < fds.size(); i++) {
                pollfd user_pollfd = fds[i];
                if (user_pollfd.fd == this->sockfd && (user_pollfd.revents & POLLIN)) {
                    sockaddr_storage addr{};
                    socklen_t addrlen = sizeof(addr);
                    int new_socket = accept(this->sockfd, (sockaddr*)&addr, &addrlen);

                    if (new_socket == -1) {
                        cout << "Error while accepting new connection" << strerror(errno) << endl; // TODO: Зробити норм
                    }
                    else {
                        string msg = "New connection from";
                        ServerSocket::print_connection(addr, msg);
                        this->fds.push_back({new_socket, POLLIN, 0});
                    }
                }
                else if (user_pollfd.revents & POLLIN) {
                    readMsgFromUser(i);
                }
            }
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        const char *IP = "0.0.0.0";
        const char *PORT = "1701";
    }

    const char *IP = argv[1];
    const char *PORT = argv[2];
    ServerClient(IP, PORT, 10);
}