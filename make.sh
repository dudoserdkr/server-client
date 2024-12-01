#!/bin/bash

mkdir -p build

cd build

g++ -std=c++17 -o client ../Client_files/client_start.cpp ../Client_files/CrossPlatformClient.cpp ../Client_files/UnixSocketClient.cpp -lpthread

g++ -std=c++17 -o server ../Server_files/server_start.cpp ../Server_files/Server.cpp ../Server_files/ServerClient.cpp ../Server_files/ServerSocket.cpp -pthread
