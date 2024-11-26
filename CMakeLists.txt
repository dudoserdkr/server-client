cmake_minimum_required(VERSION 3.28)
project(server)

set(CMAKE_CXX_STANDARD 17)

add_executable(server
        Server_files/Server.cpp
        Server_files/ServerSocket.cpp
        Server_files/ServerClient22.cpp
        Server_files/ServerClient.cpp
        Server_files/ServerClient.h
)