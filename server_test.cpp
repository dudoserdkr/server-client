#include "Client_files/CrossPlatformClient.h"
#include "Server_files/Server.h"
#include <thread>
#include <random>
#include <vector>
#include <iostream>

using namespace std;

void getRandomNumberAsCharArray(int min, int max, char buffer[], size_t bufferSize) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    int randomNumber = dis(gen);
    snprintf(buffer, bufferSize, "%d", randomNumber);
}

char PORT[20];
int BACKLOG;

void create_server() {
    Server server("0.0.0.0", PORT, BACKLOG);
}

void create_client() {
    UnixSocketClient client;
    CrossPlatformClient crossPlatformClient(&client, "0.0.0.0", PORT);
}

int main() {
    getRandomNumberAsCharArray(2000, 10000, PORT, 20);

    vector<thread> clients;

    int n;
    cout << "Enter number of clients: ";
    cin >> n;
    BACKLOG = n;
    while (n--) {
        clients.push_back(thread(create_client));
    }
    cout << "BACKLOG: " << BACKLOG << endl;

    thread server_thread(create_server);
    server_thread.join();

    for (thread &client : clients) {
        client.join();
    }
}