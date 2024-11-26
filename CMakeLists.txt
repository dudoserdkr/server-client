cmake_minimum_required(VERSION 3.28)
project(server)

set(CMAKE_CXX_STANDARD 17)

add_executable(server
        Server_files/Server.cpp
        Server_files/ServerSocket.cpp
        Server_files/ServerClient22.cpp
        Server_files/ServerClient.cpp
        Server_files/ServerClient.h
        Client_files/UnixSocketClient.cpp
        Client_files/UnixSocketClient.h
        Server_files/ServerClient22.h
        Server_files/ServerSocket.h
        Server_files/Server.h
        Server_files/Server.cpp
        Client_files/UnixSocketClient.h
)