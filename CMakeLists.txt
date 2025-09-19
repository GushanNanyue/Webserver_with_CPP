cmake_minimum_required(VERSION 3.16)
project(Webserver_with_CPP)

set(CMAKE_CXX_STANDARD 14)

add_executable(server socket_server_test.cpp)
add_executable(client socket_client_test.cpp)
