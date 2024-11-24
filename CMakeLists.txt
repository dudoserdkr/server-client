cmake_minimum_required(VERSION 3.28)
project(server)

set(CMAKE_CXX_STANDARD 17)

add_executable(server

        Client_files/UnixSocketClient_test.cpp
        Client_files/UnixSocketClient_test.cpp
        Client_files/CrossPlatformClient.cpp
        Client_files/UnixSocketClient.h
        Client_files/UnixSocketClient.cpp
)
