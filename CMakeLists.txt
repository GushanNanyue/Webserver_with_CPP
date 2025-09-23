cmake_minimum_required(VERSION 3.16)
project(Webserver_with_CPP)

set(CMAKE_CXX_STANDARD 17)

add_executable(server socket_server_test.cpp
        src/Socket.cpp
        src/Socket.h
        src/NetAddress.cpp
        src/NetAddress.h
        src/Epoll.cpp
        src/Epoll.h
        src/Channel.cpp
        src/Channel.h
        src/Server.cpp
        src/Server.h
        src/EventLoop.cpp
        src/EventLoop.h
)
add_executable(client socket_client_test.cpp)
