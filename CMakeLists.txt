cmake_minimum_required(VERSION 3.16)
project(Webserver_with_CPP)

set(CMAKE_CXX_STANDARD 17)

add_executable(server socket_server_test.cpp
        Socket.cpp
        Socket.h
        Net_Address.cpp
        Net_Address.h
        Epoll.cpp
        Epoll.h
)
add_executable(client socket_client_test.cpp)
