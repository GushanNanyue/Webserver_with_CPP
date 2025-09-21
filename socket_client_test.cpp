//
// Created by cl on 2025/9/18.
//

#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

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



    char buf[BUFFER_SIZE];
    bzero(&buf, sizeof(buf));
    scanf("%s", buf);
    ssize_t write_bytes = send(socket_fd, buf, strlen(buf),0);
    if(write_bytes == -1){
        printf("socket already disconnected, can't write any more!\n");
        return -1;
    }

    close(socket_fd);
    return 0;
}