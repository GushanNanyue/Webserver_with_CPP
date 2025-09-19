//
// Created by cl on 2025/9/18.
//

#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket create error");
    }
    struct sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8000);

    if (connect(socket_fd, reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) < 0) {
        perror("connect error");
    }
    char str[] = "hello world";
    send(socket_fd, str, strlen(str), 0);

    close(socket_fd);
    return 0;
}