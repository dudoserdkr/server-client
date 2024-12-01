#include "CrossPlatformClient.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    const char *IP;
    const char *PORT;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<IP>:<PORT> or 0.0.0.0:12345" << std::endl;
        exit(1);
    }
    else {
        IP = (strtok(argv[1], ":"));
        PORT = strtok(NULL, ":");
    }
    UnixSocketClient client;
    CrossPlatformClient crossPlatformClient(&client, IP, PORT);
}