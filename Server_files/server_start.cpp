#include "Server.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    const char *IP;
    const char *PORT;
    int BACKLOG;

    if (argc != 3) {
        cerr << "Usage: " << argv[0] << "<IP>:<PORT> BACKLOG" << endl << "0.0.0.0:12345 10 <- example" << endl;
        exit(1);
    }
    else {
        IP = (strtok(argv[1], ":"));
        PORT = strtok(NULL, ":");
        BACKLOG = atoi(argv[2]);
    }
    Server server(IP, PORT, BACKLOG);
}